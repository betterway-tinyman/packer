#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSlaveSystemMediator.hpp>
#include <samchon/protocol/slave/SlaveClient.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A distributed slave client for mediation.
	 *
	 * @details 
	 * <p> DistributedClientServerMediator is a DistributedSlaveSystemMediator class 
	 * specialized in client driver. </p>
	 *
	 * \par [Inherited]
	 *	   @copydetails master::DistributedSlaveSystemMediator
	 */
	class SAMCHON_FRAMEWORK_API DistributedClientServerMediator
		: public virtual DistributedSlaveSystemMediator,
		public virtual slave::SlaveClient
	{
	private:
		typedef DistributedSlaveSystemMediator super;
		typedef slave::SlaveClient network_supr;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedClientServerMediator();
		virtual ~DistributedClientServerMediator() = default;
	};
};
};
};