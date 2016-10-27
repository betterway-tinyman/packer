#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityDeque.hpp>
#	include <samchon/templates/external/ExternalSystemRole.hpp>
#include <samchon/templates/external/base/ExternalSystemBase.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <samchon/templates/external/base/ExternalSystemArrayBase.hpp>
#include <samchon/templates/external/base/ExternalServerBase.hpp>

#include <samchon/protocol/ClientDriver.hpp>

namespace samchon
{
namespace templates
{
namespace external
{
	/**
	 * An external system driver.
	 * 
	 * The {@link ExternalSystem} class represents an external system, connected and interact with this system. 
	 * {@link ExternalSystem} takes full charge of network communication with the remote, external system have connected.
	 * Replied {@link Invoke} messages from the external system is shifted to and processed in, children elements of this
	 * class, {@link ExternalSystemRole} objects.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_external_system.png)
	 * 
	 * #### Bridge & Proxy Pattern
	 * The {@link ExternalSystem} class can be a *bridge* for *logical proxy*. In framework within user, 
	 * which {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
	 * important. Only interested in user's perspective is *which can be done*.
	 *
	 * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
	 * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
	 * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
	 *
	 * <ul>
	 *	<li>
	 *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
	 *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
	 *	</li>
	 *	<li>
	 *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
	 *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the 
	 *		external system.
	 *	</li>
	 *	<li> Those strategy is called *Bridge Pattern* and *Proxy Pattern*. </li>
	 * </ul>
	 * 
	 * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/CPP-Templates-External_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class ExternalSystem
		: public protocol::SharedEntityDeque<ExternalSystemRole>,
		public base::ExternalSystemBase,
		public virtual protocol::IProtocol
	{
	private:
		typedef protocol::SharedEntityDeque<ExternalSystemRole> super;

	protected:
		std::shared_ptr<protocol::Communicator> communicator_;
	
		/**
		 * The name represents external system have connected. 
		 */
		std::string name;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		ExternalSystem()
			: super()
		{
			this->system_array_ = nullptr;
		}

		/**
		 * Construct from parent {@link ExternalSystemArray}.
		 * 
		 * @param systemArray The parent {@link ExternalSystemArray} object.
		 */
		ExternalSystem(base::ExternalSystemArrayBase *systemArray)
			: super()
		{
			this->system_array_ = systemArray;
		};

		/**
		 * Constrct from parent {@link ExternalSystemArray} and communicator.
		 * 
		 * @param systemArray The parent {@link ExternalSystemArray} object.
		 * @param communicator Communicator with the remote, external system.
		 */
		ExternalSystem(base::ExternalSystemArrayBase *systemArray, std::shared_ptr<protocol::ClientDriver> driver)
		{
			this->communicator_ = driver;
		};

		/**
		 * Default Destructor.
		 * 
		 * The {@link ExternalSystem} object is destructed when connection with the remote system is closed or this 
		 * {@link ExternalSystem} object is {@link ExternalSystemArray.erase erased} from its parent 
		 * {@link ExternalSystemArray} object.
		 */
		virtual ~ExternalSystem() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			name = xml->fetchProperty("name");

			base::ExternalServerBase *server = dynamic_cast<base::ExternalServerBase*>(this);
			if (server != nullptr)
			{
				server->ip = xml->getProperty<std::string>("ip");
				server->port = xml->getProperty<int>("port");
			}

			super::construct(xml);
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */	
		/**
		 * Identifier of {@link ExternalSystem} is its {@link name}.
		 * 
		 * @return name.
		 */
		virtual auto key() const -> std::string
		{
			return name;
		};

		/**
		 * Get {@link name}.
		 */
		auto getName() const -> std::string
		{
			return name;
		};

	public:
		/* ---------------------------------------------------------
			NETWORK & MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Close connection.
		 */
		void close()
		{
			communicator_->close();
		};

		/**
		 * Send {@link Invoke} message to external system.
		 * 
		 * @param invoke An {@link Invoke} message to send.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			communicator_->sendData(invoke);
		};

		/**
		 * Handle an {@Invoke} message has received.
		 * 
		 * @param invoke An {@link Invoke} message have received.
		 */
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			// SHIFT TO ROLES
			for (size_t i = 0; i < size(); i++)
				at(i)->replyData(invoke);

			// SHIFT TO SYSTEM_ARRAY
			((IProtocol*)system_array_)->replyData(invoke);
		};

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "system";
		};
		virtual auto CHILD_TAG() const -> std::string override
		{
			return "role";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> &xml = super::toXML();
			xml->setProperty("name", name);

			const base::ExternalServerBase *server = dynamic_cast<const base::ExternalServerBase*>(this);
			if (server != nullptr)
			{
				xml->setProperty("ip", server->ip);
				xml->setProperty("port", server->port);
			}

			return xml;
		};
	};
};
};
};