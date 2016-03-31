#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/PRInvokeHistory.hpp>

#include <atomic>
#include <vector>
#include <samchon/library/CriticalVector.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystemArray;
	class PRMasterHistoryArray;

	/**
	 * @brief A history log of an Invoke message on a master.
	 *
	 * @details
	 * \par [Inherited]
	 *		@copydetails protocol::InvokeHistory
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API PRMasterHistory
		: public PRInvokeHistory
	{
		friend class ParallelSystemArray;

	protected:
		typedef PRInvokeHistory super;

		/**
		 * @brief A master the history is belonged to.
		 */
		ParallelSystemArray *master;

		/**
		 * @brief An array of histories which are generated in each system.
		 */
		library::CriticalVector<PRInvokeHistory*> historyArray;

		/**
		 * @brief Completed count.
		 */
		std::atomic<size_t> completed;

	public:
		/**
		 * @brief Construct from master and invoke message.
		 *
		 * @param master An array of master history.
		 * @param invoke An invoke message to send.
		 */
		PRMasterHistory(PRMasterHistoryArray*, std::shared_ptr<Invoke>, size_t, size_t);
		virtual ~PRMasterHistory() = default;

		virtual void notifyEnd() override;
	};
};
};
};