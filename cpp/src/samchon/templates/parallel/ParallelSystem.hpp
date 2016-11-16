#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/ExternalSystem.hpp>
#include <samchon/protocol/IListener.hpp>

#include <samchon/templates/parallel/base/ParallelSystemArrayBase.hpp>

#include <thread>
#include <samchon/HashMap.hpp>
#include <samchon/templates/parallel/PRInvokeHistory.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * A driver for a parallel slave system.
	 * 
	 * The {@link ParallelSystem} is an abstract class represents a **slave** system in *Parallel Processing System*, 
	 * connected with this **master** system. This {@link ParallelSystem} takes full charge of network communication with 
	 * the remote, parallel **slave** system has connected.
	 * 
	 * When a *parallel process* is requested (by {@link ParallelSystemArray.sendSegementData} or 
	 * {@link ParallelSystemArray.sendPieceData}), the number of pieces to be allocated to a {@link ParallelSystem} is 
	 * turn on its {@link getPerformance performance index}. Higher {@link getPerformance performance index}, then 
	 * more pieces are requested. The {@link getPerformance performance index} is revaluated whenever a *parallel process* 
	 * has completed, basic on the execution time and number of pieces. You can sugguest or enforce the 
	 * {@link getPerformance performance index} with {@link setPerformance} or {@link enforcePerformance}.
	 * 
	 * <a href="http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_parallel_system.png"
	 *		  target="_blank">
	 *	<img src="http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_parallel_system.png"
	 *		 style="max-width: 100%" />
	 * </a>
	 *
	 * #### Bridge & Proxy Pattern
	 * This class {@link ParallelSystem} is derived from the {@link ExternalSystem} class. Thus, you can take advantage 
	 * of the *Bridge & Proxy Pattern* in this {@link ParallelSystem} class. If a process to request is not the
	 * *parallel process* (to be distrubted to all slaves), but the **exclusive process** handled in a system, then it
	 * may better to utilizing the *Bridge & Proxy Pattern*:
	 * 
	 * The {@link ExternalSystem} class can be a *bridge* for *logical proxy*. In framework within user,
	 * which {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
	 * important. Only interested in user's perspective is *which can be done*.
	 *
	 * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
	 * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
	 * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_parallel_system.png)
	 * 
	 * @handbook [Templates - Parallel System](https://github.com/samchon/framework/wiki/CPP-Templates-Parallel_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class ParallelSystem
		: public virtual external::ExternalSystem,
		public virtual protocol::IListener
	{
	private:
		typedef external::ExternalSystem super;

		HashMap<size_t, std::pair<std::shared_ptr<protocol::Invoke>, std::shared_ptr<slave::InvokeHistory>>> progress_list_;
		HashMap<size_t, std::shared_ptr<slave::InvokeHistory>> history_list_;

		double performance_{ 1.0 };
		bool enforced_{ false };
		bool excluded_{ false };

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		// TAKES SUPER'S CONSTRUCTORS
		using super::super;

		/**
		 * Default Destructor.
		 * 
		 * The {@link ParallelSystem} object is destructed when connection with the remote system is closed or this
		 * {@link ParallelSystem} object is {@link ParallelSystemArray.erase erased} from its parent 
		 * {@link ParallelSystemArray} object.
		 * 
		 * You may think if there're some *parallel processes* have requested but not completed yet, then it would be a
		 * critical problem because the *parallel processes* will not complete forever. Do not worry. The critical problem
		 * does not happen. After the destruction, the remained *parallel processes* will be shifted to and proceeded in 
		 * other {@link ParallelSystem} objects.
		 */
		virtual ~ParallelSystem()
		{
			excluded_ = true;

			// SHIFT PARALLEL INVOKE MESSAGES HAD PROGRESSED TO OTHER SLAVES
			for (auto it = progress_list_.begin(); it != progress_list_.end(); it++)
			{
				// INVOKE MESSAGE AND ITS HISTORY ON PROGRESS
				std::shared_ptr<protocol::Invoke> invoke = it->second.first;
				std::shared_ptr<slave::InvokeHistory> history = it->second.second;

				// SEND THEM BACK
				_Send_back_history(invoke, history);
			}
		};

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			super::construct(xml);

			performance_ = xml->getProperty<double>("performance");
			enforced_ = xml->getProperty<bool>("enforced");
		};

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get performance index.
		 *
		 * Get *performance index* that indicates how much fast the remote system is.
		 *
		 * If this {@link ParallelSystem parallel system} does not have any {@link Invoke} message had handled, then the
		 * *performance index* will be ```1.0```, which means default and average value between all {@link ParallelSystem}
		 * instances (that are belonged to a same {@link ParallelSystemArray} object).
		 *
		 * You can specify this *performance index* by yourself but notice that, if the *performance index* is higher
		 * than other {@link ParallelSystem} objects, then this {@link ParallelSystem parallel system} will be ordered to
		 * handle more processes than other {@link ParallelSystem} objects. Otherwise, the *performance index* is lower
		 * than others, of course, less processes will be delivered.
		 *
		 * - {@link setPerformance setPerformance()}
		 * - {@link enforcePerformance enforcePerformance()}
		 *
		 * Unless {@link enforcePerformance enforcePerformance()} is called, This *performance index* is **revaluated**
		 * whenever user calls one of them below.
		 *
		 * - {@link ParallelSystemArray.sendSegmentData ParallelSystemArray.sendSegmentData()}
		 * - {@link ParallelSystemArray.sendPieceData ParallelSystemArray.sendPieceData()}
		 * - {@link DistributedProcess.sendData DistributedProcess.sendData()}.
		 *
		 * @return Performance index.
		 */
		auto getPerformance() const -> double
		{
			return performance_;
		};

		/**
		 * Set performance index.
		 *
		 * Set *performance index* that indicates how much fast the remote system is. This *performance index* can be
		 * **revaulated**.
		 *
		 * Note that, initial and average *performance index* of {@link ParallelSystem} objects are ```1.0```. If the
		 * *performance index* is higher than other {@link ParallelSystem} objects, then this {@link ParallelSystem} will
		 * be ordered to handle more processes than other {@link ParallelSystem} objects. Otherwise, the
		 * *performance index* is lower than others, of course, less processes will be delivered.
		 *
		 * Unlike {@link enforcePerformance}, configuring *performance index* by this {@link setPerformance} allows
		 * **revaluation**. This **revaluation** prevents wrong valuation from user. For example, you *mis-valuated* the
		 * *performance index*. The remote system is much faster than any other, but you estimated it to the slowest one.
		 * It looks like a terrible case that causes {@link ParallelSystemArray entire parallel systems} to be slower,
		 * however, don't mind. The system will direct to the *propriate performance index* eventually with the
		 * **revaluation** by following methods.
		 *
		 * - {@link ParallelSystemArray.sendSegmentData ParallelSystemArray.sendSegmentData()}
		 * - {@link ParallelSystemArray.sendPieceData ParallelSystemArray.sendPieceData()}
		 * - {@link DistributedProcess.sendData DistributedProcess.sendData()}.
		 *
		 * @param val New performance index, but can be revaluated.
		 */
		void setPerformance(double val)
		{
			performance_ = val;
			enforced_ = false;
		};

		/**
		 * Enforce performance index.
		 *
		 * Enforce *performance index* that indicates how much fast the remote system is. The *performance index* will be
		 * fixed, never be **revaluated**.
		 *
		 * Note that, initial and average *performance index* of {@link ParallelSystem} objects are ```1.0```. If the
		 * *performance index* is higher than other {@link ParallelSystem} objects, then this {@link ParallelSystem} will
		 * be ordered to handle more processes than other {@link ParallelSystem} objects. Otherwise, the
		 * *performance index* is lower than others, of course, less processes will be delivered.
		 *
		 * The difference between {@link setPerformance} and this {@link enforcePerformance} is allowing **revaluation**
		 * or not. This {@link enforcePerformance} does not allow the **revaluation**. The *performance index* is clearly
		 * fixed and never be changed by the **revaluation**. But you've to keep in mind that, you can't avoid the
		 * **mis-valuation** with this {@link enforcePerformance}.
		 *
		 * For example, there's a remote system much faster than any other, but you **mis-estimated** it to the slowest.
		 * In that case, there's no way. The {@link ParallelSystemArray entire parallel systems} will be slower by the
		 * **mis-valuation**. By the reason, using {@link enforcePerformance}, it's recommended only when you can clearly
		 * certain the *performance index*. If you can't certain the *performance index* but want to recommend, then use
		 * {@link setPerformance} instead.
		 *
		 * @param val New performance index to be fixed.
		 */
		void enforcePerformance(double val)
		{
			performance_ = val;
			enforced_ = true;
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN - PERFORMANCE ESTIMATION
		--------------------------------------------------------- */
	protected:
		virtual void _Reply_data(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "_Report_history")
				_Report_history(invoke->front()->getValueAsXML());
			else if (invoke->getListener() == "_Send_back_history")
			{
				size_t uid = invoke->front()->getValue<size_t>();
				auto it = progress_list_.find(uid);

				if (it != progress_list_.end())
					_Send_back_history(it->second.first, it->second.second);
			}
			else
				replyData(invoke);
		};

		virtual void _Report_history(std::shared_ptr<library::XML> xml)
		{
			library::UniqueWriteLock uk(system_array_->getMutex());

			//--------
			// CONSTRUCT HISTORY
			//--------
			std::shared_ptr<PRInvokeHistory> history(new PRInvokeHistory());
			history->construct(xml);

			// IF THE HISTORY IS NOT EXIST IN PROGRESS, THEN TERMINATE REPORTING
			auto progress_it = progress_list_.find(history->getUID());
			if (progress_it == progress_list_.end())
				return;

			// ARCHIVE FIRST AND LAST INDEX
			history->first_ = std::dynamic_pointer_cast<PRInvokeHistory>(progress_it->second.second)->getFirst();
			history->last_ = std::dynamic_pointer_cast<PRInvokeHistory>(progress_it->second.second)->getLast();

			// ERASE FROM ORDINARY PROGRESS AND MIGRATE TO THE HISTORY
			progress_list_.erase(progress_it);
			history_list_.insert({ history->getUID(), history });

			// NOTIFY TO THE MANAGER, SYSTEM_ARRAY
			((base::ParallelSystemArrayBase*)system_array_)->_Complete_history(history);
		};

		virtual void _Send_back_history(std::shared_ptr<protocol::Invoke> invoke, std::shared_ptr<slave::InvokeHistory> $history)
		{
			std::shared_ptr<PRInvokeHistory> history = std::dynamic_pointer_cast<PRInvokeHistory>($history);
			if (history == nullptr)
				return;

			// REMOVE UID AND FIRST, LAST INDEXES
			for (size_t i = invoke->size(); i < invoke->size(); i--)
			{
				const std::string &name = invoke->at(i)->getName();

				if (name == "_History_uid" || name == "_Piece_first" || name == "_Piece_last")
					invoke->erase(invoke->begin() + i);
			}

			// RE-SEND (DISTRIBUTE) THE PIECE TO OTHER SLAVES
			std::thread
			(
				&base::ParallelSystemArrayBase::sendPieceData, (base::ParallelSystemArrayBase*)system_array_,
				invoke, history->getFirst(), history->getLast()
			).detach();

			// ERASE FROM THE PROGRESS LIST
			progress_list_.erase(history->getUID());
		};

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> xml = super::toXML();
			xml->setProperty("performance", performance_);
			xml->setProperty("enforced", enforced_);

			return xml;
		};

		/* ---------------------------------------------------------
			INTERNAL ACCESSORS
		--------------------------------------------------------- */
		auto _Get_progress_list() -> HashMap<size_t, std::pair<std::shared_ptr<protocol::Invoke>, std::shared_ptr<slave::InvokeHistory>>>& { return progress_list_; };
		auto _Get_progress_list() const -> const HashMap<size_t, std::pair<std::shared_ptr<protocol::Invoke>, std::shared_ptr<slave::InvokeHistory>>>& { return progress_list_; };
		
		auto _Get_history_list() -> HashMap<size_t, std::shared_ptr<slave::InvokeHistory>>& { return history_list_; };
		auto _Get_history_list() const -> const HashMap<size_t, std::shared_ptr<slave::InvokeHistory>>& { return history_list_; };

		auto _Is_enforced() const -> bool
		{
			return enforced_;
		};
		auto _Is_excluded() const -> bool
		{
			return excluded_;
		};
		void _Set_excluded()
		{
			excluded_ = true;
		};
	};
};
};
};