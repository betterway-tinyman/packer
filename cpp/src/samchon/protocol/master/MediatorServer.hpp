#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/MediatorSystem.hpp>
#include <samchon/protocol/Server.hpp>

namespace samchon
{
namespace protocol
{
	class ClientDriver;

namespace master
{
	class SAMCHON_FRAMEWORK_API MediatorServer
		: public MediatorSystem,
		public virtual Server
	{
	protected:
		int port;

	public:
		MediatorServer(external::ExternalClientArray *systemArray, int port);
		virtual ~MediatorServer();

		virtual void start();

	protected:
		virtual void addClient(std::shared_ptr<ClientDriver> driver) override final;
	};
};
};
};