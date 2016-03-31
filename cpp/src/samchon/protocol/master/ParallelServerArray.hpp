#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArray.hpp>
#include <samchon/protocol/ExternalServerArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief An array of ParallelServer(s).
	 *
	 * @details
	 * <p> ParallelServerArray is an ParallelSystemArray specialized in servers' driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystemArray
	 */
	class SAMCHON_FRAMEWORK_API ParallelServerArray
		: public virtual ParallelSystemArray,
		public virtual ExternalServerArray
	{
	protected:
		typedef ParallelSystemArray super;
		typedef ExternalServerArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelServerArray();
		virtual ~ParallelServerArray() = default;
	};
};
};
};