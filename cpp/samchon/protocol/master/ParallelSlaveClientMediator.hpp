#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSlaveSystemMediator.hpp>
#include <samchon/protocol/slave/ParallelClient.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A parallel slave client for mediation.
	 *
	 * @details 
	 * <p> ParallelSlaveClientMediator is a ParallelSlaveSystemMediator class 
	 * specialized in client driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ParallelSlaveSystemMediator
	 */
	class SAMCHON_FRAMEWORK_API ParallelSlaveClientMediator
		: public virtual ParallelSlaveSystemMediator,
		public virtual slave::ParallelClient
	{
	protected:
		typedef ParallelSlaveSystemMediator super;
		typedef slave::ParallelClient network_supr;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelSlaveClientMediator();
		virtual ~ParallelSlaveClientMediator() = default;

		void setAddress(const std::string &, int, const std::string & = "");
	};
};
};
};