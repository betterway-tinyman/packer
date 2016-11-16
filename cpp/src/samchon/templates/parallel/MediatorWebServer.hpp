#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/MediatorServer.hpp>
#include <samchon/protocol/WebServer.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * A mediator server, driver for the master client.
	 * 
	 * The {@link MediatorWebServer} is a class opening a server accepting the **master** client, following the 
	 * web-socket protocol.
	 * 
	 * #### [Inherited] {@link MediatorSystem}
	 * @copydetails parallel::MediatorSystem
	 */
	class MediatorWebServer
		: public MediatorServer,
		public virtual protocol::WebServer
	{
	private:
		typedef MediatorServer super;

	public:
		/**
		 * Initializer Constructor.
		 * 
		 * @param systemArray The parent {@link ParallelSystemArrayMediator} object.
		 * @param port Port number of server to open.
		 */
		MediatorWebServer(external::base::ExternalSystemArrayBase *system_array, int port)
			: super(system_array, port)
		{
		};
		virtual ~MediatorWebServer() = default;
	};
};
};
};