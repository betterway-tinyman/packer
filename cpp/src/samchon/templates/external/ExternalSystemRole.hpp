#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IProtocol.hpp>

namespace samchon
{
namespace templates
{
namespace external
{
	class ExternalSystem;

	/**
	 * A role of an external system.
	 * 
	 * The {@link ExternalSystemRole} class represents a role, *WHAT TO DO*. Extends the {@link ExternalSystemRole} class 
	 * and overrides {@link replyData replyData()} to define the *WHAT TO DO*. And assign this {@link ExternalSystemRole}
	 * object to related {@link ExternalSystem} object. 
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_external_system.png)
	 * 
	 * #### Proxy Pattern
	 * The {@link ExternalSystemRole} class can be an *logical proxy*. In framework within user, which 
	 * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not 
	 * important. Only interested in user's perspective is *which can be done*. 
	 * 
	 * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged 
	 * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}. 
	 * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
	 * 
	 * <ul>
	 *	<li>
	 *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
	 *		from {@link ExternalSystem} object, via {@link ExternalSystemArray.getRole ExternalSystemArray.getRole()}.
	 *	</li>
	 *	<li>
	 *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
	 *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
	 *		external system.
	 *	</li>
	 *	<li> Those strategy is called *Proxy Pattern*. </li>
	 * </ul>
	 *  
	 * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/CPP-Templates-External_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class ExternalSystemRole
		: public virtual protocol::Entity<std::string>,
		public virtual protocol::IProtocol
	{
	private:
		typedef Entity<std::string> super;

		ExternalSystem *system;

	protected:
		/**
		 * A name, represents and identifies this {@link ExternalSystemRole role}.
		 * 
		 * This {@link name} is an identifier represents this {@link ExternalSystemRole role}. This {@link name} is
		 * used in {@link ExternalSystemArray.getRole} and {@link ExternalSystem.get}, as a key elements. Thus, this
		 * {@link name} should be unique in an {@link ExternalSystemArray}.
		 */
		std::string name;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Constructor from a system.
		 * 
		 * @param system An external system containing this role.
		 */
		ExternalSystemRole(ExternalSystem *system)
		{
			this->system = system;
		};

		/**
		 * Default Destructor.
		 */
		virtual ~ExternalSystemRole() = default;

		virtual void construct(std::shared_ptr<library::XML> xml)
		{
			name = xml->getProperty("name");
		};

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Identifier of {@link ExternalSystemRole} is its {@link name}.
		 */
		virtual auto key() const -> std::string
		{
			return name;
		};

		/**
		 * Get parent {@link ExternalSystemRole} object.
		 */
		auto getSystem() const -> ExternalSystem*
		{
			return system;
		};

		/**
		 * Get name, who represents and identifies this role.
		 */
		auto getName() const -> std::string
		{
			return name;
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message.
		 * 
		 * Sends an {@link Invoke} message to remote system through the parent {@link ExternalSystem} object.
		 * 
		 * @param invoke An {@link Invoke} message to send to the external system.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			((IProtocol*)system)->sendData(invoke);
		};

		/**
		 * Handle replied {@link Invoke} message.
		 * 
		 * {@link ExternalSystemRole.replyData ExternalSystemRole.replyData()} is an abstract method handling a replied 
		 * {@link Invoke message} gotten from remote system via parent {@link ExternalSystem} object. Overrides this 
		 * method and defines the *WHAT TO DO* with the {@link Invoke message}.
		 * 
		 * @param invoke An {@link Invoke} message received from the {@link ExternalSystem external system}.
		 */
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) = 0;

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string
		{
			return "role";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> xml = super::toXML();
			xml->setProperty("name", name);

			return xml;
		};
	};
};
};
};