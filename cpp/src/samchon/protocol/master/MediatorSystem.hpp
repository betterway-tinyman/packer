#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>

namespace samchon
{
namespace protocol
{
	class InvokeHistory;

namespace external
{
	class ExternalClientArray;
};
namespace master
{
	class SAMCHON_FRAMEWORK_API MediatorSystem
		: public virtual slave::SlaveSystem
	{
		friend class ParallelSystemArrayMediator;

	private:
		typedef slave::SlaveSystem super;

		external::ExternalClientArray *system_array;
		HashMap<size_t, std::shared_ptr<InvokeHistory>> progress_list;

	public:
		MediatorSystem(external::ExternalClientArray *systemArray);
		virtual ~MediatorSystem();

		virtual void start() = 0;

		void replyData(std::shared_ptr<Invoke>) override;

	private:
		void notify_end(size_t uid);
	};
};
};
};