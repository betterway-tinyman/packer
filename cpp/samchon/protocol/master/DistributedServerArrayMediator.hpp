#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSystemArrayMediator.hpp>
#include <samchon/protocol/master/DistributedServerArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A mediator of distributed system between master and slaves.
	 *
	 * @details
	 * <p> DistributedServerArrayMediator is a DistributedSystemArrayMediator specialized in
	 * servers' driver. DistributedServerArrayMediator is a server for its master and also
	 * manages children servers' slaves. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystemArrayMediator
	 */
	class SAMCHON_FRAMEWORK_API DistributedServerArrayMediator
		: public virtual DistributedSystemArrayMediator,
		public virtual DistributedServerArray
	{
	private:
		typedef DistributedSystemArrayMediator super;
		typedef DistributedServerArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedServerArrayMediator();
		virtual ~DistributedServerArrayMediator() = default;

		virtual void start() override;
	};
};
};
};
