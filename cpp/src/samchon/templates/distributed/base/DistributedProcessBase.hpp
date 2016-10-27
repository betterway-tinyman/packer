#pragma once
#include <samchon/API.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/templates/distributed/DSInvokeHistory.hpp>

namespace samchon
{
namespace protocol
{
	class Invoke;
};

namespace templates
{
namespace distributed
{
	class DistributedProcess;
	class DistributedSystem;

namespace base
{
	class DistributedProcessBase
	{
		friend class DistributedProcess;

	private:
		HashMap<size_t, std::shared_ptr<DSInvokeHistory>> progress_list_;
		HashMap<size_t, std::shared_ptr<DSInvokeHistory>> history_list_;

		double resource;
		bool enforced_;

	public:
		auto getResource() const -> double
		{
			return resource;
		};

		virtual auto sendData(std::shared_ptr<protocol::Invoke>, double) -> std::shared_ptr<DistributedSystem> = 0;

		virtual void replyData(std::shared_ptr<protocol::Invoke>) = 0;

		void _Report_history(std::shared_ptr<DSInvokeHistory> history)
		{
			// ERASE FROM ORDINARY PROGRESS AND MIGRATE TO THE HISTORY
			progress_list_.erase(history->getUID());
			history_list_.emplace(history->getUID(), history);
		};
	};
};
};
};
};