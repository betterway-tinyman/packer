#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/ExternalSystemArray.hpp>
#	include <samchon/templates/parallel/ParallelSystem.hpp>
#include <samchon/templates/parallel/base/ParallelSystemArrayBase.hpp>

namespace samchon
{
namespace templates
{

/**
 * A template for Parallel Processing System.
 * 
 * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/CPP-Templates-Parallel_System)
 * @author Jeongho Nam <http://samchon.org>
 */
namespace parallel
{
	/**
	 * Master of Parallel Processing System.
	 * 
	 * The {@link ParallelSystemArray} is an abstract class containing and managing remote parallel **slave** system 
	 * drivers, {@link ParallelSystem} objects. Within framework of network, {@link ParallelSystemArray} represents your 
	 * system, a **Master** of *Parallel Processing System* that requesting *parallel process* to **slave** systems and the 
	 * children {@link ParallelSystem} objects represent the remote **slave** systems, who is being requested the 
	 * *parallel processes*.
	 * 
	 * You can specify this {@link ParallelSystemArray} class to be *a server accepting parallel clients* or 
	 * *a client connecting to parallel servers*. Even both of them is possible. Extends one of them below and overrides 
	 * abstract factory method(s) creating the child {@link ParallelSystem} object.
	 * 
	 * - {@link ParallelClientArray}: A server accepting {@link ParallelSystem parallel clients}.
	 * - {@link ParallelServerArray}: A client connecting to {@link ParallelServer parallel servers}.
	 * - {@link ParallelServerClientArray}: Both of them. Accepts {@link ParallelSystem parallel clients} and connects to
	 *                                      {@link ParallelServer parallel servers} at the same time.
	 * 
	 * When you need the **parallel process**, then call one of them: {@link sendSegmentData} or {@link sendPieceData}.
	 * When the **parallel process** has completed, {@link ParallelSystemArray} estimates each {@link ParallelSystem}'s 
	 * {@link ParallelSystem.getPerformance performance index} basis on their execution time. Those performance indices 
	 * will be reflected to the next **parallel process**, how much pieces to allocate to each {@link ParallelSystem}.
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_parallel_system.png)
	 * 
	 * #### Proxy Pattern
	 * This class {@link ParallelSystemArray} is derived from the {@link ExternalSystemArray} class. Thus, you can take 
	 * advantage of the *Proxy Pattern* in the {@link ParallelSystemArray} class. If a process to request is not the 
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
	 * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/CPP-Templates-Parallel_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <class System = ParallelSystem>
	class ParallelSystemArray
		: public virtual external::ExternalSystemArray<System>,
		public base::ParallelSystemArrayBase
	{
	private:
		typedef external::ExternalSystemArray<System> super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystemArray()
			: super(),
			base::ParallelSystemArrayBase()
		{
		};
		virtual ~ParallelSystemArray() = default;

		/* =========================================================
			INVOKE MESSAGE CHAIN
				- SEND DATA
				- PERFORMANCE ESTIMATION
		============================================================
			SEND & REPLY DATA
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message with segment size.
		 * 
		 * Sends an {@link Invoke} message requesting a **parallel process** with its *segment size*. The {@link Invoke}
		 * message will be delivered to children {@link ParallelSystem} objects with the *piece size*, which is divided
		 * from the *segment size*, basis on their {@link ParallelSystem.getPerformance performance indices}.
		 * 
		 * - If segment size is 100,
		 * - The segment will be allocated such below:
		 * 
		 * Name    | Performance index | Number of pieces to be allocated | Formula
		 * --------|-------------------|----------------------------------|--------------
		 * Snail   |                 1 |                               10 | 100 / 10 * 1
		 * Cheetah |                 4 |                               40 | 100 / 10 * 4
		 * Rabbit  |                 3 |                               30 | 100 / 10 * 3
		 * Turtle  |                 2 |                               20 | 100 / 10 * 2
		 * 
		 * When the **parallel process** has completed, then this {@link ParallelSystemArraY} will estimate 
		 * {@link ParallelSystem.getPerformance performance indices} of {@link ParallelSystem} objects basis on their 
		 * execution time.
		 * 
		 * @param invoke An {@link Invoke} message requesting parallel process.
		 * @param size Number of pieces to segment.
		 * 
		 * @see {@link sendPieceData}, {@link ParallelSystem.getPerformacen}
		 */
		auto sendSegmentData(std::shared_ptr<protocol::Invoke> invoke, size_t size) -> size_t
		{
			return sendPieceData(invoke, 0, size);
		};

		/**
		 * Send an {@link Invoke} message with range of pieces.
		 * 
		 * Sends an {@link Invoke} message requesting a **parallel process** with its *range of pieces [first, last)*. 
		 * The {@link Invoke} will be delivered to children {@link ParallelSystem} objects with the newly computed 
		 * *range of sub-pieces*, which is divided from the *range of pieces (first to last)*, basis on their
		 * {@link ParallelSystem.getPerformance performance indices}.
		 * 
		 * - If indices of pieces are 0 to 50,
		 * - The sub-pieces will be allocated such below:
		 * 
		 * Name    | Performance index | Range of sub-pieces to be allocated | Formula
		 * --------|-------------------|-------------------------------------|------------------------
		 * Snail   |                 1 |                            ( 0,  5] | (50 - 0) / 10 * 1
		 * Cheetah |                 4 |                            ( 5, 25] | (50 - 0) / 10 * 4 + 5
		 * Rabbit  |                 3 |                            (25, 40] | (50 - 0) / 10 * 3 + 25
		 * Turtle  |                 2 |                            (40, 50] | (50 - 0) / 10 * 2 + 40
		 * 
		 * When the **parallel process** has completed, then this {@link ParallelSystemArraY} will estimate
		 * {@link ParallelSystem.getPerformance performance indices} of {@link ParallelSystem} objects basis on their
		 * execution time.
		 * 
		 * @param invoke An {@link Invoke} message requesting parallel process.
		 * @param first Initial piece's index in a section.
		 * @param last Final piece's index in a section. The range used is [*first*, *last*), which contains 
		 *			   all the pieces' indices between *first* and *last*, including the piece pointed by index
		 *			   *first*, but not the piece pointed by the index *last*.
		 * 
		 * @see {@link sendSegmentData}, {@link ParallelSystem.getPerformacen}
		 */
		virtual auto sendPieceData(std::shared_ptr<protocol::Invoke> invoke, size_t first, size_t last) -> size_t
		{
			library::UniqueWriteLock uk(getMutex());

			if (invoke->has("_History_uid") == false)
				invoke->emplace_back(new protocol::InvokeParameter("_History_uid", _Fetch_history_sequence()));
			else
			{
				// INVOKE MESSAGE ALREADY HAS ITS OWN UNIQUE ID
				//	- THIS IS A TYPE OF ParallelSystemArrayMediator. THE MESSAGE HAS COME FROM ITS MASTER
				//	- A ParallelSystem HAS DISCONNECTED. THE SYSTEM SHIFTED ITS CHAIN TO OTHER SLAVES.
				size_t uid = invoke->get("_History_uid")->getValue<size_t>();

				// FOR CASE 1. UPDATE HISTORY_SEQUENCE TO MAXIMUM
				if (uid > _Get_history_sequence())
					_Set_history_sequence(uid);
			}

			// TOTAL NUMBER OF PIECES TO DIVIDE
			size_t segment_size = last - first;

			// SYSTEMS TO BE GET DIVIDED PROCESSES AND
			std::vector<std::shared_ptr<ParallelSystem>> system_array;
			std::vector<std::thread> threads;
			
			system_array.reserve(size());
			threads.reserve(size());
			
			// POP EXCLUDEDS
			for (size_t i = 0; i < size(); i++)
			{
				std::shared_ptr<ParallelSystem> system = at(i);
				
				if (system->_Is_excluded() == false)
					system_array.push_back(system);
			}

			// ORDERS
			for (size_t i = 0; i < system_array.size(); i++)
			{
				std::shared_ptr<ParallelSystem> system = system_array[i];
				
				// COMPUTE FIRST AND LAST INDEX TO ALLOCATE
				size_t piece_size = (i == system_array.size() - 1)
					? segment_size - first
					: (size_t)(segment_size / system_array.size() * system->getPerformance());
				if (piece_size == 0)
					continue;

				std::shared_ptr<protocol::Invoke> my_invoke(new protocol::Invoke(invoke->getListener()));
				{
					// DUPLICATE INVOKE AND ATTACH PIECE INFO
					my_invoke->assign(invoke->begin(), invoke->end());
					my_invoke->emplace_back(new protocol::InvokeParameter("_Piece_first", first));
					my_invoke->emplace_back(new protocol::InvokeParameter("_Piece_last", last));
				};

				// ENROLL TO PROGRESS LIST
				std::shared_ptr<slave::InvokeHistory> history(new PRInvokeHistory(my_invoke));
				system->_Get_progress_list().emplace(history->getUID(), std::make_pair(invoke, history));

				// ENROLL THE SEND DATA INTO THREADS
				threads.emplace_back(&ParallelSystem::sendData, system.get(), my_invoke);
				first += piece_size; // FOR THE NEXT STEP
			}
			uk.unlock();

			// JOIN THREADS
			for (auto it = threads.begin(); it != threads.end(); it++)
				it->join();

			return threads.size();
		};

		/* ---------------------------------------------------------
			PERFORMANCE ESTIMATION - INTERNAL METHODS
		--------------------------------------------------------- */
		virtual auto _Complete_history(std::shared_ptr<slave::InvokeHistory> history) -> bool
		{
			// WRONG TYPE
			if (std::dynamic_pointer_cast<PRInvokeHistory>(history) == nullptr)
				return false;

			//========
			// READ LOCK
			//========
			library::UniqueWriteLock uk(getMutex());
			size_t uid = history->getUID();

			// ALL THE SUB-TASKS ARE DONE?
			for (size_t i = 0; i < size(); i++)
				if (at(i)->_Get_progress_list().has(uid) == true)
					return false; // IT'S ON A PROCESS IN SOME SYSTEM.

			//--------
			// RE-CALCULATE PERFORMANCE INDEX
			//--------
			// CONSTRUCT BASIC DATA
			std::vector<std::pair<std::shared_ptr<ParallelSystem>, double>> system_pairs;
			double performance_index_average = 0.0;

			system_pairs.reserve(size());

			for (size_t i = 0; i < size(); i++)
			{
				std::shared_ptr<ParallelSystem> system = at(i);
				if (system->_Get_history_list().has(uid) == false)
					continue; // NO HISTORY (HAVE NOT PARTICIPATED IN THE PARALLEL PROCESS)

				// COMPUTE PERFORMANCE INDEX BASIS ON EXECUTION TIME OF THIS PARALLEL PROCESS
				std::shared_ptr<PRInvokeHistory> my_history = std::dynamic_pointer_cast<PRInvokeHistory>(system->_Get_history_list().get(uid));
				double performance_index = my_history->computeSize() / (double)my_history->computeElapsedTime();

				// PUSH TO SYSTEM PAIRS AND ADD TO AVERAGE
				system_pairs.emplace_back(system, performance_index);
				performance_index_average += performance_index;
			}
			performance_index_average /= system_pairs.size();

			// RE-CALCULATE PERFORMANCE INDEX
			for (size_t i = 0; i < system_pairs.size(); i++)
			{
				// SYSTEM AND NEW PERFORMANCE INDEX BASIS ON THE EXECUTION TIME
				auto system = system_pairs[i].first;
				if (system->_Is_enforced() == true)
					continue; // PERFORMANCE INDEX IS ENFORCED. DOES NOT PERMIT REVALUATION

				double new_performance = system_pairs[i].second / performance_index_average;

				// DEDUCT RATIO TO REFLECT THE NEW PERFORMANCE INDEX
				double ordinary_ratio;
				if (system->_Get_history_list().size() < 2)
					ordinary_ratio = .3;
				else
					ordinary_ratio = std::min(0.7, 1.0 / (system->_Get_history_list().size() - 1.0));

				system->setPerformance((system->getPerformance() * ordinary_ratio) + (new_performance * (1 - ordinary_ratio)));
			}

			// AT LAST, NORMALIZE PERFORMANCE INDEXES OF ALL SLAVE SYSTEMS
			_Normalize_performance();
			return true;
		};

	protected:
		virtual void _Normalize_performance()
		{
			// COMPUTE AVERAGE
			double average = 0.0;
			size_t denominator = 0;

			for (size_t i = 0; i < size(); i++)
			{
				std::shared_ptr<ParallelSystem> system = at(i);
				if (system->_Is_enforced() == true)
					continue; // PERFORMANCE INDEX IS ENFORCED. DOES NOT PERMIT REVALUATION

				average += system->getPerformance();
				denominator++;
			}
			average /= (double)denominator;

			// DIVIDE FROM THE AVERAGE
			for (size_t i = 0; i < size(); i++)
			{
				std::shared_ptr<ParallelSystem> system = at(i);
				if (system->_Is_enforced() == true)
					continue; // PERFORMANCE INDEX IS ENFORCED. DOES NOT PERMIT REVALUATION

				system->setPerformance(system->getPerformance() / average);
			}
		};
	};
};
};
};