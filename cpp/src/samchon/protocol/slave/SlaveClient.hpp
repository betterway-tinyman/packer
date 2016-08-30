#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>
#include <samchon/protocol/external/ExternalServer.hpp>

#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	class SlaveClient
		: public virtual SlaveSystem,
		public virtual external::ExternalServer

	{
	public:
		SlaveClient() : 
			SlaveSystem(), 
			external::ExternalServer()
		{
		};
		virtual ~SlaveClient() = default;
	};
};
};
};