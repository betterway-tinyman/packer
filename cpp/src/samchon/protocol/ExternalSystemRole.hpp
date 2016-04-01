#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <set>

namespace samchon
{
namespace protocol
{
	class ExternalSystem;

	/**
	 * @brief A role belongs to an external system.
	 *
	 * @details
	 * <p> ExternalSystemRole is a 'control' class groupping methods, handling Invoke messages 
	 * interacting with an external system that the ExternalSystemRole is belonged to, by a subject or 
	 * unit of a module. <p>
	 *
	 * <p> ExternalSystemRole can be a "logical proxy" for an ExternalSystem which is containing the 
	 * ExternalSystemRole. Of course, the ExternalSystemRole is belonged to an ExternalSystem. However, 
	 * if you access an ExternalSystemRole from an ExternalSystemArray directly, not passing by a 
	 * belonged ExternalSystem, and send an Invoke message even you're not knowing which ExternalSystem 
	 * is related in, the ExternalSystemRole acted a role of proxy. </p>
	 *
	 * <p> It's called as "Proxy pattern". With the pattern, you can only concentrate on 
	 * ExternalSystemRole itself, what to do with Invoke message, irrespective of the ExternalSystemRole 
	 * is belonged to which ExternalSystem. </p>
	 *
	 * @image html  cpp/protocol_external_system.png
	 * @image latex cpp/protocol_external_system.png
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API ExternalSystemRole
		: public Entity,
		public IProtocol
	{
	protected:
		typedef Entity super;

	private:
		/**
		 * @brief A driver of external system containing the ExternalSystemRole.
		 */
		ExternalSystem *system;

	protected:
		/**
		 * @brief A name representing the role.
		 */
		std::string name;

		/**
		 * @brief Listeners to reply in the role.
		 */
		std::set<std::string> replyListeners;

		/**
		 * @brief Listeners to send in the role.
		 */
		std::set<std::string> sendListeners;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Construct from external system driver
		 *
		 * @param system A driver of external system the ExternalSystemRole is belonged to.
		 */
		ExternalSystemRole(ExternalSystem *);
		virtual ~ExternalSystemRole() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

	private:
		void constructListeners(std::set<std::string> &, const std::shared_ptr<library::XML>);

	public:
		/* ------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------ */
		/**
		 * @brief Get an external system driver.
		 */
		auto getSystem() const->ExternalSystem*;

		virtual auto key() const->std::string override;

		/**
		 * @brief Test whether has a listener for send in the role.
		 */
		auto hasSendListener(const std::string &) const -> bool;

		/**
		 * @brief Test whether has a listener for reply in the role.
		 */
		auto hasReplyListener(const std::string &) const -> bool;

		/* ------------------------------------------------------------------
			CHAIN OF INVOKE MESSAGE
		------------------------------------------------------------------ */
		virtual void sendData(std::shared_ptr<Invoke>) override;

	public:
		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;

	private:
		auto toListenersXML(const std::set<std::string> &, const std::string &) const->std::shared_ptr<library::XML>;
	};
};
};