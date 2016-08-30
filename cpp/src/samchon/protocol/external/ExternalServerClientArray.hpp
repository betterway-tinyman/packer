#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalServerArray.hpp>
#include <samchon/protocol/external/ExternalClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace external
{
	class SAMCHON_FRAMEWORK_API ExternalServerClientArray
		: public virtual ExternalServerArray,
		public virtual ExternalClientArray
	{
	public:
		/* =========================================================
			CONSTRUCTORS
				- DEFAULT
				- FACTORY METHODS FOR CHILDREN
		============================================================
			CONSTRUCTORS
		--------------------------------------------------------- */
		ExternalServerClientArray();
		virtual ~ExternalServerClientArray();

	protected:
		/* ---------------------------------------------------------
			FACTORY METHODS FOR CHILDREN
		--------------------------------------------------------- */
		// virtual void addClient(std::shared_ptr<ClientDriver> driver) override final;

		virtual auto createChild(std::shared_ptr<library::XML> xml) -> ExternalSystem* override final;

		virtual auto createExternalServer(std::shared_ptr<library::XML>) -> ExternalServer* = 0;
		// virtual auto createExternalClient(std::shared_ptr<ClientDriver>) -> ExternalSystem* = 0;
	};
};
};
};