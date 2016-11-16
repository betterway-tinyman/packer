#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>
#include <samchon/library/EventDispatcher.hpp>

#include <bws/packer/GAWrapperArray.hpp>
#include <bws/packer/WrapperGroup.hpp>
#include <bws/packer/WrapperVolume.hpp>

#include <random>
#include <samchon/library/GAParameters.hpp>
#include <samchon/library/GeneticAlgorithm.hpp>
#include <samchon/library/Event.hpp>
#include <samchon/library/ProgressEvent.hpp>
#include <bws/packer/PCKGeneticAlgorithm.hpp>

namespace bws
{
namespace packer
{
	class GAWrapperArray;
	class WrapperArray;
	class InstanceArray;

	/**
	 * @brief Packer, a solver of 3d bin packing with multiple wrappers.
	 *
	 * @details 
	 * <p> Packer is a facade class supporting packing operations in user side. You can solve a packing problem
	 * by constructing Packer class with {@link WrapperArray wrappers} and {@link InstanceArray instances} to 
	 * pack and executing Packer::optimize() method. </p>
	 *
	 * <p> During the optimization process by Packer::optimize(), ProgressEvent will be thrown. If you want
	 * to listen the ProgressEvent, register a listener function by #addEventListener() method. </p>
	 *
	 * <p> In background side, deducting packing solution, those algorithms are used. </p>
	 * <ul>
	 *	<li> <a href="http://betterwaysystems.github.io/packer/reference/AirForceBinPacking.pdf" target="_blank">
	 *		Airforce Bin Packing; 3D pallet packing problem: A human intelligence-based heuristic approach </a>
	 *	</li>
	 *	<li> Genetic Algorithm </li>
	 *	<li> Greedy and Back-tracking algorithm </li>
	 * </ul>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Packer
		: public protocol::Entity<>,
		public library::EventDispatcher
	{
	private:
		typedef protocol::Entity<> super;
		typedef library::EventDispatcher event_super;

	protected:
		/**
		 * @brief Type of Wrapper(s).
		 */
		std::shared_ptr<WrapperArray> wrapperArray;

		/**
		 * @brief Instance(s) to be packed (wrapped).
		 */
		std::shared_ptr<InstanceArray> instanceArray;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		Packer()
			: super(),
			event_super()
		{
			wrapperArray.reset(new WrapperArray());
			instanceArray.reset(new InstanceArray());
		};

		/**
		 * @brief Construct from members. 
		 *
		 * @param wrapperArray Candidate wrappers who can contain instances.
		 * @param instanceArray Instances to be packed into some wrappers.
		 */
		Packer(std::shared_ptr<WrapperArray> wrapperArray, std::shared_ptr<InstanceArray> instanceArray)
			: super(),
			event_super()
		{
			this->wrapperArray = wrapperArray;
			this->instanceArray = instanceArray;

			std::sort(wrapperArray->begin(), wrapperArray->end(), sortWrappers);
		};
		virtual ~Packer() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			wrapperArray->construct(xml->get(wrapperArray->TAG())->at(0));
			instanceArray->construct(xml->get(instanceArray->TAG())->at(0));

			std::sort(wrapperArray->begin(), wrapperArray->end(), sortWrappers);
		};

	public:
		/* -----------------------------------------------------------
			GETTERSE
		----------------------------------------------------------- */
		/**
		 * @brief Get Wrappers.
		 */
		auto getWrapperArray() const -> std::shared_ptr<WrapperArray>
		{
			return wrapperArray;
		};

		/**
		 * @brief Get Instances.
		 */
		auto getInstanceArray() const -> std::shared_ptr<InstanceArray>
		{
			return instanceArray;
		};

		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		/**
		 * @brief Compute an optimized packing solution.
		 *
		 * @details
		 * 
		 *
		 * @param gaParams Parameters of genetic algorithm.
		 */
		auto optimize(const library::GAParameters &gaParams = {500, 50, 20, .2}) const -> std::shared_ptr<WrapperArray>
		{
			// TO BE RETURNED
			std::shared_ptr<WrapperArray> wrappers(new WrapperArray());

			if (wrapperArray->size() == 1)
			{
				// ONLY A TYPE OF WRAPPER EXISTS,
				// OPTMIZE IN LEVEL OF WRAPPER_GROUP AND TERMINATE THE OPTIMIZATION
				std::shared_ptr<WrapperGroup> wrapperGroup(new WrapperGroup(wrapperArray->front()));

				// IF THERE'S AN INSTANCE CANNOT BE PACKED BY SIZE 
				// (AN INSTANCE IS GREATER THAN THE WRAPPER)
				for (size_t i = 0; i < instanceArray->size(); i++)
					if (wrapperGroup->allocate(instanceArray->at(i)) == false)
						return wrappers;

				// FETCH THE OPTIMIZATION RESULT
				wrapperGroup->optimize();
				wrappers->assign(wrapperGroup->begin(), wrapperGroup->end());
			}
			else
			{
				////////////////////////////////////////
				// WITH GENETIC_ALGORITHM
				////////////////////////////////////////
				// CONSTRUCT INITIAL SET
				std::shared_ptr<GAWrapperArray> geneArray = initGenes();
				//shared_ptr<GAPopulation<GAWrapperArray>> population(new GAPopulation<GAWrapperArray>(geneArray, gaParams.getPopulation()));

				//// EVOLVE
				//PCKGeneticAlgorithm geneticAlgorithm
				//(
				//	gaParams.getMutationRate(), 
				//	gaParams.getTournament(),
				//	wrapperArray // CANDIDATES
				//);

				//size_t generation = gaParams.getGeneration();
				//for (size_t i = 0; i < generation; i++)
				//{
				//	population = geneticAlgorithm.evolvePopulation(population);

				//	shared_ptr<Event> event(new ProgressEvent(nullptr, i + 1, generation));
				//	((Packer*)this)->dispatch(event);
				//}

				//geneArray = population->fitTest();

				// FETCH RESULT AND
				auto &result = geneArray->getResult();
				for (auto it = result.begin(); it != result.end(); it++)
					wrappers->insert(wrappers->end(), it->second->begin(), it->second->end());

				// DO THE POST-PROCESS
				wrappers = repack(wrappers);
			}

			// SORT THE WRAPPERS BY ITEMS' POSITION
			for (size_t i = 0; i < wrappers->size(); i++)
				sort(wrappers->at(i)->begin(), wrappers->at(i)->end(),
					[](const std::shared_ptr<Wrap> &left, const std::shared_ptr<Wrap> &right) -> bool
					{
						if (left->getZ() != right->getZ())
							return left->getZ() < right->getZ();
						else if (left->getY() != right->getY())
							return left->getY() < right->getY();
						else
							return left->getX() < right->getX();
					});

			// RETURN WRAPPERS HAVE PACKED.
			return wrappers;
		};

	protected:
		/**
		 * @brief Initialize sequence list (gene_array).
		 *
		 * @details
		 * <p> Deducts initial sequence list by such assumption: </p>
		 * 
		 * <ul>
		 *	<li> Cost of larger wrapper is less than smaller one, within framework of price per volume unit. </li>
		 *	<ul>
		 *		<li> Wrapper Larger: (price: $1,000, volume: 100cm^3 -> price per volume unit: $10 / cm^3) </li>
		 *		<li> Wrapper Smaller: (price: $700, volume: 50cm^3 -> price per volume unit: $14 / cm^3) </li>
		 *		<li> Larger's <u>cost</u> is less than Smaller, within framework of price per volume unit </li>
		 *	</ul>
		 * </ul>
		 *
		 * <p> Method #initGenes() construct {@link WrapperGroup WrapperGroups} corresponding with #wrapperArray
		 * and allocates {@link #instanceArray instances} to a WrapperGroup, has the smallest <u>cost</u> between 
		 * containbles. </p>
		 *
		 * <p> After executing packing solution by WrapperGroup::optimize(), trying to re-pack each WrapperGroup
		 * to another type of Wrapper, deducts the best solution between them. It's the initial sequence list
		 * of genetic algorithm. </p>
		 *
		 * @return Initial sequence list.
		 */
		virtual auto initGenes() const -> std::shared_ptr<GAWrapperArray>
		{
			using namespace std;

			////////////////////////////////////////////////////
			// LINEAR OPTIMIZATION
			////////////////////////////////////////////////////
			// CONSTRUCT WRAPPER_GROUPS
			vector<shared_ptr<WrapperGroup>> wrapperGroups;
	
			for (size_t i = 0; i < wrapperArray->size(); i++)
			{
				const shared_ptr<Wrapper> &wrapper = wrapperArray->at(i);

				wrapperGroups.emplace_back(new WrapperGroup(wrapper));
			}

			// CONSTRUCT VOLUMES
			vector<WrapperVolume> wrapperVolumes;
			wrapperVolumes.reserve(wrapperArray->size());

			for (size_t i = 0; i < wrapperArray->size(); i++)
				wrapperVolumes.emplace_back(wrapperArray->at(i));

			// allocate instances by autorhity
			for (size_t i = 0; i < instanceArray->size(); i++)
			{
				shared_ptr<Instance> instance = instanceArray->at(i);
				double minProprity = INT_MAX;
				size_t minIndex = 0;
		
				for (size_t j = 0; j < wrapperVolumes.size(); j++)
				{
					const auto &wrapper = wrapperArray->at(j);
					auto &volume = wrapperVolumes.at(j);

					if (wrapper->operator>=(*instance) == false)
						continue;

					double priority = volume.getPriority(instance);
					if (priority < minProprity)
					{
						minIndex = j;
						minProprity = priority;
					}
				}

				shared_ptr<WrapperGroup> &wrapperGroup = wrapperGroups.at(minIndex);
				WrapperVolume &volume = wrapperVolumes.at(minIndex);

				if (volume.allocate(instance) == false)
				{
					volume = WrapperVolume(wrapperGroup->getSample());
					volume.allocate(instance);
				}

				wrapperGroup->allocate(instance, 1);
			}


			////////////////////////////////////////////////////
			// ADDICTIONAL OPTIMIZATION BY POST-PROCESS
			////////////////////////////////////////////////////
			// OPTIMIZE WRAPPER_GROUP
			shared_ptr<WrapperArray> wrappers(new WrapperArray());

			for (size_t i = 0; i < wrapperGroups.size(); i++)
			{
				// OPTIMIZE AND ASSIGN THE GROUP'S
				shared_ptr<WrapperGroup> &wrapperGroup = wrapperGroups.at(i);

				wrapperGroup->optimize();
				wrappers->insert(wrappers->end(), wrapperGroup->begin(), wrapperGroup->end());
			}

			// DO EARLY POST-PROCESS
			wrappers = repack(wrappers);

			////////////////////////////////////////////////////
			// CONSTRUCT GENE_ARRAY
			////////////////////////////////////////////////////
			// INSTANCES AND GENES
			shared_ptr<InstanceArray> ga_instances(new InstanceArray());
			shared_ptr<WrapperArray> genes(new WrapperArray());

			for (size_t i = 0; i < wrappers->size(); i++)
			{
				auto &wrapper = wrappers->at(i);

				for (size_t j = 0; j < wrapper->size(); j++)
				{
					ga_instances->push_back(wrapper->at(j)->getInstance());
					genes->push_back(wrapper);
				}
			}

			// GENE_ARRAY
			shared_ptr<GAWrapperArray> geneArray(new GAWrapperArray(ga_instances));
			geneArray->assign(genes->begin(), genes->end());

			return geneArray;
		};

		/**
		 * @brief Try to repack each wrappers to another type.
		 *
		 * @details Re-packs instances who are packed in the $wrappers to another type of Wrappers. 
		 *			A type of Wrapper with the smallest cost will be returned, containing the instances.
		 *
		 * @param $wrappers Wrappers to repack.
		 * @return Re-packed wrappers.
		 */
		virtual auto repack(const std::shared_ptr<WrapperArray> $wrappers) const -> std::shared_ptr<WrapperArray>
		{
			using namespace std;

			shared_ptr<WrapperArray> result(new WrapperArray());

			for (size_t i = 0; i < $wrappers->size(); i++)
			{
				shared_ptr<Wrapper> wrapper = $wrappers->at(i);
				shared_ptr<WrapperGroup> minGroup(new WrapperGroup(wrapper));

				minGroup->push_back(wrapper);

				for (size_t j = 0; j < this->wrapperArray->size(); j++)
				{
					shared_ptr<Wrapper> myWrapper = this->wrapperArray->at(j);

					if (wrapper->operator==(*myWrapper))
						continue;

					bool valid = true;

					// 타깃 타입의 Wrapper에 대해 Group을 구성
					shared_ptr<WrapperGroup> myGroup(new WrapperGroup(myWrapper));
					for (size_t k = 0; k < wrapper->size(); k++)
						if (myGroup->allocate(wrapper->at(k)->getInstance(), 1) == false)
						{
							// 너무 커서 입력할 수 없는 제품이 있으면
							valid = false;
							break;
						}

					// 제낀다
					if (valid == false)
						continue;

					// 그룹 단위의 최적화 실시
					myGroup->optimize();

					// 현재의 그룹셋이 더 싸다면, 교체함
					if (myGroup->getPrice() < minGroup->getPrice())
						minGroup = myGroup;
				}

				result->insert(result->end(), minGroup->begin(), minGroup->end());
			}

			return result;
		};

	private:
		static auto sortWrappers(const std::shared_ptr<Wrapper> &left, const std::shared_ptr<Wrapper> &right) -> bool
		{
			return left->getVolume() > right->getVolume();
		};

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const-> std::string override
		{
			return "packer";
		};
		
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = super::toXML();
			xml->push_back(wrapperArray->toXML());
			xml->push_back(instanceArray->toXML());

			return xml;
		};
	};
};
};