#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IListener.hpp>

#include <samchon/protocol/Communicator.hpp>
#include <samchon/templates/slave/PInvoke.hpp>

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
		virtual void _Reply_data(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->has("_History_uid"))
			{
				std::thread([this, invoke]()
				{
					// INIT HISTORY - WITH START TIME
					std::shared_ptr<InvokeHistory> history(new InvokeHistory(invoke));
					invoke->erase("_History_uid");
					invoke->erase("_Process_name");
					invoke->erase("_Process_weight");

					// MAIN PROCESS - REPLY_DATA
					std::shared_ptr<PInvoke> pInvoke(new PInvoke(invoke, history, this));
					replyData(pInvoke);

					// NOTIFY - WITH END TIME
					if (pInvoke->isHold() == false)
						pInvoke->complete();
				}).detach();
			}
			else
				replyData(invoke);
		};
	};
};
};
};