#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/MediatorSystem.hpp>
#include <samchon/protocol/external/ExternalServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API MediatorClient
		: public MediatorSystem,
		public external::ExternalServer
	{
	public:
		MediatorClient(external::ExternalClientArray *systemArray, const std::string &ip, int port);
		virtual ~MediatorClient();

		virtual void start();
	};
};
};
};