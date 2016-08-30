#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace protocol
{
namespace service
{
	class Server;
	class Client;

	class SAMCHON_FRAMEWORK_API User
		: public virtual IProtocol
	{
		friend class Server;
		friend class Client;

	private:
		// RELATED OBJECTS
		Server *server;
		std::weak_ptr<User> my_weak_ptr;

		HashMap<size_t, std::shared_ptr<Client>> client_map;
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
		User();
		virtual ~User();

	protected:
		virtual auto createClient() -> Client* = 0;

	private:
		void erase_client(Client*);

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto getServer() const -> Server*
		{
			return server;
		};
		auto getAccount() const -> std::string
		{
			return account;
		};
		auto getAuthority() const -> int
		{
			return authority;
		};

	protected:
		void setAccount(const std::string &account, int authority);

	public:
		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke>);
	};
};
};
};