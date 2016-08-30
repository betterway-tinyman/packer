#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystem.hpp>
#include <samchon/protocol/external/ExternalServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API ParallelServer
		: public ParallelSystem,
		public external::ExternalServer
	{
	public:
		ParallelServer(ParallelSystemArray *systemArray);
		virtual ~ParallelServer();
	};
};
};
};