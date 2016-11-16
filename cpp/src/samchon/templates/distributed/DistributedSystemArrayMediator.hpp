#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystemArray.hpp>
#include <samchon/templates/parallel/ParallelSystemArrayMediator.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * Mediator of Distributed Processing System.
	 * 
	 * The {@link DistributedSystemArrayMediator} class be a master for its slave systems, and be a slave to its master
	 * system at the same time. This {@link DistributedSystemArrayMediator} be a master system, containing and managing
	 * {@link DistributedSystem} objects, which represent distributed slave systems, by extending 
	 * {@link DistributedSystemArray} class. Also, be a slave system through {@link getMediator mediator} object, which is 
	 * derived from the {@link SlavSystem} class.
	 *
	 * As a master, you can specify this {@link DistributedSystemArrayMediator} class to be <i>a master server accepting
	 * slave clients<i> or <i>a master client to connecting slave servers</i>. Even both of them is possible. Extends one
	 * of them below and overrides abstract factory method(s) creating the child {@link DistributedSystem} object.
	 *
	 * - {@link DistributedClientArrayMediator}: A server accepting {@link DistributedSystem distributed clients}.
	 * - {@link DistributedServerArrayMediator}: A client connecting to {@link DistributedServer distributed servers}.
	 * - {@link DistributedServerClientArrayMediator}: Both of them. Accepts {@link DistributedSystem distributed clients} and
	 *                                                 connects to {@link DistributedServer distributed servers} at the 
	 *                                                 same time.
	 *
	 * As a slave, you can specify this {@link DistributedSystemArrayMediator} to be <i>a client slave connecting to master
	 * server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
	 * Overrides the {@link createMediator createMediator()} method and return one of them:
	 * 
	 * - A client slave connecting to master server:
	 *   - {@link MediatorClient}
	 *   - {@link MediatorWebClient}
	 * - A server slave accepting master client:
	 *   - {@link MediatorServer}
	 *   - {@link MediatorWebServer}
	 * 
	 * #### [Inherited] {@link DistributedSystemArray}
	 * @copydetails distributed::DistributedSystemArray
	 */
	template <class System = DistributedSystem>
	class DistributedSystemArrayMediator
		: public virtual DistributedSystemArray<System>,
		public virtual parallel::ParallelSystemArrayMediator<System>
	{
	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		DistributedSystemArrayMediator()
			: DistributedSystemArray<System>(),
			ParallelSystemArrayMediator<System>()
		{
		};
		virtual ~DistributedSystemArrayMediator() = default;

	protected:
		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual auto _Complete_history(std::shared_ptr<slave::InvokeHistory> history) -> bool override
		{
			bool ret = DistributedSystemArray::_Complete_history(history);
			if (ret == true)
				getMediator()->_Complete_history(history->getUID());

			return ret;
		};
	};
};
};
};