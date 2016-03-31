#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystem.hpp>
#include <samchon/protocol/ExternalServer.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	/**
	 * @brief A network driver for a parallel server.
	 *
	 * @details 
	 * <p> ParallelServer is an ParallelSystem specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ParallelSystem
	 */
	class SAMCHON_FRAMEWORK_API ParallelServer
		: public virtual ParallelSystem,
		public virtual ExternalServer
	{
	protected:
		typedef ParallelSystem super;
		typedef ExternalServer network_super;

	public:
		/**
		* @brief Default Constructor.
		*/
		ParallelServer();
		virtual ~ParallelServer() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};