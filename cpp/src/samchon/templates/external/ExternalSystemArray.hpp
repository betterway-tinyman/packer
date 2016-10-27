#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityDeque.hpp>
#	include <samchon/templates/external/ExternalSystem.hpp>
#include <samchon/templates/external/base/ExternalSystemArrayBase.hpp>
#include <samchon/protocol/IProtocol.hpp>

namespace samchon
{
namespace templates
{
/**
 * A template for External Systems Manager.
 * 
 * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/CPP-Templates-External_System)
 * @author Jeongho Nam <http://samchon.org>
 */
namespace external
{
	/**
	 * An array and manager of {@link ExternalSystem external system drivers}.
	 * 
	 * The {@link ExternalSystemArray} is an abstract class containing and managing external system drivers, 
	 * {@link ExternalSystem} objects. Within framewokr of network, {@link ExternalSystemArray} represents your system 
	 * and children {@link ExternalSystem} objects represent remote, external systems connected with your system. 
	 * With this {@link ExternalSystemArray}, you can manage multiple external systems as a group. 
	 * 
	 * You can specify this {@link ExternalSystemArray} class to be *a server accepting external clients* or 
	 * *a client connecting to external servers*. Even both of them is also possible. 
	 * 
	 * - {@link ExternalClientArray}: A server accepting {@link ExternalSystem external clients}.
	 * - {@link ExternalServerArray}: A client connecting to {@link ExternalServer external servers}.
	 * - {@link ExternalServerClientArray}: Both of them. Accepts {@link ExternalSystem external clients} and connects to 
	 *                                      {@link ExternalServer external servers} at the same time.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_external_system.png)
	 * 
	 * #### Proxy Pattern
	 * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
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
	 *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
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
	template <class System = ExternalSystem>
	class ExternalSystemArray
		: public protocol::SharedEntityDeque<System>,
		public base::ExternalSystemArrayBase,
		public protocol::IProtocol
	{
	private:
		typedef protocol::SharedEntityDeque<System> super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		ExternalSystemArray()
			: super()
		{
		};

		/**
		 * Default Destructor.
		 */
		virtual ~ExternalSystemArray() = default;
		
	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Test whether the role exists.
		 * 
		 * @param name Name, identifier of target {@link ExternalSystemRole role}.
		 * 
		 * @return Whether the role has or not.
		 */
		auto hasRole(const std::string &key) const -> bool
		{
			for (size_t i = 0; i < size(); i++)
				for (size_t j = 0; j < at(i)->size(); j++)
					if (at(i)->at(j)->key() == key)
						return true;

			return false;
		};

		/**
		 * Get a role.
		 * 
		 * @param name Name, identifier of target {@link ExternalSystemRole role}.
		 * 
		 * @return The specified role.
		 */
		auto getRole(const std::string &key) const -> std::shared_ptr<ExternalSystemRole>
		{
			for (size_t i = 0; i < size(); i++)
				for (size_t j = 0; j < at(i)->size(); j++)
					if (at(i)->at(j)->key() == key)
						return at(i)->at(j);

			throw std::out_of_range("No such key.");
		};

		virtual auto _Get_children() const -> std::vector<std::shared_ptr<ExternalSystem>>
		{
			std::vector<std::shared_ptr<ExternalSystem>> children(begin(), end());
			return children;
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message.
		 * 
		 * @param invoke An {@link Invoke} message to send.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke)
		{
			for (size_t i = 0; i < size(); i++)
				at(i)->sendData(invoke);
		};

		/**
		 * Handle an {@Invoke} message have received.
		 * 
		 * @param invoke An {@link Invoke} message have received.
		 */
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) = 0;

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "systemArray";
		};
		virtual auto CHILD_TAG() const -> std::string override
		{
			return "system";
		};
	};
};
};
};