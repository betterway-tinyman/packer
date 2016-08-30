#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#define KEEP_CLIENT_ALIVE auto &__keeper = __keep_alive();

namespace samchon
{
namespace protocol
{
	class ClientDriver;

namespace service
{
	class Server;
	class User;
	class Service;

	class SAMCHON_FRAMEWORK_API Client
		: public virtual IProtocol
	{
		friend class Server;
		friend class User;
		friend class Service;

	private:
		// RELATED OBJECTS
		User *user;
		std::weak_ptr<Client> my_weak_ptr;
		std::shared_ptr<ClientDriver> driver;
		std::unique_ptr<Service> service;

		// KEY
		size_t no;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Client();
		virtual ~Client();

	protected:
		virtual auto createService(const std::string &) -> Service* = 0;

		auto __keep_alive() const -> std::pair<std::shared_ptr<User>, std::shared_ptr<Client>>;

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto getUser() const -> User*
		{
			return user;
		};
		auto getService() const -> Service*
		{
			return service.get();
		};

	public:
		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke>);
		virtual void replyData(std::shared_ptr<Invoke>);
	};
};
};
};