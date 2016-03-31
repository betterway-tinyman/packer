#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArrayMediator.hpp>
#include <samchon/protocol/master/ParallelServerArray.hpp>

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
	 * <p> ParallelServerArrayMediator is a ParallelSystemArrayMediator specialized in 
	 * servers' driver. ParallelServerArrayMediator is a server for its master and also 
	 * manages children servers' slaves. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ParallelSystemArrayMediator
	 */
	class SAMCHON_FRAMEWORK_API ParallelServerArrayMediator
		: public virtual ParallelSystemArrayMediator,
		public virtual ParallelServerArray
	{
	protected:
		typedef ParallelSystemArrayMediator super;
		typedef ParallelServerArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelServerArrayMediator();
		virtual ~ParallelServerArrayMediator() = default;

		virtual void start() override;
	};
};
};
};
