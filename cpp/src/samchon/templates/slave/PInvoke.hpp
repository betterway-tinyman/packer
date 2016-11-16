#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Invoke.hpp>

#include <samchon/templates/slave/InvokeHistory.hpp>
#include <samchon/protocol/IProtocol.hpp>

namespace samchon
{
namespace templates
{
namespace slave
{
	class PInvoke : public protocol::Invoke
	{
	private:
		typedef protocol::Invoke super;

		std::shared_ptr<InvokeHistory> history_;
		protocol::IProtocol *master_driver_;

		bool hold_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS AND PROCEDURES
		--------------------------------------------------------- */
		/**
		 * Initializer Constructor.
		 *
		 * @param invoke The {@link Invoke} message came from the master.
		 * @param history A {@link InvokeHistory history} object archiving the process command.
		 * @param masterDriver 
		 */
		PInvoke(const std::shared_ptr<protocol::Invoke> &invoke, std::shared_ptr<InvokeHistory> history, protocol::IProtocol *masterDriver)
			: super(invoke->getListener()) // COPY LISTENER
		{
			// ASSIGN PARAMETERS
			assign(invoke->begin(), invoke->end());

			// MEMBERS
			this->history_ = history;
			this->master_driver_ = masterDriver;

			// DEFAULT IS NOT BEING HOLD
			this->hold_ = false;
		};

		void hold()
		{
			hold_ = true;
		};

		void complete()
		{
			history_->complete();

			master_driver_->sendData(history_->toInvoke());
		};

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto getHistory() const -> std::shared_ptr<InvokeHistory>
		{
			return history_;
		};
		auto isHold() const -> bool
		{
			return hold_;
		};
	};
};
};
};