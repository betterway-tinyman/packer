#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/MediatorSystem.hpp>
#include <samchon/protocol/ServerConnector.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * A mediator client, driver for the master server.
	 * 
	 * The {@link MediatorServer} is a class being a client connecting to the **master** server, following the protocol 
	 * of Samchon Framework's own.
	 * 
	 * #### [Inherited] {@link MediatorSystem}
	 * @copydetails parallel::MediatorSystem 
	 */
	class MediatorClient
		: public MediatorSystem
	{
	private:
		typedef MediatorSystem super;

	protected:
		std::string ip_;
		int port_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Initializer Constructor.
		 * 
		 * @param systemArray The parent {@link ParallelSystemArrayMediator} object.
		 * @param ip IP address to connect.
		 * @param port Port number to connect.
		 */
		MediatorClient(external::base::ExternalSystemArrayBase *system_array_, const std::string &ip, int port)
			: super(system_array_)
		{
			this->ip_ = ip;
			this->port_ = port;
		};
		virtual ~MediatorClient() = default;

	protected:
		/**
		 * Factory method creating {@link ServerConnector} object.
		 * 
		 * The {@link createServerConnector createServerConnector()} is an abstract method creating 
		 * {@link ServerConnector} object. Overrides and returns one of them, considering which protocol the **master** 
		 * server follows:
		 * 
		 * - {@link ServerConnector}
		 * - {@link WebServerConnector}
		 * 
		 * @return A newly created {@link ServerConnector} object.
		 */
		virtual auto createServerConnector() -> protocol::ServerConnector*
		{
			return new protocol::ServerConnector(this);
		};

	public:
		/* ---------------------------------------------------------
			METHOD OF CONNECTOR
		--------------------------------------------------------- */
		virtual void start() override
		{
			if (communicator_ != nullptr)
				return;

			protocol::ServerConnector *connector = createServerConnector();
			communicator_.reset(connector);
			
			connector->connect(ip_, port_);
		};
	};
};
};
};