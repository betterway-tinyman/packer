#pragma once
#include <samchon/templates/external/ExternalClientArray.hpp>
#	include <samchon/examples/interaction/Viewer.hpp>
#include <samchon/examples/interaction/base/ReporterBase.hpp>

#include <samchon/examples/interaction/base/MonitorBase.hpp>
#include <samchon/examples/interaction/base/SystemBase.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Monitor;

	class Reporter 
		: public templates::external::ExternalClientArray<Viewer>,
		public base::ReporterBase
	{
	private:
		typedef templates::external::ExternalClientArray<Viewer> super;

		Monitor *monitor;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Reporter(Monitor *monitor) : super()
		{
			this->monitor = monitor;
		};
		virtual ~Reporter() = default;

	protected:
		virtual auto createExternalClient(std::shared_ptr<protocol::ClientDriver> driver) -> Viewer* override
		{
			return new Viewer(this, driver);
		};

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto getMonitor() const -> Monitor* override
		{
			return monitor;
		};

		/* ---------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendSystems() override final
		{
			System *root = ((base::MonitorBase*)monitor)->getRootSystem();
			if (root == nullptr)
				return;

			sendData(std::make_shared<protocol::Invoke>("setSystems", ((base::SystemBase*)root)->toXML()));
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke>)
		{
			// NOTHING TO DO
		};
	};
};
};
};