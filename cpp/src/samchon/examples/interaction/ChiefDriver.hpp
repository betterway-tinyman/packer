#pragma once
#include <samchon/templates/slave/SlaveClient.hpp>

#include <samchon/templates/slave/PInvoke.hpp>
#include <samchon/examples/interaction/base/MasterBase.hpp>
#include <samchon/examples/interaction/TSPRequest.hpp>
#include <samchon/examples/interaction/PackerRequest.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class ChiefDriver 
		: public templates::slave::SlaveClient
	{
	private:
		typedef templates::slave::SlaveClient super;

		base::MasterBase *master;
		int uid;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ChiefDriver(base::MasterBase *master)
			: super()
		{
			this->master = master;
			this->uid = -1;
		};
		virtual ~ChiefDriver() = default;

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			super::sendData(invoke);

			if (uid != -1)
				master->getMonitor()->reportSendData(uid, invoke);
		};
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "set_chief_uid")
				uid = invoke->front()->getValue<int>();
			else
			{
				auto pInvoke = std::dynamic_pointer_cast<templates::slave::PInvoke>(invoke);
				if (pInvoke != nullptr)
				{
					pInvoke->hold();
					((base::MasterBase*)master)->_Get_p_invoke_queue().push(pInvoke);
				}
				
				if (invoke->getListener() == "optimizeTSP")
					master->getTSP()->optimize(invoke->front()->getValueAsXML());
				else if (invoke->getListener() == "optimizePacker")
					master->getPacker()->optimize(invoke->front()->getValueAsXML());
			}
		};
	};
};
};
};