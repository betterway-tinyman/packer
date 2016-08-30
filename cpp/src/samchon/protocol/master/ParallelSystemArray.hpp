#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystemArray.hpp>
#	include <samchon/protocol/master/ParallelSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystemArrayMediator;
	class ParallelSystem;
	class PRInvokeHistory;

	class SAMCHON_FRAMEWORK_API ParallelSystemArray
		: public virtual external::ExternalSystemArray
	{
		friend class ParallelSystemArrayMediator;
		friend class ParallelSystem;

	private:
		typedef external::ExternalSystemArray super;

		size_t history_sequence;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ParallelSystemArray();
		virtual ~ParallelSystemArray();

		SHARED_ENTITY_DEQUE_ELEMENT_ACCESSOR_INLINE(ParallelSystem)

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		void sendSegmentData(std::shared_ptr<Invoke> invoke, size_t size)
		{
			sendPieceData(invoke, 0, size);
		};
		virtual void sendPieceData(std::shared_ptr<Invoke> invoke, size_t index, size_t count);

	private:
		virtual auto notify_end(std::shared_ptr<PRInvokeHistory>) -> bool;

		void normalize_performance();
	};
};
};
};