#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/ParallelSystem.hpp>
#include <samchon/protocol/slave/SlaveClient.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	/**
	 * @brief A slave parallel client.
	 *
	 * @details
	 * <p> ParallelClient is a ParallelSystem class specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *	   @copydetails slave::ParallelSystem
	 */
	class SAMCHON_FRAMEWORK_API ParallelClient
		: public virtual ParallelSystem,
		public virtual SlaveClient
	{
	protected:
		typedef ParallelSystem super;
		typedef SlaveClient network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelClient();
		virtual ~ParallelClient() = default;
	};
};
};
};