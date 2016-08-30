#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>
#include <samchon/protocol/Server.hpp>

#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	class SlaveServer
		: public virtual SlaveSystem,
		public virtual Server

	{
	public:
		SlaveServer() : SlaveSystem()
		{
		};
		virtual ~SlaveServer() = default;

	protected:
		virtual void addClient(std::shared_ptr<ClientDriver> driver) override final
		{
			this->communicator = driver;
			this->communicator->listen(this);
		};
	};
};
};
};