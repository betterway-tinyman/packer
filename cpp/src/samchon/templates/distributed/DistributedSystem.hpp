#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystem.hpp>

#include <samchon/templates/distributed/DSInvokeHistory.hpp>
#include <samchon/templates/distributed/base/DistributedSystemArrayBase.hpp>
#include <samchon/templates/distributed/base/DistributedProcessBase.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * A driver for a distributed slave system.
	 * 
	 * The {@link DistributedSystem} is an abstract class represents a **slave** system in *Distributed Processing System*,
	 * connected with this **master** system. This {@link DistributedSystem} takes full charge of network communication 
	 * with the remote, distributed **slave** system has connected.
	 * 
	 * This {@link DistributedSystem} has a {@link getPerformance performance index} that indicates how much the **slave** 
	 * system is fast. The {@link getPerformance performance index} is referenced and revaluated whenever those methods 
	 * are called:
	 * 
	 * - Requesting a *parallel process*
	 *   - {@link DistributedSystemArray.sendSegmentData}
	 *   - {@link DistributedSystemArray.sendPieceData}
	 * - Requesting a *distributed process*: {@link DistributedProcess.sendData}
	 * 
	 * Note that, this {@link DistributedSystem} class derived from the {@link ExternalSystem} class. Thus, this 
	 * {@link DistributedSystem} can also have children {@link ExternalSystemRole} objects exclusively. However, the 
	 * children {@link ExternalSystemRole roles} objects are different with the {@link DistributedProcess}. The 
	 * domestic {@link ExternalSystemRole roles} are belonged to only a specific {@link DistributedSystem} object. 
	 * Otherwise, the {@link DistributedProcess} objects are belonged to a {@link DistributedSystemArray} object. 
	 * Furthermore, the relationship between this {@link DistributedSystem} and {@link DistributedProcess} classes are 
	 * **M: N Associative**.
	 * 
	 * Articles     | {@link DistributedProcess}     | {@link ExternalSystemRole}
	 * -------------|--------------------------------|----------------------------
	 * Belonged to  | {@link DistributedSystemArray} | {@link DistributedSystem}
	 * Relationship | M: N Associative               | 1: N Composite
	 * Ownership    | References                     | Exclusive possession
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_distributed_system.png)
	 * 
	 * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/CPP-Templates-Distributed_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class DistributedSystem
		: public virtual parallel::ParallelSystem
	{
	private:
		typedef parallel::ParallelSystem super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		using super::super;

		virtual ~DistributedSystem()
		{
			_Set_excluded();

			// SHIFT PARALLEL INVOKE MESSAGES HAD PROGRESSED TO OTHER SLAVES
			for (auto it = _Get_progress_list().begin(); it != _Get_progress_list().end(); it++)
			{
				// INVOKE MESSAGE AND ITS HISTORY ON PROGRESS
				std::shared_ptr<protocol::Invoke> invoke = it->second.first;
				std::shared_ptr<slave::InvokeHistory> history = it->second.second;

				// SEND THEM BACK
				_Send_back_history(invoke, history);
			}
		};

	protected:
		/**
		 * Factory method creating a {@link ExternalSystemRole child} object.
		 * 
		 * In {@link distributed} module, the process class {@link DistributedProcess} is not belonged to a specific 
		 * {@link DistributedSystem} object. It only belongs to a {@link DistributedSystemArray} object and has a 
		 * **M: N Associative Relationship** between this {@link DistributedSystem} class.
		 * 
		 * By that reason, it's the normal case that the {@link DistributedSystem} object does not have any children 
		 * {@link ExternalSystemRole} object. Thus, default {@link createChild} returns ```null```.
		 * 
		 * However, if you want a {@link DistributedSystem} to have its own domestic {@link ExternalSystemRole} objects
		 * without reference to the {@link DistributedProcess} objects, it is possible. Creates and returns the 
		 * domestic {@link ExternalSystemRole} object.
		 * 
		 * @param xml {@link XML} represents the {@link ExternalSystemRole child} object.
		 * @return A newly created {@link ExternalSystemRole} object or ```null```.
		 */
		virtual auto createChild(std::shared_ptr<library::XML>) -> external::ExternalSystemRole* override
		{
			return nullptr;
		};

	public:
		auto _Compute_average_elapsed_time() const -> double
		{
			double sum = 0.0;
			size_t denominator = 0;

			for (auto it = _Get_history_list().begin(); it != _Get_history_list().end(); it++)
			{
				std::shared_ptr<DSInvokeHistory> history = std::dynamic_pointer_cast<DSInvokeHistory>(it->second);
				if (history == nullptr)
					continue;

				double elapsed_time = history->computeElapsedTime() / history->getWeight();

				sum += elapsed_time / ((base::DistributedProcessBase*)(history->getProcess()))->getResource();
				denominator++;
			}

			if (denominator == 0)
				return -1;
			else
				return sum / denominator;
		};

	public:
		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			// SHIFT TO PROCESSES
			auto process_map = ((base::DistributedSystemArrayBase*)system_array_)->getProcessMap();
			for (auto it = process_map.begin(); it != process_map.end(); it++)
				((base::DistributedProcessBase*)(it->second.get()))->replyData(invoke);

			// SHIFT TO MASTER AND SLAVES
			super::replyData(invoke);
		};

		virtual void _Send_back_history(std::shared_ptr<protocol::Invoke> invoke, std::shared_ptr<slave::InvokeHistory> $history)
		{
			std::shared_ptr<DSInvokeHistory> history = std::dynamic_pointer_cast<DSInvokeHistory>($history);
			if (history != nullptr)
			{
				// RE-SEND INVOKE MESSAGE TO ANOTHER SLAVE VIA ROLE
				((base::DistributedProcessBase*)(history->getProcess()))->sendData(invoke, history->getWeight());
			}

			// ERASE THE HISTORY
			super::_Send_back_history(invoke, history);
		};
		
	protected:
		virtual void _Report_history(std::shared_ptr<library::XML> xml) override
		{
			if (xml->hasProperty("_Piece_first") == true)
			{
				//--------
				// ParallelSystem's history -> PRInvokeHistory
				//--------
				super::_Report_history(xml);
			}
			else
			{
				library::UniqueWriteLock uk(system_array_->getMutex());

				//--------
				// DistributedProcess's history -> DSInvokeHistory
				//--------
				// CONSTRUCT HISTORY
				std::shared_ptr<DSInvokeHistory> history(new DSInvokeHistory(this));
				history->construct(xml);

				// IF THE HISTORY IS NOT EXIST IN PROGRESS, THEN TERMINATE REPORTING
				auto progress_it = _Get_progress_list().find(history->getUID());
				if (progress_it == _Get_progress_list().end())
					return;

				history->weight_ = std::dynamic_pointer_cast<DSInvokeHistory>(progress_it->second.second)->getWeight();

				// ERASE FROM ORDINARY PROGRESS AND MIGRATE TO THE HISTORY
				_Get_progress_list().erase(progress_it);
				_Get_history_list().emplace(history->getUID(), history);

				// ALSO NOTIFY TO THE ROLE
				if (history->getProcess() != nullptr)
					((base::DistributedProcessBase*)(history->getProcess()))->_Report_history(history);

				// COMPLETE THE HISTORY IN THE BELONGED SYSTEM_ARRAY
				((parallel::base::ParallelSystemArrayBase*)system_array_)->_Complete_history(history);
			}
		};
	};
};
};
};