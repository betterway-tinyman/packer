#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArray.hpp>
#include <samchon/protocol/ExternalClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief An array of ParallelClient(s).
	 *
	 * @details
	 * <p> ParallelClientArray is an ParallelSystemArray specialized in clients' driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystemArray
	 */
	class SAMCHON_FRAMEWORK_API ParallelClientArray
		: public virtual ParallelSystemArray,
		public virtual ExternalClientArray
	{
	protected:
		typedef ParallelSystemArray super;
		typedef ExternalClientArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelClientArray();
		virtual ~ParallelClientArray() = default;
	};
};
};
};