#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystem.hpp>
#include <samchon/templates/external/ExternalServer.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * A driver for distributed slave server.
	 *
	 * The {@link DistributedServer} is an abstract class, derived from the {@link DistributedSystem} class, connecting to
	 * remote, distributed **slave** server. Extends this {@link DistributedServer} class and overrides the
	 * {@link createServerConnector createServerConnector()} method following which protocol the **slave** server uses.
	 * 
	 * #### [Inheritdoc] {@link DistributedSystem}
	 * @copydetails distributed::DistributedSystem
	 */
	class DistributedServer
		: public DistributedSystem,
		public external::ExternalServer
	{
	public:
		/**
		 * Default Constructor.
		 */
		DistributedServer(external::base::ExternalSystemArrayBase *systemArray)
			: DistributedSystem(systemArray),
			external::ExternalServer(systemArray)
		{
			this->system_array_ = systemArray;
		};
		virtual ~DistributedServer() = default;
	};
};
};
};