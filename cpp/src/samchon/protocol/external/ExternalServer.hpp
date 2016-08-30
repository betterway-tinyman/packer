#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystem.hpp>

namespace samchon
{
namespace protocol
{
	class ServerConnector;

namespace external
{
	class SAMCHON_FRAMEWORK_API ExternalServer
		: public virtual ExternalSystem
	{
		friend class ExternalSystem;

	private:
		typedef ExternalSystem super;

	protected:
		std::string ip;
		int port;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ExternalServer();
		virtual ~ExternalServer();

	protected:
		virtual auto createServerConnector() -> ServerConnector*;

	public:
		/* ---------------------------------------------------------
			NETWORK
		--------------------------------------------------------- */
		virtual void connect();

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto getIP() const -> std::string
		{
			return ip;
		};
		auto getPort() const -> int
		{
			return port;
		};
	};
};
};
};