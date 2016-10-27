#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/ExternalSystem.hpp>
#include <samchon/templates/external/base/ExternalServerBase.hpp>

#include <samchon/protocol/ServerConnector.hpp>

namespace samchon
{
namespace templates
{
namespace external
{
	/**
	 * An external server driver.
	 *
	 * The {@link ExternalServer} is an abstract class, derived from the {@link ExternalSystem} class, connecting to
	 * remote, external server. Extends this {@link ExternalServer} class and overrides the
	 * {@link createServerConnector createServerConnector()} method following which protocol the external server uses.
	 * 
	 * #### [Inherited] {@link ExternalSystem}
	 * @copydetails external::ExternalSystem
	 */
	class ExternalServer
		: public virtual ExternalSystem,
		public base::ExternalServerBase
	{
		friend class ExternalSystem;

	private:
		typedef ExternalSystem super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Construct from parent {@link ExternalSystemArray}.
		 * 
		 * @param systemArray The parent {@link ExternalSystemArray} object.
		 */
		ExternalServer(base::ExternalSystemArrayBase *systemArray)
			: super(systemArray)
		{
		};

		virtual ~ExternalServer() = default;

	protected:
		/**
		 * Factory method creating {@link IServerConnector} object.
		 * 
		 * The {@link createServerConnector createServerConnector()} is an abstract method creating 
		 * {@link IServerConnector} object. Overrides and returns one of them, considering which templates the external
		 * system follows:
		 * 
		 * - {@link ServerConnector}
		 * - {@link WebServerConnector}
		 * 
		 * @return A newly created {@link IServerConnector} object.
		 */
		virtual auto createServerConnector() -> protocol::ServerConnector*
		{
			return new protocol::ServerConnector(this);
		};

	public:
		/* ---------------------------------------------------------
			NETWORK
		--------------------------------------------------------- */
		/**
		 * Connect to external server.
		 */
		virtual void connect()
		{
			if (communicator_ != nullptr || ip.empty() == true)
				return;

			// CREATE CONNECTOR AND CONNECT
			std::shared_ptr<protocol::ServerConnector> connector(this->createServerConnector());
			this->communicator_ = connector;

			connector->connect(ip, port);

			// AFTER DISCONNECTION, ERASE THIS OBJECT
			protocol::SharedEntityDeque<ExternalSystem> *systemArray = (protocol::SharedEntityDeque<ExternalSystem>*)system_array_;
			for (size_t i = 0; i < systemArray->size(); i++)
				if (systemArray->at(i).get() == this)
				{
					systemArray->erase(systemArray->begin() + i);
					break;
				}
		};
	};
};
};
};