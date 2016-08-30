#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#define KEEP_SERVICE_ALIVE auto &__keeper = __keep_alive();

namespace samchon
{
namespace protocol
{
namespace service
{
	class Server;
	class User;
	class Client;

	class SAMCHON_FRAMEWORK_API Service
		: public virtual IProtocol
	{
		friend class Server;

	private:
		Client *client;

		std::string path;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Service();
		virtual ~Service();

	protected:
		virtual void start() = 0;

		auto __keep_alive() const -> std::pair<std::shared_ptr<User>, std::shared_ptr<Client>>;

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto getClient() const -> Client*
		{
			return client;
		};
		auto getPath() const -> std::string
		{
			return path;
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke>);
	};
}
}
}