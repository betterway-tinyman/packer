#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/ParallelSystem.hpp>
#include <samchon/protocol/slave/SlaveServer.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	/**
	 * @brief A slave parallel server.
	 *
	 * @details
	 * <p> ParallelSystem is a ParallelSystem class specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails slave::ParallelSystem
	 */
	class SAMCHON_FRAMEWORK_API ParallelServer
		: public virtual ParallelSystem,
		public virtual SlaveServer
	{
	private:
		typedef ParallelSystem super;
		typedef SlaveServer network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		ParallelServer();
		virtual ~ParallelServer() = default;
	};
};
};
};