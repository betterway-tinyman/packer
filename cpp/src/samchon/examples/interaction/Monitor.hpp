#pragma once
#include <samchon/protocol/Server.hpp>
#include <samchon/examples/interaction/base/MonitorBase.hpp>

#include <samchon/examples/interaction/System.hpp>
#include <samchon/examples/interaction/Reporter.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Monitor 
		: public protocol::Server,
		public base::MonitorBase
	{
	private:
		int sequence;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Monitor() : protocol::Server()
		{
			reporter.reset(new Reporter(this));
			sequence = 0;
		};
		virtual ~Monitor() = default;

		virtual void constructSystemTree() override
		{
			for (auto it = system_map.begin(); it != system_map.end(); it++)
				it->second->clear();

			for (auto it = system_map.begin(); it != system_map.end(); it++)
				if (it->second->getParent() != nullptr)
					it->second->getParent()->push_back(it->second);
		};

	protected:
		virtual void addClient(std::shared_ptr<protocol::ClientDriver> driver) override
		{
			int uid = ++sequence;
			std::unique_ptr<System> system(new System(this, driver, uid));

			system_map.emplace(uid, system.get());
			driver->listen(system.get());

			system_map.erase(uid);
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto getRootSystem() const -> System*
		{
			if (system_map.empty())
				return nullptr;

			// FIND ROOT (CHIEF) SYSTEM
			// CHIEF MAY THE FIRST
			System *system = system_map.begin()->second;
			while (system->getParent() != nullptr)
				system = system->getParent();

			return system;
		};

		/* ---------------------------------------------------------
			MAIN
		--------------------------------------------------------- */
		static void main()
		{
			Monitor monitor;
			monitor.open(37900);
			monitor.getReporter()->open(37950);
		};
	};
};
};
};