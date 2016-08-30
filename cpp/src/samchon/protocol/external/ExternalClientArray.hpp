#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystemArray.hpp>

#include <samchon/protocol/Server.hpp>
#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace protocol
{
namespace external
{
	class SAMCHON_FRAMEWORK_API ExternalClientArray
		: public virtual ExternalSystemArray,
		public virtual Server
	{
	public:
		/* =========================================================
			CONSTRUCTORS
				- DEFAULT
				- FACTORY METHODS FOR CHILDREN
		============================================================
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		ExternalClientArray();
		virtual ~ExternalClientArray();

	protected:
		/* ---------------------------------------------------------
			FACTORY METHODS FOR CHILDREN
		--------------------------------------------------------- */
		virtual void addClient(std::shared_ptr<ClientDriver> driver) override final;

		virtual auto createChild(std::shared_ptr<library::XML> xml) -> ExternalSystem* override; // = delete;
		virtual auto createExternalClient(std::shared_ptr<ClientDriver>) -> ExternalSystem* = 0;
	};
};
};
};