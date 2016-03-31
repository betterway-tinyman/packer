#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSystem.hpp>
#include <samchon/protocol/ExternalClient.hpp>

namespace samchon
{
namespace protocol
{	
namespace master
{
	/**
	 * @brief A network driver for a distributed client.
	 *
	 * @details 
	 * <p> DistributedClient is an DistributedSystem specialized in client driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystem
	 */
	class SAMCHON_FRAMEWORK_API DistributedClient
		: public virtual DistributedSystem,
		public virtual ExternalClient
	{
	private:
		typedef DistributedSystem super;
		typedef ExternalClient network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedClient();
		virtual ~DistributedClient() = default;
	};
};
};
};