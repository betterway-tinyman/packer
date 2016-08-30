#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/MediatorClient.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API MediatorWebClient
		: public MediatorClient
	{
	public:
		MediatorWebClient(external::ExternalClientArray *systemArray, const std::string &ip, int port);
		virtual ~MediatorWebClient();

	protected:
		virtual auto createServerConnector() -> ServerConnector* override;
	};
};
};
};