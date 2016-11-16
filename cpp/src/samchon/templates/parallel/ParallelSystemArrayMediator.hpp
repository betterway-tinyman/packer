#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystemArray.hpp>
#include <samchon/templates/parallel/MediatorSystem.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * Mediator of Parallel Processing System.
	 * 
	 * The {@link ParallelSystemArrayMediator} class be a **master** for its slave systems, and be a **slave** to its
	 * master system at the same time. This {@link ParallelSystemArrayMediator} be a **master **system, containing and
	 * managing {@link ParallelSystem} objects, which represent parallel slave systems, by extending
	 * {@link ParallelSystemArray} class. Also, be a **slave** system through {@link getMediator mediator} object, which is
	 * derived from the {@link SlavSystem} class.
	 * 
	 * As a **master**, you can specify this {@link ParallelSystemArrayMediator} class to be <i>a master server accepting 
	 * slave clients<i> or <i>a master client to connecting slave servers</i>. Even both of them is possible. Extends one
	 * of them below and overrides abstract factory method(s) creating the child {@link ParallelSystem} object.
	 *
	 * - {@link ParallelClientArrayMediator}: A server accepting {@link ParallelSystem parallel clients}.
	 * - {@link ParallelServerArrayMediator}: A client connecting to {@link ParallelServer parallel servers}.
	 * - {@link ParallelServerClientArrayMediator}: Both of them. Accepts {@link ParallelSystem parallel clients} and 
	 *                                              connects to {@link ParallelServer parallel servers} at the same time.
	 * 
	 * As a **slave**, you can specify this {@link ParallelSystemArrayMediator} to be <i>a client slave connecting to
	 * master server</i> or <i>a server slave accepting master client</i> by overriding the {@link createMediator} method.
	 * Overrides the {@link createMediator createMediator()} method and return one of them:
	 * 
	 * - A client slave connecting to master server:
	 *   - {@link MediatorClient}
	 *   - {@link MediatorWebClient}
	 * - A server slave accepting master client:
	 *   - {@link MediatorServer}
	 *   - {@link MediatorWebServer}
	 * 
	 * #### [Inherited] {@link ParallelSystemArray}
	 * @copydetails parallel::ParallelSystemArray
	 */
	template <class System = ParallelSystem>
	class ParallelSystemArrayMediator
		: public virtual ParallelSystemArray<System>
	{
	private:
		typedef ParallelSystemArray<System> super;

		std::unique_ptr<MediatorSystem> mediator_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystemArrayMediator()
			: super()
		{
		};
		virtual ~ParallelSystemArrayMediator() = default;

	protected:
		/**
		 * Factory method creating a {@link MediatorSystem} object.
		 * 
		 * The {@link createMediator createMediator()} is an abstract method creating the {@link MediatorSystem} object. 
		 * 
		 * You know what? this {@link ParallelSystemArrayMediator} class be a **master** for its slave systems, and be a 
		 * **slave** to its master system at the same time. The {@link MediatorSystem} object makes it possible; be a 
		 * **slave** system. This {@link createMediator} determines specific type of the {@link MediatorSystem}.
		 * 
		 * Overrides the {@link createMediator createMediator()} method to create and return one of them following which
		 * protocol and which type of remote connection (server or client) will be used: 
		 * 
		 * - A client slave connecting to master server:
		 *   - {@link MediatorClient}
		 *   - {@link MediatorWebClient}
		 * - A server slave accepting master client:
		 *   - {@link MediatorServer}
		 *   - {@link MediatorWebServer}
		 * 
		 * @return A newly created {@link MediatorSystem} object.
		 */
		virtual auto createMediator() -> MediatorSystem* = 0;

		/**
		 * Start mediator.
		 * 
		 * If the {@link getMediator mediator} is a type of server, then opens the server accepting master client. 
		 * Otherwise, the {@link getMediator mediator} is a type of client, then connects the master server.
		 */
		virtual void startMediator()
		{
			if (mediator_ != nullptr)
				return;

			mediator_.reset(createMediator());
			mediator_->start();
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get {@link MediatorSystem} object.
		 * 
		 * When you need to send an {@link Invoke} message to the master system of this 
		 * {@link ParallelSystemArrayMediator}, then send to the {@link MediatorSystem} through this {@link getMediator}.
		 * 
		 * ```cpp
		 * getMediator()->sendData(...);
		 * ```
		 * 
		 * @return The {@link MediatorSystem} object.
		 */
		auto getMediator() const -> MediatorSystem*
		{
			return mediator_.get();
		};

	protected:
		/* ---------------------------------------------------------
			HISTORY HANDLER
		--------------------------------------------------------- */
		virtual auto _Complete_history(std::shared_ptr<slave::InvokeHistory> history) -> bool override
		{
			bool ret = super::_Complete_history(history);
			if (ret == true)
				mediator_->_Complete_history(history->getUID());

			return ret;
		};
	};
};
};
};