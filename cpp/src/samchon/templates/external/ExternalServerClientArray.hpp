#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/ExternalServerArray.hpp>
#include <samchon/templates/external/ExternalClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace external
{
	/**
	 * An array and manager of {@link IExternalServer external servers} and {@link ExternalSystem external clients}.
	 * 
	 * The {@link ExternalServerClientArray} is an abstract class, derived from the {@link ExternalSystemArray} class,
	 * opening a server accepting {@link ExternalSystem external clients} and being a client connecting to
	 * {@link IExternalServer external servers} at the same time.
	 *
	 * Extends this {@link ExternalServerClientArray} and overrides below methods. After the overridings, open server
	 * with {@link open open()} method and connect to {@link IExternalServer external servers} through the
	 * {@link connect connect()} method.
	 *
	 * - {@link createServerBase createServerBase()}
	 * - {@link createExternalClient createExternalClient()}
	 * - {@link createExternalServer createExternalServer()}
	 * 
	 * #### [Inherited] {@link ExternalSystemArray}
	 * @copydetails external::ExternalSystemArray
	 */
	template <class System = ExternalSystem>
	class ExternalServerClientArray
		: public virtual ExternalServerArray<System>,
		public virtual ExternalClientArray<System>
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
		 * @brief Default Constructor.
		 */
		ExternalServerClientArray()
			: ExternalServerArray<System>(),
			ExternalClientArray<System>()
		{
		};

		/**
		 * @brief Default Destructor.
		 */
		virtual ~ExternalServerClientArray() = default;

	protected:
		/* ---------------------------------------------------------
			FACTORY METHODS FOR CHILDREN
		--------------------------------------------------------- */
		/**
		 * Factory method of a child Entity.
		 *
		 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
		 *
		 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
		 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
		 */
		virtual auto createChild(std::shared_ptr<library::XML> xml) -> ExternalSystem* override final
		{
			return nullptr;
		};

		/**
		 * Factory method creating an {@link IExternalServer} object.
		 *
		 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
		 * @return A newly created {@link IExternalServer} object.
		 */
		virtual auto createExternalServer(std::shared_ptr<library::XML>) -> ExternalServer* = 0;
	};
};
};
};