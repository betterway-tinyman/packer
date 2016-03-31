#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystem.hpp>
#include <mutex>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystemArray;

	class PRInvokeHistoryArray;
	class PRMasterHistory;

	/**
	 * @brief A network driver for a parallel system.
	 *
	 * @details
	 * <p> ParallelSystem class is an ExternalSystem having performance index and history logs
	 * of handled Invoke messages with those elapsed times. Those performance index and history
	 * logs are used to determine how much segmentation to be allocated. </p>
	 * 
	 * <p> Each segmentation in requested process (Invoke message) is equivalent. Thus, role of
	 * ParallelSystem objects in a ParallelSystemArray are almost same and does not need to specify
	 * ExternalSystemRole on ParallelSystem. </p>
	 *
	 *	\li Not a matter to specifying ExternalSystemRole objects to each ParallelSystem. In that
	 *		case, Invoke messages having segmentation size will be processed by ParallelSystem's own
	 *		logic and Invoke messages without segmentation size will be handled by ordinary logic of
	 *		ExternalSystem's own.
	 *
	 * <p> @image html  cpp/protocol_master_parallel_system.png
	 *	   @image latex cpp/protocol_master_parallel_system.png </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ExternalSystem
	 */
	class SAMCHON_FRAMEWORK_API ParallelSystem
		: public virtual ExternalSystem
	{
		friend class ParallelSystemArray;
		friend class PRMasterHistory;

	protected:
		typedef ExternalSystem super;

		/**
		 * @brief A master that the system is belonged to.
		 */
		ParallelSystemArray *systemArray;

		/**
		 * @brief A list of history log for reported Invoke messages.
		 */
		PRInvokeHistoryArray *historyArray;

		/**
		 * @brief A list of invoke messages on progress.
		 */
		PRInvokeHistoryArray *progressArray;

		/**
		 * @brief A performance index.
		 *
		 * <p> A performance index z is calculated by normalization calculating reverse number of
		 * @details
		 * whole parallel system's average elapsed times and its z value between the normal
		 * distribution. </p>
		 *
		 *	\li X = (¥ì + z¥ò)
		 *
		 * <p> If a parallel system has no history of handling Invoke message, then set the
		 * performance index to 0 as default. The performance index will be re-calcuated whenever
		 * segmented process has sent (ParallelSystemArray::sendData(Invoke, size_t)). </p>
		 */
		double performance;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystem();
		virtual ~ParallelSystem();

		virtual void construct(std::shared_ptr<library::XML>) override;


	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->ExternalSystemRole* override;

		/* ------------------------------------------------------------------
			CHAIN OF INVOKE MESSAGE
		------------------------------------------------------------------ */
		virtual void _replyData(std::shared_ptr<Invoke>) override;

	private:
		/**
		 * @brief Send a message with segmentation index.
		 *
		 * @param invoke An invoke message requesting a process.
		 * @param startIndex Starting index number of segmentation.
		 * @param size Size of segmentation.
		 */
		void sendPieceData(PRMasterHistory*, std::shared_ptr<Invoke>, size_t, size_t);

		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
	public:
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};