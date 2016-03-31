#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSlaveSystemMediator.hpp>
#include <samchon/protocol/slave/ParallelServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A parallel slave server for mediation.
	 *
	 * @details 
	 * <p> ParallelSlaveServerMediator is a ParallelSlaveSystemMediator class 
	 * specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ParallelSlaveSystemMediator
	 */
	class SAMCHON_FRAMEWORK_API ParallelSlaveServerMediator
		: public virtual ParallelSlaveSystemMediator,
		public virtual slave::ParallelServer
	{
	private:
		typedef ParallelSlaveSystemMediator super;
		typedef slave::ParallelServer network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelSlaveServerMediator();
		virtual ~ParallelSlaveServerMediator() = default;

		void setAddress(int);
	};
};
};
};