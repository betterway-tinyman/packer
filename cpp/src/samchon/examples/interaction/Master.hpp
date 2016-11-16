#pragma once
#include <samchon/templates/parallel/ParallelClientArray.hpp>
#include <samchon/examples/interaction/SlaveDriver.hpp>
#include <samchon/examples/interaction/base/MasterBase.hpp>

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
	class Master
		: public virtual templates::parallel::ParallelClientArray<SlaveDriver>,
		public base::MasterBase
	{
	private:
		typedef templates::parallel::ParallelClientArray<SlaveDriver> super;

		std::unique_ptr<ChiefDriver> chief;

		std::string name;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Master() : super()
		{
			name = "Master";

			chief.reset(new ChiefDriver(this));
			monitor.reset(new MonitorDriver(this));

			tsp.reset(new TSPRequest(this));
			packer.reset(new PackerRequest(this));
		};
		virtual ~Master() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			setUID(xml->getProperty<int>("uid"));
			name = xml->getProperty<std::string>("name");

			super::construct(xml);
		};

		virtual void open(int port) override
		{
			std::thread(&ChiefDriver::connect, chief.get(), "127.0.0.1", 37000).detach();
			std::thread(&MonitorDriver::connect, monitor.get()).detach();

			super::open(port);
		};

	protected:
		virtual auto createExternalClient(std::shared_ptr<protocol::ClientDriver> driver) -> SlaveDriver*
		{
			return new SlaveDriver(this, driver);
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto getParent() const -> protocol::IProtocol* override
		{
			return chief.get();
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

	protected:
		virtual auto _Complete_history(std::shared_ptr<templates::InvokeHistory> history) -> bool override
		{
			bool ret = super::_Complete_history(history);

			if (ret == true && p_invoke_queue.empty() == false)
			{
				// COMPLETE A HISTORY AND POP IT.
				p_invoke_queue.front()->complete();
				p_invoke_queue.pop();
			}
			return ret;
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
			STATIC MAIN
		--------------------------------------------------------- */
		static void main(int port)
		{
			Master master;
			master.open(port);
		};
	};
};
};
};