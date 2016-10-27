#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystemArrayMediator.hpp>
#include <samchon/templates/external/ExternalServerArray.hpp>

#include <samchon/templates/distributed/DistributedServer.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * Mediator of Distributed Processing System, a client connecting to slave servers.
	 *
	 * The {@link DistributedServerArrayMediator} is an abstract class, derived from {@link DistributedSystemArrayMediator}
	 * class, connecting to {@link IDistributedServer distributed servers}.
	 *
	 * Extends this {@link DistributedServerArrayMediator} and overrides {@link createChild createChild()} method creating
	 * child {@link IDistributedServer} object. After the extending and overriding, construct children
	 * {@link IDistributedServer} objects and call the {@link connect connect()} method.
	 * 
	 * #### [Inherited] {@link DistributedSystemArrayMediator}
	 * @copydetails distributed::DistributedSystemArrayMediator
	 */
	template <class System = DistributedServer>
	class DistributedServerArrayMediator
		: public DistributedSystemArrayMediator<System>,
		public external::ExternalServerArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		DistributedServerArrayMediator()
			: DistributedSystemArrayMediator<System>(),
			external::ExternalServerArray<System>()
		{
		};
		virtual ~DistributedServerArrayMediator() = default;
	};
};
};
};