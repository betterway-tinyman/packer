#pragma once
#include <samchon/templates/parallel/MediatorClient.hpp>

#include <samchon/examples/interaction/base/MasterBase.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Mediator;

	class MasterDriver : public templates::parallel::MediatorClient
	{
	private:
		typedef templates::parallel::MediatorClient super;

		int uid;

	public:
		MasterDriver(Mediator *mediator, int port)
			: super((templates::external::base::ExternalSystemArrayBase*)mediator, "127.0.0.1", port)
		{
			uid = -1;
		};
		virtual ~MasterDriver() = default;

		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			super::sendData(invoke);

			if (uid != -1)
				getSystemArray<base::MasterBase>()->getMonitor()->reportSendData(uid, invoke);
		};
		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "set_master_uid")
				uid = invoke->front()->getValue<int>();
			else
				super::replyData(invoke);
		};
	};
};
};
};