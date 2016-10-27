#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/slave/SlaveSystem.hpp>
#include <samchon/protocol/Server.hpp>

#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace templates
{
namespace slave
{
	class SlaveServer
		: public virtual SlaveSystem,
		public virtual protocol::Server

	{
	public:
		SlaveServer()
			: SlaveSystem(), 
			protocol::Server()
		{
		};
		virtual ~SlaveServer() = default;

	protected:
		virtual void addClient(std::shared_ptr<protocol::ClientDriver> driver) override
		{
			this->communicator_ = driver;
			driver->listen(this);
		};
	};
};
};
};