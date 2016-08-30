#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class MediatorSystem;

	class SAMCHON_FRAMEWORK_API ParallelSystemArrayMediator
		: public ParallelSystemArray
	{
	private:
		typedef ParallelSystemArray super;

		std::unique_ptr<MediatorSystem> mediator;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ParallelSystemArrayMediator();
		virtual ~ParallelSystemArrayMediator();

	protected:
		virtual auto createMediator() -> MediatorSystem* = 0;

	public:
		/* ---------------------------------------------------------
			NETWORK INITIALIZATION
		--------------------------------------------------------- */
	protected:
		void start_mediator();

	public:
		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke>) override;

		virtual void sendPieceData(std::shared_ptr<Invoke> invoke, size_t index, size_t count);
		
	private:
		virtual auto notify_end(std::shared_ptr<PRInvokeHistory>) -> bool;
	};
};
};
};