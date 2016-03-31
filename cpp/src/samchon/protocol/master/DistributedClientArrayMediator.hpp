#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSystemArrayMediator.hpp>
#include <samchon/protocol/master/DistributedClientArray.hpp>

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
	 * <p> DistributedClientArrayMediator is a DistributedSystemArrayMediator specialized in 
	 * clients' driver. DistributedClientArrayMediator is a client for its master and also 
	 * manages children clients' slaves. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystemArrayMediator
	 */
	class SAMCHON_FRAMEWORK_API DistributedClientArrayMediator
		: public virtual DistributedSystemArrayMediator,
		public virtual DistributedClientArray
	{
	private:
		typedef DistributedSystemArrayMediator super;
		typedef DistributedClientArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedClientArrayMediator();
		virtual ~DistributedClientArrayMediator() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void start() override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};
