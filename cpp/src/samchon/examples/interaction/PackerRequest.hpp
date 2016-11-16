#pragma once

#include <iostream>
#include <samchon/examples/interaction/base/MasterBase.hpp>
#include <samchon/examples/packer/Packer.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
	class PackerRequest
	{
	private:
		base::MasterBase *master;

		std::shared_ptr<packer::Packer> best_packer;
		size_t requested_size;
		size_t completed_count;

	public:
		PackerRequest(base::MasterBase *master)
		{
			this->master = master;
		};
		void init(size_t size)
		{
			best_packer = nullptr;
			this->completed_count = 0;
			this->requested_size = size;
		};

		void optimize(std::shared_ptr<library::XML> xml)
		{
			std::shared_ptr<packer::Packer> packer(new packer::Packer());
			packer->construct(xml);

			auto invoke = std::make_shared<protocol::Invoke>("optimizePacker", packer->toXML());
			size_t size = library::CombinedPermutationGenerator(packer->size(), packer->productSize()).size();

			std::cout << "Start Packer optimization: #" << size << std::endl;
			init(size);
		};
		void replyOptimization(std::shared_ptr<library::XML> xml)
		{
			std::shared_ptr<packer::Packer> packer(new packer::Packer());
			packer->construct(xml);

			std::cout << "A slave has finished his optimization: $" << packer->calcPrice() << std::endl;

			// IF CURRENT TRAVEL IS SHORTER, MAKE IT THE BEST
			if (best_packer == nullptr || best_packer->calcPrice() == 0 ||
				packer->calcPrice() < best_packer->calcPrice())
			{
				std::cout << "The slave renewed the best solution" << std::endl;
				best_packer = packer;
			}

			// IF ALL TASKS ARE DONE, REPORT TO THE CHIEF
			if (++completed_count == requested_size)
				master->getParent()->sendData
				(
					std::make_shared<protocol::Invoke>("replyPackerOptimization", best_packer->toXML())
				);
		};
	};
};
};
};