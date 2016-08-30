#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/WebServer.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace protocol
{
namespace service
{
	class User;
	class Client;

	class SAMCHON_FRAMEWORK_API Server
		: public WebServer,
		public virtual IProtocol
	{
		friend class User;

	private:
		typedef WebServer super;

		HashMap<std::string, std::shared_ptr<User>> session_map;
		HashMap<std::string, std::shared_ptr<User>> account_map;

		library::RWMutex session_map_mtx;
		library::RWMutex account_map_mtx;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Server();
		virtual ~Server();

	protected:
		virtual auto createUser() -> User* = 0;

	public:
		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke>);

	private:
		/* ---------------------------------------------------------
			CLIENT I/O
		--------------------------------------------------------- */
		virtual void addClient(std::shared_ptr<ClientDriver>) final;

		void erase_user(User*);
	};
};
};
};