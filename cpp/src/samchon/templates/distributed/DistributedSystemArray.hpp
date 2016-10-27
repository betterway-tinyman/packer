#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystemArray.hpp>
#	include <samchon/templates/distributed/DistributedSystem.hpp>
#	include <samchon/templates/distributed/DistributedProcess.hpp>
#include <samchon/templates/distributed/base/DistributedSystemArrayBase.hpp>

namespace samchon
{
namespace templates
{

/**
 * A template for Distributed Processing System.
 * 
 * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/CPP-Templates-Distributed_System)
 * @author Jeongho Nam <http://samchon.org>
 */
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
	*   - {@link MediatorSharedWorkerClient}
	* - A server slave accepting master client:
	*   - {@link MediatorServer}
	*   - {@link MediatorWebServer}
	*   - {@link MediatorSharedWorkerServer}
	*
	* #### [Inherited] {@link DistributedSystemArray}
	* The {@link DistributedSystemArray} is an abstract class containing and managing remote distributed **slave** system
	* drivers, {@link DistributedSystem} objects. Within framework of network, {@link DistributedSystemArray} represents
	* your system, a **Master** of *Distributed Processing System* that requesting *distributed process* to **slave**
	* systems and the children {@link DistributedSystem} objects represent the remote **slave** systems, who is being
	* requested the *distributed processes*.
	*
	* The {@link DistributedSystemArray} contains {@link DistributedProcess} objects directly. You can request a
	* **distributed process** through the {@link DistributedProcess} object. You can access the
	* {@link DistributedProcess} object(s) with those methods:
	*
	* - {@link hasRole}
	* - {@link getRole}
	* - {@link insertRole}
	* - {@link eraseRole}
	* - {@link getRoleMap}
	*
	* When you need the **distributed process**, call the {@link DistributedProcess.sendData} method. Then the
	* {@link DistributedProcess} will find the most idle {@link DistributedSystem} object who represents a distributed
	* **slave **system. The {@link Invoke} message will be sent to the most idle {@link DistributedSystem} object. When
	* the **distributed process** has completed, then {@link DistributedSystem.getPerformance performance index} and
	* {@link DistributedProcess.getResource resource index} of related objects will be revaluated.
	*
	* ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_distributed_system.png)
	*
	* #### Parallel Process
	* This {@link DistributedSystemArray} class is derived from the {@link ParallelSystemArray} class, so you can request
	* a **parallel process**, too.
	*
	* When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
	* When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s
	* {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices will
	* be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
	*
	* #### Proxy Pattern
	* This class {@link DistributedSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take
	* advantage of the *Proxy Pattern* in the {@link DistributedSystemArray} class. If a process to request is not the
	* *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
	* may better to utilizing the *Proxy Pattern*:
	*
	* The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
	* {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
	* important. Only interested in user's perspective is *which can be done*.
	*
	* By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
	* to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
	* Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
	*
	* <ul>
	*	<li>
	*		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
	*		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
	*	</li>
	*	<li>
	*		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
	*		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
	*		external system.
	*	</li>
	*	<li> Those strategy is called *Proxy Pattern*. </li>
	* </ul>
	*
	* @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/CPP-Templates-Distributed_System)
	* @author Jeongho Nam <http://samchon.org>
	*/
	template <class System = DistributedSystem>
	class DistributedSystemArray
		: public virtual parallel::ParallelSystemArray<System>,
		public base::DistributedSystemArrayBase
	{
	private:
		typedef parallel::ParallelSystemArray<System> super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		DistributedSystemArray()
			: super()
		{
		};
		virtual ~DistributedSystemArray() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			//--------
			// CONSTRUCT ROLES
			//--------
			// CLEAR ORDINARY ROLES
			process_map_.clear();

			// CREATE ROLES
			if (xml->has("processes") == true && xml->get("processes")->front()->has("process") == true)
			{
				std::shared_ptr<library::XMLList> &role_xml_list = xml->get("processes")->front()->get("process");
				for (size_t i = 0; i < role_xml_list->size(); i++)
				{
					std::shared_ptr<library::XML> &role_xml = role_xml_list->at(i);

					// CONSTRUCT ROLE FROM XML
					std::shared_ptr<DistributedProcess> role(createProcess(role_xml));
					role->construct(role_xml);

					// AND INSERT TO ROLE_MAP
					insertProcess(role);
				}
			}

			//--------
			// CONSTRUCT SYSTEMS
			//--------
			super::construct(xml);
		};

	protected:
		/**
		 * Factory method creating a child {@link DistributedProcess process} object.
		 * 
		 * @param xml {@link XML} represents the {@link DistributedProcess child} object.
		 * @return A new {@link DistributedProcess} object.
		 */
		virtual auto createProcess(std::shared_ptr<library::XML>) -> DistributedProcess* = 0;

		/* ---------------------------------------------------------
			HISTORY HANDLER - PERFORMANCE ESTIMATION
		--------------------------------------------------------- */
		virtual auto _Complete_history(std::shared_ptr<protocol::InvokeHistory> $history) -> bool override
		{
			std::shared_ptr<DSInvokeHistory> history = std::dynamic_pointer_cast<DSInvokeHistory>($history);

			// ParallelSystem's history -> PRInvokeHistory
			if (history == nullptr)
				return super::_Complete_history($history);

			//--------
			// DistributedProcess's history -> DSInvokeHistory
			//--------
			// NO ROLE, THEN FAILED TO COMPLETE
			if (history->getProcess() == nullptr)
				return false;

			// ESTIMATE PERFORMANCE INDEXES
			estimate_system_performance(history); // ESTIMATE SYSTEMS' INDEX
			estimate_process_resource(history); // ESTIMATE PROCESS' PERFORMANCE

			// AT LAST, NORMALIZE PERFORMANCE INDEXES OF ALL SYSTEMS AND ROLES
			_Normalize_performance();
			return true;
		};

		virtual void _Normalize_performance() override
		{
			// NORMALIZE SYSTEMS' PERFORMANCE INDEXES
			super::_Normalize_performance();

			// NORMALIZE ROLES' PERFORMANCE INDEXES
			double average = 0;
			size_t denominator = 0;

			for (auto it = process_map_.begin(); it != process_map_.end(); it++)
			{
				auto process = it->second;
				if (process->isEnforced() == true)
					continue; // THE RESOURCE INDEX IS ENFORCED. DO NOT PERMIT REVALUATION

				average += process->getResource();
				denominator++;
			}
			average /= (double)denominator;

			// DIVIDE FROM THE AVERAGE
			for (auto it = process_map_.begin(); it != process_map_.end(); it++)
			{
				auto process = it->second;
				if (process->isEnforced() == true)
					continue; // THE RESOURCE INDEX IS ENFORCED. DO NOT PERMIT REVALUATION

				process->setResource(process->getResource() / average);
			}
		};

	private:
		void estimate_process_resource(std::shared_ptr<DSInvokeHistory> history)
		{
			DistributedProcess *process = history->getProcess();
			if (process->isEnforced() == true)
				return; // THE RESOURCE INDEX IS ENFORCED. DO NOT PERMIT REVALUATION

			double average_elapsed_time_of_others = 0;
			size_t denominator = 0;

			// COMPUTE AVERAGE ELAPSED TIME
			for (auto it = process_map_.begin(); it != process_map_.end(); it++)
			{
				DistributedProcess *my_process = it->second.get();
				if (my_process == process || my_process->_Get_history_list()->empty() == true)
					continue;

				average_elapsed_time_of_others += my_process->_Compute_average_elapsed_time() * my_process->getResource();
				denominator++;
			}

			// COMPARE WITH THIS HISTORY'S ELAPSED TIME
			if (denominator != 0)
			{
				// DIVE WITH DENOMINATOR
				average_elapsed_time_of_others /= (double)denominator;

				// DEDUCT NEW PERFORMANCE INDEX BASED ON THE EXECUTION TIME
				//	- ROLE'S PERFORMANCE MEANS; HOW MUCH TIME THE ROLE NEEDS
				//	- ELAPSED TIME IS LONGER, THEN PERFORMANCE IS HIGHER
				double elapsed_time = history->computeElapsedTime() / history->getWeight(); // CONSIDER WEIGHT
				double new_performance = elapsed_time / average_elapsed_time_of_others; // THE NEW RESOURCE

				// DEDUCT RATIO TO REFLECT THE NEW PERFORMANCE INDEX -> MAXIMUM: 15%
				double ordinary_ratio;
				if (process->_Get_history_list()->size() < 2)
					ordinary_ratio = .15;
				else
					ordinary_ratio = min(.85, 1.0 / (process->_Get_history_list()->size() - 1.0));

				// DEFINE NEW PERFORMANCE
				process->setResource
				(
					(process->getResource() * ordinary_ratio)
					+ (new_performance * (1 - ordinary_ratio))
				);
			}
		};

		void estimate_system_performance(std::shared_ptr<DSInvokeHistory> history)
		{
			DistributedSystem *system = history->getSystem();
			if (system->isEnforced() == true)
				return; // THE PERFORMANCE INDEX IS ENFORCED. IT DOESN'T PERMIT REVALUATION

			double average_elapsed_time_of_others = 0;
			size_t denominator = 0;

			// COMPUTE AVERAGE ELAPSED TIME
			for (size_t i = 0; i < this->size(); i++)
			{
				shared_ptr<DistributedSystem> system = this->at(i);

				double avg = system->_Compute_average_elapsed_time();
				if (avg == -1)
					continue;

				average_elapsed_time_of_others += avg;
				denominator++;
			}

			// COMPARE WITH THIS HISTORY'S ELAPSED TIME
			if (denominator != 0)
			{
				// DIVE WITH DENOMINATOR
				average_elapsed_time_of_others /= denominator;

				// DEDUCT NEW PERFORMANCE INDEX BASED ON THE EXECUTION TIME
				//	- SYSTEM'S PERFORMANCE MEANS; HOW FAST THE SYSTEM IS
				//	- ELAPSED TIME IS LOWER, THEN PERFORMANCE IS HIGHER
				double elapsed_time = history->computeElapsedTime() / history->getWeight();
				double new_performance = average_elapsed_time_of_others / elapsed_time;

				// DEDUCT RATIO TO REFLECT THE NEW PERFORMANCE INDEX -> MAXIMUM: 30%
				double ordinary_ratio;
				if (system->_Get_history_list()->size() < 2)
					ordinary_ratio = .3;
				else
					ordinary_ratio = min(0.7, 1.0 / (system->_Get_history_list()->size() - 1.0));

				// DEFINE NEW PERFORMANCE
				system->setPerformance
				(
					(system->getPerformance() * ordinary_ratio)
					+ (new_performance * (1 - ordinary_ratio))
				);
			}
		};

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> &xml = super::toXML();
			if (process_map_.empty() == true)
				return xml;

			std::shared_ptr<library::XML> processes_xml(new library::XML());
			{
				processes_xml->setTag("processes");
				for (auto it = process_map_.begin(); it != process_map_.end(); it++)
					processes_xml->push_back(it->second->toXML());
			}
			xml->push_back(processes_xml);
			return xml;
		};
	};
};
};
};