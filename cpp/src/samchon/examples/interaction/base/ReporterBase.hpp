#pragma once

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Monitor;

namespace base
{
	class ReporterBase
	{
	public:
		virtual auto getMonitor() const -> Monitor* = 0;

		virtual void sendSystems() = 0;
	};
};
};
};
};