#include <bws/packer/Packer.hpp>

#include <bws/packer/InstanceArray.hpp>

#include <bws/packer/GAWrapperArray.hpp>
#include <bws/packer/WrapperGroup.hpp>
#include <bws/packer/WrapperVolume.hpp>

#include <random>
#include <samchon/library/GeneticAlgorithm.hpp>
#include <samchon/library/Event.hpp>
#include <samchon/library/ProgressEvent.hpp>
#include <bws/packer/PCKGeneticAlgorithm.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

auto sortWrappers(const std::shared_ptr<Wrapper> &left, const std::shared_ptr<Wrapper> &right) -> bool
{
	return left->getVolume() > right->getVolume();
}

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
Packer::Packer()
	: super(),
	event_super()
{
	wrapperArray.reset(new WrapperArray());
	instanceArray.reset(new InstanceArray());
}
Packer::Packer(shared_ptr<WrapperArray> wrapperArray, shared_ptr<InstanceArray> instanceArray)
	: super(),
	event_super()
{
	this->wrapperArray = wrapperArray;
	this->instanceArray = instanceArray;

	sort(wrapperArray->begin(), wrapperArray->end(), sortWrappers);
}

void Packer::construct(shared_ptr<XML> xml)
{
	wrapperArray->construct(xml->get(wrapperArray->TAG())->at(0));
	instanceArray->construct(xml->get(instanceArray->TAG())->at(0));

	sort(wrapperArray->begin(), wrapperArray->end(), sortWrappers);
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto Packer::getWrapperArray() const -> shared_ptr<WrapperArray>
{
	return wrapperArray;
}
auto Packer::getInstanceArray() const -> shared_ptr<InstanceArray>
{
	return instanceArray;
}

/* -----------------------------------------------------------
	OPERATORS
----------------------------------------------------------- */
auto Packer::optimize(const GAParameters &gaParams) const -> shared_ptr<WrapperArray>
{
	// TO BE RETURNED
	shared_ptr<WrapperArray> wrappers(new WrapperArray());

	if (wrapperArray->size() == 1)
	{
		// ONLY A TYPE OF WRAPPER EXISTS,
		// OPTMIZE IN LEVEL OF WRAPPER_GROUP AND TERMINATE THE OPTIMIZATION
		shared_ptr<WrapperGroup> wrapperGroup(new WrapperGroup(wrapperArray->front()));

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
		shared_ptr<GAWrapperArray> geneArray = initGenes();
		shared_ptr<GAPopulation<GAWrapperArray>> population(new GAPopulation<GAWrapperArray>(geneArray, gaParams.getPopulation()));

		// EVOLVE
		PCKGeneticAlgorithm geneticAlgorithm
		(
			gaParams.getMutationRate(), 
			gaParams.getTournament(),
			wrapperArray // CANDIDATES
		);

		size_t generation = gaParams.getGeneration();
		for (size_t i = 0; i < generation; i++)
		{
			population = geneticAlgorithm.evolvePopulation(population);

			shared_ptr<Event> event(new ProgressEvent(nullptr, i + 1, generation));
			((Packer*)this)->dispatch(event);
		}

		geneArray = population->fitTest();

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
			[](const shared_ptr<Wrap> &left, const shared_ptr<Wrap> &right) -> bool
			{
				if (left->getZ() != right->getZ())
					return left->getZ() < right->getZ();
				else if (left->getY() != right->getY())
					return left->getY() < right->getY();
				else
					return left->getX() < right->getX();
			});
}

auto Packer::initGenes() const -> shared_ptr<GAWrapperArray>
{
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
}

auto Packer::repack(shared_ptr<WrapperArray> $wrappers) const -> shared_ptr<WrapperArray>
{
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
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto Packer::TAG() const -> string
{
	return "packer";
}

auto Packer::toXML() const -> shared_ptr<XML>
{
	shared_ptr<XML> &xml = super::toXML();
	xml->push_back(wrapperArray->toXML());
	xml->push_back(instanceArray->toXML());

	return xml;
}