#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/MediatorServer.hpp>

#include <samchon/protocol/WebServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API MediatorWebServer
		: public MediatorServer,
		public virtual WebServer
	{
	public:
		MediatorWebServer(external::ExternalClientArray *systemArray, int port);
		virtual ~MediatorWebServer();
	};
};
};
};