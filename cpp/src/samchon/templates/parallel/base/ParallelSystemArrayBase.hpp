#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/base/ExternalSystemArrayBase.hpp>

namespace samchon
{
namespace templates
{
namespace slave
{
	class InvokeHistory;
};

namespace parallel
{
namespace base
{
	class ParallelSystemArrayBase
	{
	private:
		size_t history_sequence_{0};

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ParallelSystemArrayBase()
		{
			history_sequence_ = 0;
		};
		virtual ~ParallelSystemArrayBase() = default;

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto _Get_history_sequence() const -> size_t
		{
			return history_sequence_;
		};
		auto _Fetch_history_sequence() -> size_t
		{
			return ++history_sequence_;
		};

		void _Set_history_sequence(size_t val)
		{
			history_sequence_ = val;
		};

	public:
		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual auto sendPieceData(std::shared_ptr<protocol::Invoke> invoke, size_t first, size_t last) -> size_t = 0;

		virtual auto _Complete_history(std::shared_ptr<slave::InvokeHistory> history) -> bool = 0;
	};
};
};
};
};