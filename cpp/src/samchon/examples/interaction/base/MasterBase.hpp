#pragma once
#include <samchon/templates/parallel/ParallelSystemArray.hpp>
#include <samchon/examples/interaction/base/SystemBase.hpp>

#include <queue>
#include <samchon/templates/PInvoke.hpp>
#include <samchon/examples/interaction/MonitorDriver.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class SlaveDriver;
	class TSPRequest;
	class PackerRequest;

namespace base
{
	class MasterBase
		: public virtual templates::parallel::ParallelSystemArray<SlaveDriver>,
		public SystemBase
	{
	protected:
		std::unique_ptr<MonitorDriver> monitor;
		std::unique_ptr<TSPRequest> tsp;
		std::unique_ptr<PackerRequest> packer;

		std::queue<std::shared_ptr<templates::PInvoke>> p_invoke_queue;

	public:
		virtual auto getParent() const -> protocol::IProtocol* = 0;
		auto getMonitor() const -> MonitorDriver*
		{
			return monitor.get();
		};

		auto getTSP() const -> TSPRequest*
		{
			return tsp.get();
		};
		auto getPacker() const -> PackerRequest*
		{
			return packer.get();
		};

		auto _Get_p_invoke_queue() -> std::queue<std::shared_ptr<templates::PInvoke>>&
		{
			return p_invoke_queue;
		};
	};
};
};
};
};