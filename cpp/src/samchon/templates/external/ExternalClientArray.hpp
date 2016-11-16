#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/ExternalSystemArray.hpp>

#include <samchon/protocol/Server.hpp>
#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace templates
{
namespace external
{
	/**
	 * An array and manager of {@link ExternalSystem external clients} as a server.
	 * 
	 * The {@link ExternalClientArray} is an abstract class, derived from the {@link ExternalSystemArray} class, opening
	 * a server accepting {@link ExternalSystem external clients}.
	 *
	 * Extends this {@link ExternalClientArray}, overrides {@link createExternalClient createExternalClient()} creating 
	 * child {@link ExternalSystem} object. After the extending and overridings, open this server using the 
	 * {@link open open()} method. If you want this server to follow web-socket protocol, then overrides {@link WebServer} 
	 * virtually.
	 * 
	 * #### [Inherited] {@link ExternalSystemArray}
	 * @copydetails external::ExternalSystemArray
	 */
	template <class System = ExternalSystem>
	class ExternalClientArray
		: public virtual ExternalSystemArray<System>,
		public virtual protocol::Server
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
		ExternalClientArray()
			: ExternalSystemArray<System>(),
			protocol::Server()
		{
		};

		virtual ~ExternalClientArray() = default;

	protected:
		/* ---------------------------------------------------------
			FACTORY METHODS FOR CHILDREN
		--------------------------------------------------------- */
		/**
		 * Add a newly connected remote client.
		 * 
		 * When a {@link ClientDriver remote client} connects to this *server* {@link ExternalClientArray} object, 
		 * then this {@link ExternalClientArray} creates a child {@link ExternalSystem external client} object through 
		 * the {@link createExternalClient createExternalClient()} method and {@link insert inserts} it.
		 * 
		 * @param driver A communicator for external client.
		 */
		virtual void addClient(std::shared_ptr<protocol::ClientDriver> driver) override final
		{
			std::shared_ptr<ExternalSystem> system(createExternalClient(driver));
			if (system == nullptr)
				return;

			system->communicator_ = driver;
			{
				library::UniqueWriteLock uk(getMutex());
				push_back(system);
			}
			driver->listen(system.get());

			for (size_t i = 0; i < size(); i++)
				if (at(i) == system)
				{
					library::UniqueWriteLock uk(getMutex());

					erase(begin() + i);
					break;
				}
		};

		/**
		 * (Deprecated) Factory method creating child object.
		 * 
		 * The method {@link createChild createChild()} is deprecated. Don't use and override this. 
		 * 
		 * Note that, the {@link ExternalClientArray} is a server accepting {@link ExternalSystem external clients}.
		 * There's no way to creating the {@link ExternalSystem external clients} in advance before opening the server.
		 * 
		 * @param xml An {@link XML} object represents the child {@link ExternalSystem} object.
		 * @return null
		 */
		virtual auto createChild(std::shared_ptr<library::XML> xml) -> System* override
		{
			return nullptr;
		} // = delete;

		/**
		 * Factory method creating a child {@link ExternalSystem} object.
		 * 
		 * @param driver A communicator with connected client.
		 * @return A newly created {@link ExternalSystem} object.
		 */
		virtual auto createExternalClient(std::shared_ptr<protocol::ClientDriver>) -> System* = 0;
	};
};
};
};