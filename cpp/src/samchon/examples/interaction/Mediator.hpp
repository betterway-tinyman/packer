#pragma once
#include <samchon/templates/parallel/ParallelClientArrayMediator.hpp>
#include <samchon/examples/interaction/SlaveDriver.hpp>
#include <samchon/examples/interaction/base/MasterBase.hpp>

#include <samchon/examples/interaction/MasterDriver.hpp>
#include <samchon/examples/interaction/MonitorDriver.hpp>
#include <samchon/examples/interaction/ChiefDriver.hpp>

#include <samchon/examples/interaction/TSPRequest.hpp>
#include <samchon/examples/interaction/PackerRequest.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Mediator
		: public virtual templates::parallel::ParallelClientArrayMediator<SlaveDriver>,
		public base::MasterBase
	{
	private:
		typedef templates::parallel::ParallelClientArrayMediator<SlaveDriver> super;

		std::unique_ptr<ChiefDriver> chief;

		std::string name;
		int master_port;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Mediator(int masterPort) : super()
		{
			this->name = "Mediator";
			this->master_port = masterPort;

			chief.reset(new ChiefDriver(this));
			monitor.reset(new MonitorDriver(this));

			tsp.reset(new TSPRequest(this));
			packer.reset(new PackerRequest(this));
		};
		virtual ~Mediator() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			setUID(xml->getProperty<int>("uid"));
			name = xml->getProperty<std::string>("name");

			super::construct(xml);
		};

	protected:
		virtual auto createMediator() -> templates::parallel::MediatorSystem*
		{
			return new MasterDriver(this, master_port);
		};

		virtual auto createExternalClient(std::shared_ptr<protocol::ClientDriver> driver) -> SlaveDriver*
		{
			return new SlaveDriver(this, driver);
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto getParent() const -> protocol::IProtocol*
		{
			return getMediator();
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke)
		{
			super::sendData(invoke);
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke)
		{
			if (invoke->getListener() == "set_uid")
				set_uid(invoke->front()->getValue<int>());
		};

	private:
		void set_uid(int val)
		{
			setUID(val);

			// NOTIFY MY ID TO SLAVES
			sendData(std::make_shared<protocol::Invoke>("set_master_uid", val));

			// NOTIFY MY UID TO MASTER
			// Call Master::SlaveSystem::construct()
			auto xml = toXML();
			{
				xml->clear();
				xml->setTag("system");
			}
			getParent()->sendData(std::make_shared<protocol::Invoke>("construct", xml));
		};

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string
		{
			return super::TAG();
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML>
		{
			auto xml = super::toXML();
			xml->setProperty("uid", getUID());
			xml->setProperty("name", name);

			return xml;
		};

		/* ---------------------------------------------------------
			MAIN
		--------------------------------------------------------- */
		static void main(int masterPort, int myPort)
		{
			Mediator mediator(masterPort);
			mediator.open(myPort);
		};
	};
};
};
};