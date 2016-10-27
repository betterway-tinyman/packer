#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystemArrayMediator.hpp>
#include <samchon/templates/external/ExternalClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * Mediator of Distributed Processing System, a server accepting slave clients.
	 *
	 * The {@link DistributedClientArrayMediator} is an abstract class, derived from {@link DistributedSystemArrayMediator}
	 * class, opening a server accepting {@link DistributedSystem distributed clients} as a **master**.
	 *
	 * Extends this {@link DistributedClientArrayMediator}, overrides {@link createServerBase createServerBase()} to
	 * determine which protocol to follow and {@link createExternalClient createExternalClient()} creating child
	 * {@link DistributedSystem} object. After the extending and overridings, open this server using the
	 * {@link open open()} method.
	 * 
	 * #### [Inherited] {@link DistributedSystemArrayMediator}
	 * @copydetails distributed::DistributedSystemArrayMediator
	 */
	template <class System = DistributedSystem>
	class DistributedClientArrayMediator
		: public DistributedSystemArrayMediator<System>,
		public external::ExternalClientArray<System>
	{
	public:
		DistributedClientArrayMediator()
			: DistributedSystemArrayMediator<System>(),
			external::ExternalClientArray<System>()
		{
		};
		virtual ~DistributedClientArrayMediator() = default;
	};
};
};
};