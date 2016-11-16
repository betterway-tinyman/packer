#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>
#include <samchon/templates/service/Service.hpp>

#include <samchon/protocol/WebClientDriver.hpp>

#define KEEP_CLIENT_ALIVE auto &__keeper = __keep_alive();

namespace samchon
{
namespace templates
{
namespace service
{
	class Server;
	class User;

	/**
	 * A driver of remote client.
	 * 
	 * The {@link Client} is an abstract class representing and interacting with a remote client. It deals the network 
	 * communication with the remote client and shifts {@link Invoke} message to related {@link User} and {@link Service} 
	 * objects.
	 * 
	 * Extends this {@link Client} class and override the {@link createService} method, a factory method creating a child
	 * {@link Service} object. Note that, {@link Client} represents a remote client, not *an user*, a specific *web page* 
	 * or *service*. Do not define logics about user or account information. It must be declared in the parent 
	 * {@link User} class. Also, don't define processes of a specific a web page or service. Defines them in the child
	 * {@link Service} class.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_cloud_service.png)
	 *
	 * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/CPP-Templates-Cloud_Service)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Client
		: public virtual protocol::IProtocol
	{
		friend class Server;

	private:
		// RELATED OBJECTS
		User *user;
		
		std::shared_ptr<protocol::WebClientDriver> driver;
		std::unique_ptr<Service> service;

		// KEY
		size_t no;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Construct from parent {@link User} and communicator.
		 * 
		 * @param user Parent {@link User} object.
		 * @param driver Communicator with remote client.
		 */
		Client(User *user, std::shared_ptr<protocol::WebClientDriver> driver)
		{
			this->user = user;
			this->driver = driver;
		};

		/**
		 * Default Destructor.
		 * 
		 * {@link Client} object is destructed when connection with the remote client is closed or this {@link Client} 
		 * object is {@link User.erase erased} from its parent {@link User} object.
		 */
		virtual ~Client() = default;

	protected:
		/**
		 * Factory method creating {@link Service} object.
		 * 
		 * @param path Requested path.
		 * @return A newly created {@link Service} object or ```null```.
		 */
		virtual auto createService(const std::string &) -> Service* = 0;

		auto __keep_alive() const -> std::pair<std::shared_ptr<User>, std::shared_ptr<Client>>
		{
			return std::make_pair(user_weak_ptr.lock(), my_weak_ptr.lock());
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get parent {@link User} object.
		 * 
		 * Get the parent {@link User} object, who is groupping {@link Client} objects with same session id.
		 * 
		 * @return The parent {@link User} object.
		 */
		auto getUser() const -> User*
		{
			return user;
		};

		/**
		 * Get child {@link Service} object.
		 * 
		 * @return The child {@link Service} object.
		 */
		auto getService() const -> Service*
		{
			return service.get();
		};

		/**
		 * Get sequence number. 
		 * 
		 * Get sequence number of this {@link Client} object in the parent {@link User} object. This sequence number also
		 * be a *key* in the parent {@link User} object, who extended the ```std.HashMap<number, Client>```.
		 *
		 * @return Sequence number.
		 */
		auto getNo() const -> size_t
		{
			return no;
		};

		/**
		 * Change related {@link Service} object.
		 * 
		 * @param path Requested, identifier path.
		 */
		void changeService(const std::string &path)
		{
			service.reset(createService(path));
		};

		/**
		 * Change {@link Service} to another.
		 * 
		 * @param service {@link service} object to newly assigned.
		 */
		void changeService(Service *obj)
		{
			service.reset(obj);

			service->user_weak_ptr = user_weak_ptr;
			service->client_weak_ptr = my_weak_ptr;
		};

	public:
		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message. 
		 * 
		 * Sends an {@link Invoke} message to remote client.
		 * 
		 * @param invoke An {@link Invoke} messgae to send to remote client.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			driver->sendData(invoke);
		};

		/**
		 * Handle a replied {@link Invoke} message.
		 * 
		 * The default {@link Client.replyData Client.replyData()} shifts chain to its parent {@link User} and belonged 
		 * {@link Service} objects, by calling the the {@link User.replyData User.replyData()} and 
		 * {@link Service.replyData Service.replyData()} methods.
		 * 
		 * Note that, {@link Client} represents a remote client, not *an user*, a specific *web page* or *service*. Do not 
		 * define logics about user or account information. It must be declared in the parent {@link User} class. Also, 
		 * don't define processes of a specific a web page or service. Defines them in the child {@link Service} class.
		 * 
		 * ```typescript
		 * class protocol.service.Client
		 * {
		 *     public replyData(invoke: protocol.Invoke): void
		 *     {
		 *         // SHIFT TO PARENT USER
		 *         // THE PARENT USER ALSO MAY SHIFT TO ITS PARENT SERVER
		 *         this.getUser().replyData(invoke);
		 *         
		 *         // SHIFT TO BELOGED SERVICE
		 *         if (this.getService() != null)
		 *             this.getService().replyData(invoke);
		 *     }
		 * }
		 * 
		 * class MyClient extends protocol.service.Client
		 * {
		 *     public replyData(invoke: protocol.Invoke): void
		 *     {
		 *         if (invoke.getListener() == "do_something_in_client_level")
		 *             this.do_something_in_client_level();
		 *         else
		 *             super.replyData(invoke);
		 *     }
		 * }
		 * ```
		 * 
		 * @param invoke An {@link Invoke invoke} message to be handled in {@link Client} level.
		 */
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke)
		{
			((protocol::IProtocol*)user)->replyData(invoke);
			service->replyData(invoke);
		};

	private:
		std::weak_ptr<User> user_weak_ptr;
		std::weak_ptr<Client> my_weak_ptr;
	};
};
};
};