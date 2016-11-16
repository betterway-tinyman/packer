#pragma once
#include <samchon/templates/distributed/DistributedClientArray.hpp>
#	include <samchon/examples/interaction/MasterSystem.hpp>
#include <samchon/examples/interaction/base/ChiefBase.hpp>

#include <samchon/examples/interaction/TSPProcess.hpp>
#include <samchon/examples/interaction/PackerProcess.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Chief
		: public virtual templates::distributed::DistributedClientArray<MasterSystem>,
		public base::ChiefBase
	{
	private:
		typedef templates::distributed::DistributedClientArray<MasterSystem> super;
		
	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Chief() : super()
		{
			monitor.reset(new MonitorDriver(this));

			insertProcess(std::make_shared<TSPProcess>(this));
			insertProcess(std::make_shared<PackerProcess>(this));
		};
		virtual ~Chief() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			setUID(xml->getProperty<int>("uid"));

			super::construct(xml);
		};

		virtual void open(int port) override
		{
			std::thread(&MonitorDriver::connect, monitor.get()).detach();

			super::open(port);
		};

	protected:
		virtual auto createProcess(std::shared_ptr<library::XML> xml) -> templates::distributed::DistributedProcess* override
		{
			if (xml->getProperty("name") == "tsp")
				return new TSPProcess(this);
			else
				return new PackerProcess(this);
		};

		virtual auto createExternalClient(std::shared_ptr<protocol::ClientDriver> driver) -> MasterSystem* override
		{
			return new MasterSystem(this, driver);
		};

	public:
		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			super::sendData(invoke);
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "set_uid")
				set_uid(invoke->front()->getValue<size_t>());
		};

	private:
		void set_uid(int val)
		{
			setUID(val);

			// NOTIFY MY ID TO SLAVES (MASTER SYSTEMS)
			sendData(std::make_shared<protocol::Invoke>("set_chief_uid", getUID()));
		};

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return super::TAG();
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = super::toXML();
			xml->setProperty("uid", getUID());
			xml->setProperty("name", "chief");

			return xml;
		};

		/* ---------------------------------------------------------
			STATIC MAIN
		--------------------------------------------------------- */
		static void main()
		{
			Chief chief;
			std::thread(&Chief::open, &chief, 37000).detach();

			while (true)
			{
#ifdef _WIN32
				system("cls");
#else
				system("clear");
#endif

				std::cout << "Select what to do." << std::endl;
				std::cout << "	1. Solve TSP" << std::endl;
				std::cout << "	2. Solve Packer" << std::endl;

				int no = std::cin.get();
				if (no == 1)
					std::dynamic_pointer_cast<TSPProcess>(chief.getProcess("tsp"))->solve();
				else if (no == 2)
					std::dynamic_pointer_cast<PackerProcess>(chief.getProcess("packer"))->solve();
			}
		};
	};
};
};
};