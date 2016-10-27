#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystemArray.hpp>
#include <samchon/templates/external/ExternalServerClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * Master of Distributed Processing System, be a server and client at the same time.
	 *
	 * The {@link DistributedServerClientArray} is an abstract class, derived from the {@link DistributedSystemArray} 
	 * class, opening a server accepting {@link Distributed distributed clients} and being a client connecting to
	 * {@link IDistributedServer distributed servers} at the same time.
	 *
	 * Extends this {@link DistributedServerClientArray} and overrides below methods creating child {@link DistributedSystem}
	 * object. After the overridings, open server with {@link open open()} method and connect to 
	 * {@link IDistributedServer distributed servers} through the {@link connect connect()} method. If you want this server to 
	 * follow web-socket protocol, then overrides {@link WebServer} virtually.
	 *
	 * - {@link createExternalClient createExternalClient()}
	 * - {@link createExternalServer createExternalServer()}
	 * 
	 * #### [Inherited] {@link DistributedSystemArray}
	 * @copydetails distributed::DistributedSystemArray
	 */
	template <class System = DistributedSystem>
	class DistributedServerClientArray
		: public DistributedSystemArray<System>,
		public external::ExternalServerClientArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		DistributedServerClientArray()
			: DistributedSystemArray<System>(),
			external::ExternalServerClientArray<System>()
		{
		};
		virtual ~DistributedServerClientArray() = default;
	};
};
};
};