#pragma once
#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystemArray;
	class ParallelSystemArrayMediator;
	class PRInvokeHistory;

	class SAMCHON_FRAMEWORK_API ParallelSystem 
		: public virtual external::ExternalSystem
	{
		friend class ParallelSystemArray;
		friend class ParallelSystemArrayMediator;

	private:
		typedef external::ExternalSystem super;

		ParallelSystemArray* systemArray;

		HashMap<size_t, std::shared_ptr<PRInvokeHistory>> progress_list;
		HashMap<size_t, std::shared_ptr<PRInvokeHistory>> history_list;

		double performance;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ParallelSystem(ParallelSystemArray *systemArray);
		virtual ~ParallelSystem();

		virtual void construct(std::shared_ptr<library::XML> xml) override;

		/* ---------------------------------------------------------
			GETTERS
		--------------------------------------------------------- */
		auto getSystemArray() const -> ParallelSystemArray*
		{
			return systemArray;
		};

		auto getPerformance() const -> double
		{
			return performance;
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void replyData(std::shared_ptr<Invoke>) override final;

	private:
		void send_piece_data(std::shared_ptr<Invoke>, size_t, size_t);

		void report_invoke_history(std::shared_ptr<library::XML>);

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};
};