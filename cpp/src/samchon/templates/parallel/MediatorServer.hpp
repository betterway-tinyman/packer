#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/MediatorSystem.hpp>
#include <samchon/protocol/Server.hpp>

#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * A mediator server, driver for the master client.
	 * 
	 * The {@link MediatorServer} is a class opening a server accepting the **master** client, following the protocol of 
	 * Samchon Framework's own.
	 * 
	 * #### [Inherited] {@link MediatorSystem}
	 * @copydetails parallel::MediatorSystem
	 */
	class MediatorServer
		: public MediatorSystem,
		public virtual protocol::Server
	{
	private:
		typedef MediatorSystem super;

		int port_;

	public:
		/**
		 * Initializer Constructor.
		 * 
		 * @param systemArray The parent {@link ParallelSystemArrayMediator} object.
		 * @param port Port number of server to open.
		 */
		MediatorServer(ParallelSystemArrayMediator *system_array, int port)
			: super(system_array),
			protocol::Server()
		{
			this->port_ = port;
		};
		virtual ~MediatorServer() = default;

		virtual void start() override
		{
			open(port_);
		};

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