#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/slave/SlaveSystem.hpp>

namespace samchon
{
namespace protocol
{
	class ServerConnector;
};

namespace templates
{
namespace slave
{
	class SlaveClient
		: public virtual SlaveSystem
	{
	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		SlaveClient()
			: SlaveSystem()
		{
		};
		virtual ~SlaveClient() = default;

	protected:
		virtual auto createServerConnector() -> protocol::ServerConnector*
		{
			return new protocol::ServerConnector(this);
		};

	public:
		/* ---------------------------------------------------------
			METHOD OF CONNECTOR
		--------------------------------------------------------- */
		void connect(const std::string &ip, int port)
		{
			shared_ptr<protocol::ServerConnector> connector(createServerConnector());
			this->communicator_ = connector;

			connector->connect(ip, port);
		};
	};
};
};
};