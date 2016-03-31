#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <set>
#include <string>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief A role belongs to a system.
	 *
	 * @details
	 * <p> SystemRole is an abstract and a 'control' class groupping methods, handling Invoke messages 
	 * interacting with an system that the SystemRole is belonged to, by a subject or unit of a module. <p>
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API SystemRole
		: public virtual Entity,
		public virtual IProtocol
	{
	protected:
		typedef Entity super;

		/**
		 * @brief A name representing the role
		 */
		std::string name;

		/**
		 * @brief Listeners belongs to the role
		 */
		std::set<std::string> listeners;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor
		 */
		SystemRole();
		virtual ~SystemRole() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* ------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------ */
		virtual auto key() const->std::string override;

		/**
		 * @brief Test whether has a listener in the role
		 */
		auto hasListener(const std::string &) const -> bool;

		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};