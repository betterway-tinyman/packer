#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/slave/SlaveSystem.hpp>
#include <samchon/protocol/IListener.hpp>

#include <samchon/templates/parallel/base/ParallelSystemArrayBase.hpp>
#include <samchon/templates/distributed/base/DistributedSystemArrayBase.hpp>
#include <samchon/templates/distributed/base/DistributedProcessBase.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace protocol
{
	class InvokeHistory;
};

namespace templates
{
namespace parallel
{
	/**
	 * A mediator, the master driver.
	 * 
	 * The {@link MediatorSystem} is an abstract class helping {@link ParallelSystemArrayMediator} can be a **slave** 
	 * system. The {@link MediatorSystem} interacts and communicates with the **master** system as a role of **slave**.
	 * 
	 * This {@link MediatorSystem} object is created in {@link ParallelSystemArrayMediator.createMediator}. Override the
	 * method and return one of them, which are derived from this {@link MediatorSystem} class, considering which  
	 * type and protocol the **master** system follows:
	 * 
	 * - A client slave connecting to master server:
	 *   - {@link MediatorClient}
	 *   - {@link MediatorWebClient}
	 * - A server slave accepting master client:
	 *   - {@link MediatorServer}
	 *   - {@link MediatorWebServer}
	 * 
	 * When the **master** orders a *parallel process* to this **slave**, then the {@link MediatorSystem} delivers the 
	 * *parallel process* to its parent {@link ParallelSystemArrayMediator} object. The 
	 * {@link ParallelSystemArrayMediator} object distributes the *parallel process* to its slaves system,
	 * {@link ParallelSystem} objects. When the *parallel process* has completed, then {@link MediatorSystem} reports the 
	 * result to its **master**.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_parallel_system.png)
	 * 
	 * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/CPP-Templates-Parallel_System),
	 *			 [Distributed System](https://github.com/samchon/framework/wiki/CPP-Templates-Distributed_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class MediatorSystem
		: public virtual slave::SlaveSystem,
		public virtual protocol::IListener
	{
	private:
		typedef slave::SlaveSystem super;

		external::base::ExternalSystemArrayBase *system_array_;
		HashMap<size_t, std::shared_ptr<protocol::InvokeHistory>> progress_list_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Construct from parent {@link ParallelSystemArrayMediator} object.
		 * 
		 * @param systemArray The parent {@link ParallelSystemArrayMediator} object.
		 */
		MediatorSystem(external::base::ExternalSystemArrayBase* systemArray)
			: super()
		{
			this->system_array_ = systemArray;
		};

		/**
		 * Default Destructor.
		 */
		virtual ~MediatorSystem() = default;

		/**
		 * Start interaction.
		 * 
		 * The {@link start start()} is an abstract method starting interaction with the **master** system. If the 
		 * **master** is a server, then connects to the **master**. Otherwise, the **master** is client, then this 
		 * {@link MediatorSystem} object wil open a server accepting the **master**.
		 */
		virtual void start() = 0;

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get parent {@link ParallelSystemArrayMediator} object.
		 */
		template <class SystemArray>
		auto getSystemArray() const -> SystemArray*
		{
			return (SystemArray*)system_array_;
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		void _Complete_history(size_t uid)
		{
			//--------
			// NEED TO REDEFINE START AND END TIME
			//--------
			// NO SUCH HISTORY; THE PROCESS HAD DONE ONLY IN THIS MEDIATOR LEVEL.
			if (progress_list_.has(uid) == false)
				return;

			// COMPLETE THE HISTORY
			std::shared_ptr<protocol::InvokeHistory> history = progress_list_.get(uid);
			history->complete();

			// ERASE THE HISTORY ON PROGRESS LIST
			progress_list_.erase(uid);

			// REPORT THE HISTORY TO MASTER
			sendData(history->toInvoke());
		};

	private:
		virtual void _replyData(std::shared_ptr<protocol::Invoke> invoke) override final
		{
			if (invoke->has("_History_uid") == true)
			{
				// REGISTER THIS PROCESS ON HISTORY LIST
				std::shared_ptr<protocol::InvokeHistory> history(new protocol::InvokeHistory(invoke));
				progress_list_.insert({ history->getUID(), history });

				if (invoke->has("_Piece_first") == true)
				{
					// PARALLEL PROCESS
					size_t first = invoke->get("_Piece_first")->getValue<size_t>();
					size_t last = invoke->get("_Piece_last")->getValue<size_t>();

					invoke->erase(invoke->end() - 2, invoke->end());
					((base::ParallelSystemArrayBase*)system_array_)->sendPieceData(invoke, first, last);
				}
				else if (invoke->has("_Process_name") == true)
				{
					// DISTRIBUTED PROCESS
					auto ds_system_array = (distributed::base::DistributedSystemArrayBase*)system_array_;

					// FIND THE MATCHED ROLE
					const std::string &process_name = invoke->get("_Process_name")->getValue<std::string>();
					if (ds_system_array->hasProcess(process_name) == false)
						return;

					// SEND DATA VIA THE ROLE
					auto process = ds_system_array->getProcess(process_name);
					((distributed::base::DistributedProcessBase*)(process.get()))->sendData(invoke, 1.0);
				}
			}
			else
				replyData(invoke);
		};

	public:
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			((protocol::IProtocol*)system_array_)->sendData(invoke);
		};
	};
};
};
};