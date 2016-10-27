#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#define KEEP_SERVICE_ALIVE auto &__keeper = __keep_alive();

namespace samchon
{
namespace templates
{
namespace service
{
	class Server;
	class User;
	class Client;

	/**
	 * A service.
	 * 
	 * The {@link Service} is an abstract class who represents a service, that is providing functions a specific page.
	 * 
	 * Extends the {@link Service} class and defines its own service, which to be provided for the specific weg page,
	 * by overriding the {@link replyData replyData()} method. Note that, the service, functions for the specific page 
	 * should be defined in this {@link Service} class, not its parent {@link Client} class who represents a remote client 
	 * and takes communication responsibility.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_cloud_service.png)
	 *
	 * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/CPP-Templates-Cloud_Service)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Service
		: public virtual protocol::IProtocol
	{
		friend class Server;
		friend class Client;

	private:
		Client *client;

		std::string path;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Construct from parent {@link Client} and requested path.
		 * 
		 * @param client Driver of remote client.
		 * @param path Requested path that identifies this {@link Service}.
		 */
		Service(Client *client, const std::string &path)
		{
			this->client = client;
			this->path = path;
		};

		/**
		 * Default Destructor.
		 * 
		 * The {@link Service} object is destructed when its parent {@link Client} object has 
		 * {@link Client.~Client destructed} or the {@link Client} object {@link Client.changeService changed} its child 
		 * {@link Service service} object to another one.
		 */
		virtual ~Service() = default;

		auto __keep_alive() const -> std::pair<std::shared_ptr<User>, std::shared_ptr<Client>>;

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		* Get client.
		*/
		auto getClient() const -> Client*
		{
			return client;
		};

		/**
		* Get requested path.
		*/
		auto getPath() const -> std::string
		{
			return path;
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message.
		 * 
		 * Sends an {@link Invoke} message to remote system through parent {@link Client} object ({@link Client.sendData}).
		 * 
		 * @param invoke An {@link Invoke} message to send to the remte system.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			((protocol::IProtocol*)client)->sendData(invoke);
		};

	private:
		std::weak_ptr<User> user_weak_ptr;
		std::weak_ptr<Client> client_weak_ptr;
	};
}
}
}