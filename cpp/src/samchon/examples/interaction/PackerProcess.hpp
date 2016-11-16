#pragma once
#include <samchon/templates/distributed/DistributedProcess.hpp>
#include <samchon/templates/distributed/base/DistributedSystemArrayBase.hpp>

#include <iostream>
#include <samchon/examples/packer/Packer.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class PackerProcess : public templates::distributed::DistributedProcess
	{
	private:
		typedef templates::distributed::DistributedProcess super;

	public:
		PackerProcess(templates::distributed::base::DistributedSystemArrayBase *systemArray)
			: super(systemArray)
		{
			this->name = "packer";
		};
		virtual ~PackerProcess() = default;

		void solve()
		{
			using namespace samchon::library;

			std::shared_ptr<packer::ProductArray> productArray(new packer::ProductArray
			({
				std::make_shared<packer::Product>("Eraser", 500, 10, 70),
				std::make_shared<packer::Product>("Eraser", 500, 10, 70),
				std::make_shared<packer::Product>("Pencil", 400, 30, 35),
				std::make_shared<packer::Product>("Book", 8000, 150, 300),
				std::make_shared<packer::Product>("Book", 8000, 150, 300),
				std::make_shared<packer::Product>("Drink", 1000, 75, 250),
				std::make_shared<packer::Product>("Drink", 1000, 75, 250),
				std::make_shared<packer::Product>("Drink", 1000, 75, 250),
				std::make_shared<packer::Product>("Umbrella", 4000, 200, 1000),
				std::make_shared<packer::Product>("Notebook-PC", 800000, 150, 850),
				std::make_shared<packer::Product>("Notebook-PC", 800000, 150, 850),
				std::make_shared<packer::Product>("Tablet-PC", 600000, 120, 450),
				std::make_shared<packer::Product>("Tablet-PC", 600000, 120, 450)
			}));

			std::unique_ptr<packer::Packer> packer(new packer::Packer(productArray));
			{
				packer->emplace_back(new packer::Wrapper("Large", 100, 200, 1000));
				packer->emplace_back(new packer::Wrapper("Medium", 70, 150, 500));
				packer->emplace_back(new packer::Wrapper("Small", 50, 100, 250));
			};

			sendData(std::make_shared<protocol::Invoke>("optimizePacker", packer->toXML()));
		};

		virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
		{
			if (invoke->getListener() == "replyOptimization")
				printSolution(invoke->front()->getValueAsXML());
		};

	private:
		void printSolution(std::shared_ptr<library::XML> xml)
		{
			std::unique_ptr<packer::Packer> packer(new packer::Packer());
			packer->construct(xml);

			std::cout << "The best packing solution: $" << packer->calcPrice() << std::endl;
		};
	};
};
};
};