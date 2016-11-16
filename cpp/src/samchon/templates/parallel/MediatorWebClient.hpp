#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/MediatorClient.hpp>
#include <samchon/protocol/WebServerConnector.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * A mediator client, driver for the master server.
	 * 
	 * The {@link MediatorWebClient} is a class being a client connecting to the **master** server, following the 
	 * web-socket protocol.
	 * 
	 * #### [Inherited] {@link MediatorSystem}
	 * @copydetails parallel::MediatorSystem
	 */
	class MediatorWebClient
		: public MediatorClient
	{
	private:
		typedef MediatorClient super;

	protected:
		std::string path_;

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
		MediatorWebClient(external::base::ExternalSystemArrayBase *system_array_, const std::string &ip, int port)
			: MediatorWebClient(system_array_, ip, port, "")
		{
		};

		/**
		 * Initializer Constructor.
		 * 
		 * @param systemArray The parent {@link ParallelSystemArrayMediator} object.
		 * @param ip IP address to connect.
		 * @param port Port number to connect.
		 * @param path Path of service which you want.
		 */
		MediatorWebClient(external::base::ExternalSystemArrayBase *system_array_, const std::string &ip, int port, const std::string &path)
			: super(system_array_, ip, port)
		{
			this->path_ = path;
		};
		virtual ~MediatorWebClient() = default;

	protected:
		virtual auto createServerConnector() -> protocol::ServerConnector*
		{
			return new protocol::WebServerConnector(this);
		};

	public:
		/* ---------------------------------------------------------
			METHOD OF CONNECTOR
		--------------------------------------------------------- */
		virtual void start() override
		{
			if (communicator_ != nullptr)
				return;

			protocol::WebServerConnector *connector = dynamic_cast<protocol::WebServerConnector*>(createServerConnector());
			communicator_.reset(connector);

			connector->connect(ip_, port_, path_);
		};
	};
};
};
};