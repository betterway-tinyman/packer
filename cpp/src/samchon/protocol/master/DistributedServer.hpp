#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSystem.hpp>
#include <samchon/protocol/ExternalServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A network driver for a distributed server.
	 *
	 * @details 
	 * <p> DistributedServer is an DistributedSystem specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystem
	 */
	class SAMCHON_FRAMEWORK_API DistributedServer
		: public virtual DistributedSystem,
		public virtual ExternalServer
	{
	private:
		typedef DistributedSystem super;
		typedef ExternalServer network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedServer();
		virtual ~DistributedServer() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};