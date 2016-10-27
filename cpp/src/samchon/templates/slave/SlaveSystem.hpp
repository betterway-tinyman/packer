#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IListener.hpp>

#include <samchon/protocol/Communicator.hpp>
#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace templates
{
namespace slave
{
	class SlaveSystem
		: public virtual protocol::IListener
	{
	protected:
		std::shared_ptr<protocol::Communicator> communicator_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		SlaveSystem()
		{
		};
		virtual ~SlaveSystem() = default;

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			communicator_->sendData(invoke);
		};
		
	protected:
		virtual void _replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->has("_History_uid"))
			{
				std::thread([this, invoke]()
				{
					// INIT HISTORY - WITH START TIME
					std::unique_ptr<protocol::InvokeHistory> history(new protocol::InvokeHistory(invoke));
					invoke->erase("_History_uid");

					// MAIN PROCESS - REPLY_DATA
					replyData(invoke);

					// NOTIFY - WITH END TIME
					history->complete();
					sendData(history->toInvoke());
				}).detach();
			}
			else
				replyData(invoke);
		};
	};
};
};
};