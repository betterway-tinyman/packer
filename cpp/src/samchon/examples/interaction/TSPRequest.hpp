#pragma once

#include <iostream>
#include <samchon/examples/interaction/base/MasterBase.hpp>

#include <samchon/examples/tsp/Travel.hpp>
#include <samchon/library/FactorialGenerator.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class TSPRequest
	{
	private:
		base::MasterBase *master;

		std::shared_ptr<tsp::Travel> best_travel;
		size_t requested_size;
		size_t completed_count;

	public:
		TSPRequest(base::MasterBase *master)
		{
			this->master = master;
		};
		void init(size_t size)
		{
			best_travel = nullptr;
			this->completed_count = 0;
			this->requested_size = size;
		};

		void optimize(std::shared_ptr<library::XML> xml)
		{
			std::shared_ptr<tsp::Travel> travel(new tsp::Travel());
			travel->construct(xml);

			auto invoke = std::make_shared<protocol::Invoke>("optimizeTSP", travel->toXML());
			size_t size = library::FactorialGenerator(travel->size()).size();

			std::cout << "Start Packer optimization: #" << size << std::endl;
			init(size);
		};
		void replyOptimization(std::shared_ptr<library::XML> xml)
		{
			std::shared_ptr<tsp::Travel> travel(new tsp::Travel());
			travel->construct(xml);

			std::cout << "A slave has finished his optimization: " << travel->calcDistance() << " km" << std::endl;

			// IF CURRENT TRAVEL IS SHORTER, MAKE IT THE BEST
			if (best_travel == nullptr || best_travel->calcDistance() == 0 ||
				travel->calcDistance() < best_travel->calcDistance())
			{
				std::cout << "The slave renewed the best solution" << std::endl;
				best_travel = travel;
			}

			// IF ALL TASKS ARE DONE, REPORT TO THE CHIEF
			if (++completed_count == requested_size)
				master->getParent()->sendData
				(
					std::make_shared<protocol::Invoke>("replyTSPOptimization", best_travel->toXML())
				);
		};
	};
};
};
};