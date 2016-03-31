#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	/**
	 *  @brief A slave system.
	 *
	 * @details
	 * <p> SlaveSystem, literally, means a slave system belongs to a maste system. </p>
	 *
	 * <p> The SlaveSystem class is used in opposite side system of master::DistributedSystem
	 * and master::ParallelSystem and reports elapsed time of each commmand (by Invoke message)
	 * for estimation of its performance. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::ExternalSystem
	 */
	class SAMCHON_FRAMEWORK_API SlaveSystem
		: public virtual ExternalSystem
	{
	protected:
		typedef ExternalSystem super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		SlaveSystem();
		virtual ~SlaveSystem() = default;

		//virtual void sendData(std::shared_ptr<Invoke>) override;

	protected:
		/**
		 * @brief Pre-processor for reporting elapsed time.
		 *
		 * \par [Inherited]
		 *		@copydetails IClient::_replyData()
		 */
		virtual void _replyData(std::shared_ptr<Invoke>) override;
	};
};
};
};