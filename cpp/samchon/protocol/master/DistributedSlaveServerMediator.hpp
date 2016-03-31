#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSlaveSystemMediator.hpp>
#include <samchon/protocol/slave/SlaveServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A distributed slave server for mediation.
	 *
	 * @details 
	 * <p> DistributedSlaveServerMediator is a DistributedSlaveSystemMediator class 
	 * specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSlaveSystemMediator
	 */
	class SAMCHON_FRAMEWORK_API DistributedSlaveServerMediator
		: public virtual DistributedSlaveSystemMediator,
		public virtual slave::SlaveServer
	{
	private:
		typedef DistributedSlaveSystemMediator super;
		typedef slave::SlaveServer network_supr;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedSlaveServerMediator();
		virtual ~DistributedSlaveServerMediator() = default;
	};
};
};
};