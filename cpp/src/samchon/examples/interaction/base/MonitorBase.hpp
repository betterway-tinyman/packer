#pragma once

#include <memory>
#include <samchon/HashMap.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Monitor;

	class Reporter;
	class System;

namespace base
{
	class MonitorBase
	{
		friend class Monitor;

	private:
		HashMap<int, System*> system_map;
		std::unique_ptr<Reporter> reporter;

	public:
		virtual void constructSystemTree() = 0;

		auto getReporter() const -> Reporter*
		{
			return reporter.get();
		};

		auto getSystems() -> HashMap<int, System*>& { return system_map; };
		auto getSystems() const -> const HashMap<int, System*>& { return system_map; };

		virtual auto getRootSystem() const -> System* = 0;
	};
};
};
};
};