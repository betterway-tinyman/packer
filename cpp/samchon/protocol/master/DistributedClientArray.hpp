#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSystemArray.hpp>
#include <samchon/protocol/ExternalClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief An array of DistributedClient(s).
	 *
	 * @details
	 * <p> DistributedClientArray is an DistributedSystemArray specialized in clients' driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystemArray
	 */
	class SAMCHON_FRAMEWORK_API DistributedClientArray
		: public virtual DistributedSystemArray,
		public virtual ExternalClientArray
	{
	private:
		typedef DistributedSystemArray super;
		typedef ExternalClientArray network_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedClientArray();
		virtual ~DistributedClientArray() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void start() override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};