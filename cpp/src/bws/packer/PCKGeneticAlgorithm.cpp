#include <bws/packer/PCKGeneticAlgorithm.hpp>

#include <bws/packer/GAWrapperArray.hpp>
#include <bws/packer/WrapperArray.hpp>

using namespace std;
using namespace samchon::library;
using namespace bws::packer;

PCKGeneticAlgorithm::PCKGeneticAlgorithm(double mutate, size_t tournament, shared_ptr<WrapperArray> candidates)
	: super(false, mutate, tournament)
{
	this->candidates = candidates;
}

void PCKGeneticAlgorithm::mutate(shared_ptr<GAWrapperArray> individual) const
{
	for (size_t i = 0; i < individual->size(); i++)
	{
		if (Math::random() > mutationRate)
			continue;

		size_t index = (size_t)(Math::random() * candidates->size());
		individual->at(i) = candidates->at(index);
	}
}