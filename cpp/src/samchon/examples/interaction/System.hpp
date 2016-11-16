#pragma once
#include <deque>
#include <samchon/protocol/EntityGroup.hpp>
#include <samchon/examples/interaction/base/SystemBase.hpp>

#include <samchon/protocol/ClientDriver.hpp>
#include <samchon/examples/interaction/base/ReporterBase.hpp>
#include <samchon/examples/interaction/base/MonitorBase.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Monitor;

	class System 
		: public protocol::EntityGroup<std::deque<System*>, System, int>,
		public base::SystemBase
	{
	private:
		typedef protocol::EntityGroup<std::deque<System*>, System, int> super;

		// MONITOR AND PARENT (HIERARCHY) OBJECTS
		Monitor *monitor;
		System *parent;

		// COMMUNICATOR
		std::shared_ptr<protocol::ClientDriver> driver;

		// ATOMIC MEMBERS
		std::string name;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		System(Monitor *monitor, std::shared_ptr<protocol::ClientDriver> driver, int uid)
			: super()
		{
			this->monitor = monitor;
			this->parent = nullptr;

			this->driver = driver;
			this->setUID(uid);
			this->name = "";

			sendData(std::make_shared<protocol::Invoke>("set_uid", uid));
		};

		virtual ~System()
		{
			// ERASE THIS OBJECT FROM MAP AND PARENT
			auto &system_map = ((base::MonitorBase*)monitor)->getSystems();
			system_map.erase(getUID());

			if (parent != nullptr)
				for (size_t i = 0; i < parent->size(); i++)
					if (parent->at(i) == this)
					{
						parent->erase(parent->begin() + i);
						break;
					}

			// LET VIEWERS TO SEND SYSTEM STRUCTURE AGAIN
			base::ReporterBase *reporter = (base::ReporterBase*)(((base::MonitorBase*)monitor)->getReporter());
			reporter->sendSystems();
		};

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			setUID(xml->getProperty<int>("uid"));
			name = xml->getProperty<std::string>("name");

			super::construct(xml);
		};

	protected:
		virtual auto createChild(std::shared_ptr<library::XML> xml) -> System*
		{
			auto &system_map = ((base::MonitorBase*)monitor)->getSystems();
			int uid = xml->getProperty<int>("uid");

			if (system_map.has(uid) == false)
				return nullptr; // NOT EXISTS
			else
			{
				System *system = system_map.get(uid);
				system->parent = this;

				return system;
			}
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto key() const -> int override
		{
			return getUID();
		};

		auto getParent() const -> System*
		{
			return parent;
		};

		void setParent(System *val)
		{
			parent = val;
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			driver->sendData(invoke);
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "construct")
			{
				construct(invoke->front()->getValueAsXML());
				((base::MonitorBase*)monitor)->constructSystemTree();

				// LET VIEWERS TO SEND SYSTEM STRUCTURE
				Reporter *reporter = ((base::MonitorBase*)monitor)->getReporter();
				((base::ReporterBase*)reporter)->sendSystems();

				System *root = ((base::MonitorBase*)monitor)->getRootSystem();
				std::cout << root->toXML()->toString() << std::endl;

				return;
			}
			else if (invoke->getListener() == "reportSendData")
			{
				std::string listener = invoke->at(0)->getValue<std::string>();
				int to = invoke->at(1)->getValue<int>();
				int from = invoke->at(2)->getValue<int>();

				Reporter *reporter = ((base::MonitorBase*)monitor)->getReporter();
				auto invoke = std::make_shared<protocol::Invoke>("printSendData", listener, from, to);

				((protocol::IProtocol*)reporter)->sendData(invoke);
			}
			std::cout << invoke->toXML()->toString() << std::endl;
		};

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "System";
		};
		virtual auto CHILD_TAG() const -> std::string override
		{
			return TAG();
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML>
		{
			std::shared_ptr<library::XML> xml = super::toXML();
			xml->setProperty("uid", getUID());
			xml->setProperty("name", name);

			return xml;
		};
	};
};
};
};