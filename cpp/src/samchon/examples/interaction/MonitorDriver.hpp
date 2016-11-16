#pragma once
#include <samchon/protocol/IProtocol.hpp>

#include <samchon/examples/interaction/base/SystemBase.hpp>
#include <samchon/protocol/ServerConnector.hpp>

#include <samchon/templates/external/base/ExternalSystemArrayBase.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class MonitorDriver : public protocol::IProtocol
	{
	private:
		base::SystemBase *system;
		std::unique_ptr<protocol::ServerConnector> connector;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		MonitorDriver(base::SystemBase *system)
		{
			this->system = system;
			connector.reset(new protocol::ServerConnector(this));
		};
		virtual ~MonitorDriver() = default;

		void connect()
		{
			connector->connect("127.0.0.1", 37900);
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			connector->sendData(invoke);
		};
		void reportSendData(int to, std::shared_ptr<protocol::Invoke> invoke)
		{
			if (system->getUID() == -1)
				return;

			sendData(std::make_shared<protocol::Invoke>("reportSendData", to, invoke->getListener()));
		};
		void sendSystemStructure()
		{
			if (system->getUID() == -1)
				return;

			std::shared_ptr<library::XML> xml = system->toXML();

			auto system_array = dynamic_cast<templates::external::base::ExternalSystemArrayBase*>(system);
			if (system_array != nullptr)
			{
				auto children = system_array->_Get_children();
				for (size_t i = 0; i < children.size(); i++)
				{
					auto child = (base::SystemBase*)(children.at(i).get());
					if (child->getUID() == -1)
						return;
				}
			}

			xml->setTag("system");
			sendData(std::make_shared<protocol::Invoke>("construct", xml));
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "set_uid")
				system->setUID(invoke->front()->getValue<int>());
		};
	};
};
};
};