#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArrayMediator.hpp>
#include <samchon/protocol/master/ParallelClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A mediator of parallel system between master and slaves.
	 *
	 * @details
	 * <p> ParallelClientArrayMediator is a ParallelSystemArrayMediator specialized in 
	 * clients' driver. ParallelClientArrayMediator is a client for its master and also 
	 * manages children clients' slaves. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ParallelSystemArrayMediator
	 */
	class SAMCHON_FRAMEWORK_API ParallelClientArrayMediator
		: public virtual ParallelSystemArrayMediator,
		public virtual ParallelClientArray
	{
	protected:
		typedef ParallelSystemArrayMediator super;
		typedef ParallelClientArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelClientArrayMediator();
		virtual ~ParallelClientArrayMediator() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void start() override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};
