#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystemArrayMediator.hpp>
#include <samchon/templates/external/ExternalServerClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * Mediator of Distributed Processing System, be a server and client at the same time as a **master**.
	 *
	 * The {@link DistributedServerClientArrayMediator} is an abstract class, derived from the
	 * {@link DistributedSystemArrayMediator} class, opening a server accepting {@link DistributedSystem distributed 
	 * clients} and being a client connecting to {@link IDistributedServer distributed servers} at the same time.
	 *
	 * Extends this {@link DistributedServerClientArrayMediator} and overrides below methods creating child 
	 * {@link DistributedSystem} object. After the overridings, open server with {@link open open()} method and connect to 
	 * {@link IDistributedServer distributed servers} through the {@link connect connect()} method. If you want this server to 
	 * follow web-socket protocol, then overrides {@link WebServer} virtually.
	 *
	 * - {@link createExternalClient createExternalClient()}
	 * - {@link createExternalServer createExternalServer()}
	 * 
	 * #### [Inherited] {@link DistributedSystemArrayMediator}
	 * @copydetails distributed::DistributedSystemArray
	 */
	template <class System = DistributedSystem>
	class DistributedServerClientArrayMediator
		: public DistributedSystemArrayMediator<System>,
		public external::ExternalServerClientArray<System>
	{
	public:
		DistributedServerClientArrayMediator()
			: DistributedSystemArray<System>(),
			external::ExternalServerClientArray<System>()
		{
		};
		virtual ~DistributedServerClientArrayMediator() = default;
	};
};
};
};