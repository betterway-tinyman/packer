#pragma once
#include <samchon/templates/external/ExternalSystem.hpp>

#include <samchon/examples/interaction/base/MonitorBase.hpp>
#include <samchon/examples/interaction/base/ReporterBase.hpp>
#include <samchon/examples/interaction/base/SystemBase.hpp>

#include <iostream>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class Reporter;

	class Viewer : public templates::external::ExternalSystem
	{
	private:
		typedef templates::external::ExternalSystem super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		Viewer(Reporter *reporter, std::shared_ptr<protocol::ClientDriver> driver)
			: super((templates::external::base::ExternalSystemArrayBase*)reporter, driver)
		{
			std::cout << "A viewer has connected" << std::endl;

			//--------
			// WHEN CONNECTED, THEN SEND SYSTEM STRUCTURE
			//--------
			System *system = ((base::MonitorBase*)(((base::ReporterBase*)reporter)->getMonitor()))->getRootSystem();
			if (system == nullptr)
				return;

			// SEND SYSTEM STRUCTURE OF ROOT.
			sendData(std::make_shared<protocol::Invoke>("setSystems", ((base::SystemBase*)system)->toXML()));
		};
		virtual ~Viewer()
		{
			std::cout << "A veiwer has disconnected" << std::endl;
		};

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> templates::external::ExternalSystemRole* override
		{ // NO ROLE
			return nullptr;
		};
	};
};
};
};