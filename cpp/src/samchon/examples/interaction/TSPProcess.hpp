#pragma once
#include <samchon/templates/distributed/DistributedProcess.hpp>
#include <samchon/templates/distributed/base/DistributedSystemArrayBase.hpp>

#include <iostream>
#include <samchon/library/Math.hpp>
#include <samchon/examples/tsp/Travel.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class TSPProcess : public templates::distributed::DistributedProcess
	{
	private:
		typedef templates::distributed::DistributedProcess super;

	public:
		TSPProcess(templates::distributed::base::DistributedSystemArrayBase *systemArray) 
			: super(systemArray)
		{
			this->name = "tsp";
		};
		virtual ~TSPProcess() = default;

		void solve()
		{
			using namespace samchon::library;

			std::unique_ptr<tsp::Travel> travel(new tsp::Travel());
			for (size_t i = 0; i < 9; i++)
				travel->emplace_back(new tsp::GeometryPoint(i+1, Math::random(), Math::random()));

			sendData(std::make_shared<protocol::Invoke>("optimizeTSP", travel->toXML()));
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "replyOptimization")
				printSolution(invoke->front()->getValueAsXML());
		};

	private:
		void printSolution(std::shared_ptr<library::XML> xml)
		{
			std::unique_ptr<tsp::Travel> travel(new tsp::Travel());
			travel->construct(xml);

			std::cout << "The best travel: " << travel->calcDistance() << " km" << std::endl;
		};
	};
};
};
};