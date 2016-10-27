#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
namespace base
{
	class ParallelSystemArrayBase
	{
	private:
		size_t history_sequence;

	public:
		ParallelSystemArrayBase()
		{
			history_sequence = 0;
		};
		virtual ~ParallelSystemArrayBase() = default;

		auto _Get_history_sequence() const -> size_t
		{
			return history_sequence;
		};
		void _Set_history_sequence(size_t val)
		{
			history_sequence = val;
		};

		auto _Fetch_history_sequence() -> size_t
		{
			++history_sequence;
		};

		virtual auto sendPieceData(std::shared_ptr<protocol::Invoke>, size_t, size_t) -> size_t = 0;

		virtual auto _Complete_history(std::shared_ptr<protocol::InvokeHistory>) -> bool = 0;

	protected:
		virtual void _Normalize_performance() = 0;
	};
};
};
};
};