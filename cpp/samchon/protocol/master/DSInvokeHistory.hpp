#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class DistributedSystem;
	class DistributedSystemRole;

	/**
	 * @brief A reported history of an Invoke message
	 *
	 * @details
	 * \par [Inherited]
	 *		@copydetails protocol::InvokeHistory
	 */
	class SAMCHON_FRAMEWORK_API DSInvokeHistory
		: public InvokeHistory
	{
	private:
		typedef InvokeHistory super;

	protected:
		/**
		 * @brief Source system
		 */
		DistributedSystem *system;

		/**
		 * @brief Source role
		 */
		DistributedSystemRole *role;

	public:
		/**
		 * @brief Construct from a system and role.
		 *
		 * @param system a source system.
		 * @param role a source role.
		 */
		DSInvokeHistory(DistributedSystem*, DistributedSystemRole*, std::shared_ptr<Invoke>);

		/**
		 * @brief Get source system.
		 */
		auto getSystem() const->DistributedSystem*;

		/**
		 * @brief Gets source role.
		 */
		auto getRole() const->DistributedSystemRole*;
	};
};
};
};