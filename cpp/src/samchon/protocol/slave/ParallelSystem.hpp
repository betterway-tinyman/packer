#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	/**
	 * @brief A slave parallel system.
	 *
	 * @details
	 * <p> ParallelSystem is a class processing segmented Invoke messages from master. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails slave::SlaveSystem
	 */
	class SAMCHON_FRAMEWORK_API ParallelSystem
		: public virtual SlaveSystem
	{
	protected:
		typedef SlaveSystem super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystem();
		virtual ~ParallelSystem() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->ExternalSystemRole* override;

		/**
		* @brief Pre-processor for segmented processes and reporting elapsed time.
		*
		* \par [Inherited]
		*	   @copydetails IClient::_replyData()
		*/
		virtual void _replyData(std::shared_ptr<Invoke>) override;

	public:
		/**
		 * @brief Handle requested processes which are segmented.
		 *
		 * @param invoke An invoke message containing segmented process requestments.
		 * @param startIndex Starting index number of segmentation.
		 * @param size Size of segmentation.
		 */
		virtual void replyPieceData(std::shared_ptr<Invoke>, size_t, size_t) = 0;
	};
};
};
};