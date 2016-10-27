#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>
#include <samchon/templates/service/Client.hpp>

#include <functional>
#include <samchon/HashMap.hpp>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace templates
{
namespace service
{
	class Server;
	class Client;

	/**
	 * An user.
	 * 
	 * The {@link User} is an abstract class groupping {@link Client} objects, who communicates with remote client, with 
	 * same *session id*. This {link User} represents a *remote user* literally. Within framework of remote system, 
	 * an {@link User} corresponds to a web-browser and a {@link Client} represents a window in the web-browser.
	 * 
	 * Extends this {@link User} class and override the {@link createClient} method, a factory method creating a child 
	 * {@link Client} object. I repeat, the {@link User} class represents a *remote user*, groupping {@link Client} 
	 * objects with same *session id*. If your cloud server has some processes to be handled in the **user level**, then 
	 * defines method in this {@link User} class. Methods managing **account** under below are some of them:
	 * 
	 * - {@link setAccount setAccount()}
	 * - {@link getAccountID getAccountID()}
	 * - {@link getAuthority getAuthority()}
	 * 
	 * The children {@link Client} objects, they're contained with their key, the {@link Client.getNo sequence number}.
	 * If you {@link User.erase erase} the children {@link Client} object by yourself, then their connection with the 
	 * remote clients will be {@link Client.close closed} and their {@link Client.destructor destruction method} will be 
	 * called. If you remove {@link clear all children}, then this {@link User} object will be also 
	 * {@link destructor destructed} and erased from the parent {@link Server} object.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_cloud_service.png)
	 * 
	 * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/CPP-Templates-Cloud_Service)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class User
		: public HashMap<size_t, std::shared_ptr<Client>>,
		public virtual protocol::IProtocol
	{
		friend class Server;

	private:
		typedef HashMap<size_t, std::shared_ptr<Client>> super;

		// RELATED OBJECTS
		Server *server;
		
		library::RWMutex client_map_mtx;
		
		// KEY
		std::string session_id;
		size_t sequence;

		// ACCOUTN
		std::string account;
		int authority;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Construct from its parent {@link Server}.
		 * 
		 * @param server The parent {@link Server} object.
		 */
		User(Server *server)
			: super()
		{
			this->server = server;
		};
		
		/**
		 * Default Destructor.
		 * 
		 * The {@link User} object is destructed when connections with the remote clients are all closed, that is all the 
		 * children {@link Client} objects are all removed, and 30 seconds has left. If some remote client connects within 
		  * the 30 seconds, then the {@link User} object doesn't be destructed.
		 */
		virtual ~User() = default;

	protected:
		/**
		 * Factory method creating a {@link Client} object.
		 * 
		 * @param driver A web communicator for remote client.
		 * @return A newly created {@link Client} object.
		 */
		virtual auto createClient() -> Client* = 0;

	private:
		void erase_client(Client *client)
		{
			size_t left_size;
			{
				library::UniqueWriteLock w_uk(client_map_mtx);
				left_size = erase(client->getNo());
			}

			// NO CLIENT, THEN ERASE THIS USER.
			if (left_size == 0)
				erase_user_function();
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get parent {@lin Server} object.
		 * 
		 * @return Parent {@link Server} object.
		 */
		auto getServer() const -> Server*
		{
			return server;
		};

		/**
		 * Get account id.
		 * 
		 * @return Account ID.
		 */
		auto getAccount() const -> std::string
		{
			return account;
		};

		/**
		 * Get authority.
		 * 
		 * @return Authority
		 */
		auto getAuthority() const -> int
		{
			return authority;
		};

		/**
		* Set *account id* and *authority*.
		*
		* The {@link setAccount setAccount()} is a method configuring *account id* and *authority* of this {@link User}.
		*
		* After the configuring, the {@link getAccountID account id} is enrolled into the parent {@link Server} as a
		* **key** for this {@link User} object. You can test existence and access this {@link User} object from
		* {@link Server.has Server.has()} and {@link Server.get Server.get()} with the {@link getAccountID account id}.
		* Of course, if ordinary {@link getAccountID account id} had existed, then the ordinary **key** will be
		* replaced.
		*
		* As you suggest, this {@link setAccount setAccount()} is something like a **log-in** function. If what you want
		* is not **logging-in**, but **logging-out**, then configure the *account id* to empty string ``""```` or call
		* the {@link lgout logout()} method.
		*
		* @param id To be account id.
		* @param authority To be authority.
		*/
		void setAccount(const std::string &account, int authority)
		{
			if (this->account == account) // SAME WITH BEFORE
				return;
			else if (this->account.empty() == false) // ACCOUNT IS CHANGED
			{
				// ERASE FROM ORDINARY ACCOUNT_MAP
				library::UniqueWriteLock uk(*account_map_mtx);
				account_map->erase(this->account);
			}

			// SET
			this->account = account;
			this->authority = authority;

			// REGISTER TO ACCOUNT_MAP IN ITS SERVER
			library::UniqueWriteLock uk(*account_map_mtx);
			account_map->set(account, my_weak_ptr.lock());
		};

		/**
		 * Log-out.
		 * 
		 * This {@link logout logout()} method configures {@link getAccountID account id} to empty string and 
		 * {@link getAuthority authority} to zero. 
		 * 
		 * The ordinary {@link getAccountID account id} will be also erased from the parent {@link Server} object. You 
		 * can't access this {@link User} object from {@link Server.has Server.has()} and {@link Server.get Server.get()}
		 * with the ordinary {@link getAccountID account id} more.
		 */
		void logout()
		{
			if (account.empty() == false)
				account_map->erase(account);

			account.clear();
		};

	public:
		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message.
		 * 
		 * Sends an {@link Invoke} message to all remote clients through the belonged {@link Client} objects. Sending the
		 * {@link Invoke} message to all remote clients, it's came true by passing through the 
		 * {@link Client.sendData Client.sendData()} methods.
		 * 
		 * ```typescript
		 * class protocol.service.User
		 * {
		 *     public sendData(invoke: Invoke): void
		 *     {
		 *         for (let it = this.begin(); !it.equal_to(this.end()); it = it.next())
		 *             it.second.sendData(invoke);
		 *     }
		 * }
		 * ```
		 * 
		 * @param invoke {@link Invoke} message to send to all remote clients.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			client_map_mtx.readLock();
			super clients = *this;
			client_map_mtx.readUnlock();

			for (auto it = clients.begin(); it != clients.end(); it++)
				it->second->sendData(invoke);
		};

		/**
		 * Handle a replied {@link Invoke} message.
		 * 
		 * The default {@link User.replyData User.replyData()} shifts chain to its parent {@link Server} object, by
		 * calling the {@link Server.replyData Server.replyData()} method. If there're some {@link Invoke} message to be 
		 * handled in this {@link User} level, then override this method and defines what to do with the {@link Invoke} 
		 * message in this {@link User} level.
		 * 
		 * ```typescript
		 * class protocol.service.User
		 * {
		 *     public replyData(invoke: protocol.Invoke): void
		 *     {
		 *         this.getServer().replyData(invoke);
		 *     }
		 * }
		 * 
		 * class MyUser extends protocol.service.User
		 * {
		 *     public replyData(invoke: protocol.Invoke): void
		 *     {
		 *          if (invoke.apply(this) == false) // IS TARGET TO BE HANDLED IN THIS USER LEVEL
		 *              super.replyData(invoke); // SHIFT TO SERVER
		 *     }
		 * }
		 * ```
		 * 
		 * @param invoke An {@link Invoke invoke} message to be handled in {@link User} level.
		 */
		virtual void replyData(std::shared_ptr<protocol::Invoke>) = 0;

	private:
		std::weak_ptr<User> my_weak_ptr;

		library::RWMutex *account_map_mtx;
		HashMap<std::string, std::shared_ptr<User>> *account_map;

		std::function<void()> erase_user_function;
	};
};
};
};