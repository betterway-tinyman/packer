#pragma once
#include <samchon/examples/interaction/base/SystemBase.hpp>

#include <samchon/examples/interaction/MonitorDriver.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class MasterSystem;

namespace base
{
	class ChiefBase : public SystemBase
	{
	protected:
		std::unique_ptr<MonitorDriver> monitor;

	public:
		auto getMonitor() const -> MonitorDriver*
		{
			return monitor.get();
		};
	};
};
};
};
};