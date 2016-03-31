#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystemArray.hpp>

#include <atomic>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystem;

	class PRMasterHistoryArray;
	class PRMasterHistory;

	/**
	 * @brief An array of parallel system drivers.
	 *
	 * @details
	 * <p> ParallelSystemArray is an ExternalSystemArray and an abstract class containing.
	 * and managing parallel system drvers within framework of master. The ParallelSystemArray
	 * class allocates segmented processes (Invoke message containing segmentation size) following
	 * by each system's performance index. </p>
	 *
	 * <p> Unlike DistributedSystemArray, ParallelSystemArray does not have such complicate relationships
	 * and logics. All segmentation sent by sendData(Invoke, size_t) requires uniform performance.
	 * ParallelSystemArray just calculates each system's performance index by elapsing time such easily. </p>
	 *
	 * <p> Of course, DistributedSystemArray determines which size of segmentation allocation will be
	 * suitable for each system by the performance index </p>.
	 *
	 * <p> Each segmentation in requested process (Invoke message) is equivalent. Thus, role of
	 * ParallelSystem objects in a ParallelSystemArray are almost same and does not need to specify
	 * ExternalSystemRole on each ParallelSystem. </p>
	 *
	 *	\li Not a matter to specifying ExternalSystemRole objects to each ParallelSystem. In that
	 *		case, Invoke messages having segmentation size will be processed by ParallelSystemArray's
	 *		own logic and Invoke messages without segmentation size will be handled by ordinary
	 *		logic of ExternalSystemArray's own.
	 *
	 * <p> @image html  cpp/protocol_master_parallel_system.png
	 *	   @image latex cpp/protocol_master_parallel_system.png </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::ExternalSystemArray
	 */
	class SAMCHON_FRAMEWORK_API ParallelSystemArray
		: public virtual ExternalSystemArray
	{
		friend class ParallelSystem;
		friend class PRMasterHistory;

	private:
		typedef ExternalSystemArray super;

		/**
		 * @brief Sequence of uid allocating for history.
		 */
		std::atomic<size_t> uid;

		/**
		 * @brief An array of history have occured.
		 */
		PRMasterHistoryArray *historyArray;

		/**
		 * @brief An array of history on progress.
		 */
		PRMasterHistoryArray *progressArray;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystemArray();
		virtual ~ParallelSystemArray();

		/* ------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------ */
		SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(ParallelSystem)

			/* ------------------------------------------------------------------
				CHAIN OF INVOKE MESSAGE
			------------------------------------------------------------------ */
			using super::sendData;

		/**
		 * @brief Send a message with segmentation size.
		 *
		 * @param invoke An invoke message requesting a process.
		 * @param size Size of segmentation.
		 */
		void sendSegmentData(std::shared_ptr<Invoke>, size_t);

		/**
		 * @brief Send a message with piece index and size.
		 *
		 * @param invoke An invoke message requesting a process.
		 * @param index Starting index number of segmentation.
		 * @param size Size of segmentation.
		 */
		virtual void sendPieceData(std::shared_ptr<Invoke>, size_t, size_t);

	protected:
		/**
		 * @brief Notify end of a master history.
		 *
		 * @details
		 * <p> Estimates performance of each system by a master history containing their elapsed times. </p>
		 *
		 * <p> The estimation is done by normalizing performances of children systems. The estimation of
		 * performance is divided by averagae elapsed time of each segmentation.
		 */
		virtual void notifyEnd(PRMasterHistory*);
	};
};
};
};