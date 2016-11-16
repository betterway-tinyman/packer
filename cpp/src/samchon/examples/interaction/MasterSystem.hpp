#pragma once
#include <samchon/templates/distributed/DistributedSystem.hpp>
#include <samchon/examples/interaction/base/SystemBase.hpp>

#include <samchon/examples/interaction/base/ChiefBase.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Chief;

	class MasterSystem
		: public templates::distributed::DistributedSystem,
		public base::SystemBase
	{
	private:
		typedef templates::distributed::DistributedSystem super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		MasterSystem(Chief *chief, std::shared_ptr<protocol::ClientDriver> driver)
			: super((templates::external::base::ExternalSystemArrayBase*)chief, driver)
		{
		};

		virtual ~MasterSystem()
		{
			if (getUID() != -1)
				getSystemArray<base::ChiefBase>()->getMonitor()->sendSystemStructure();
		};

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			setUID(xml->getProperty<int>("uid"));

			super::construct(xml);
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			super::sendData(invoke);

			// NOTIFY SEND_DATA
			getSystemArray<base::ChiefBase>()->getMonitor()->reportSendData(getUID(), invoke);
		};
	};
};
};
};