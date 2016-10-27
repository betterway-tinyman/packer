#pragma once
#include <samchon/API.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/protocol/Entity.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	class DistributedProcess;

namespace base
{
	class IDistributedSystemArray
	{
	protected:
		HashMap<std::string, std::shared_ptr<DistributedProcess>> process_map_;

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get process map. 
		 * 
		 * Gets an {@link HashMap} containing {@link DistributedProcess} objects with their *key*.
		 * 
		 * @return An {@link HasmMap> containing pairs of string and {@link DistributedProcess} object.
		 */
		auto getProcessMap() const -> const HashMap<std::string, std::shared_ptr<DistributedProcess>>&
		{
			return process_map_;
		};

		/**
		 * Test whether the process exists.
		 * 
		 * @param name Name, identifier of target {@link DistributedProcess process}.
		 * 
		 * @return Whether the process has or not.
		 */
		auto hasProcess(const std::string &name) const -> bool
		{
			return process_map_.has(name);
		};

		/**
		 * Get a process.
		 * 
		 * @param name Name, identifier of target {@link DistributedProcess process}.
		 * 
		 * @return The specified process.
		 */
		auto getProcess(const std::string &name) const -> std::shared_ptr<DistributedProcess>
		{
			return process_map_.get(name);
		};

		/**
		 * Insert a process.
		 * 
		 * @param process A process to be inserted.
		 * @return Success flag.
		 */
		void insertProcess(std::shared_ptr<DistributedProcess> role)
		{
			process_map_.emplace(((protocol::Entity<std::string>*)role.get())->key(), role);
		};

		/**
		 * Erase a process.
		 * 
		 * @param name Name, identifier of target {@link DistributedProcess process}.
		 */
		void eraseProcess(const std::string &name)
		{
			process_map_.erase(name);
		};
	};
};
};
};
};