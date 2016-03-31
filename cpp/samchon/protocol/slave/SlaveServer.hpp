#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>
#include <samchon/protocol/ExternalClient.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	/**
	 * @brief A slave server.
	 *
	 * @details 
	 * <p> SlaveServer is a SlaveSystem specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails slave::SlaveSystem
	 */
	class SAMCHON_FRAMEWORK_API SlaveServer
		: public virtual SlaveSystem,
		public virtual ExternalClient
	{
	protected:
		typedef SlaveSystem super;
		typedef ExternalClient network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		SlaveServer();
		virtual ~SlaveServer() = default;
	};
};
};
};