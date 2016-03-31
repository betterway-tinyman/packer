#include <bws/packer/WrapperArray.hpp>

#include <array>
#include <utility>
#include <samchon/library/GeneticAlgorithm.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
auto WrapperArray::createChild(shared_ptr<XML>) -> Wrapper*
{
	return new Wrapper();
}

/* -----------------------------------------------------------
	GETTER
----------------------------------------------------------- */
auto WrapperArray::getPrice() const -> double
{
	double price = 0.0;
	for (size_t i = 0; i < size(); i++)
		price += at(i)->getPrice();

	return price;
}
auto WrapperArray::getUtilization() const -> double
{
	if (this->empty() == true)
		return 0.0;

	double numerator = 0.0;
	double denominator = 0.0;

	for (size_t i = 0; i < size(); i++)
	{
		const auto &wrapper = at(i);

		for (size_t j = 0; j < wrapper->size(); j++)
			numerator += wrapper->at(j)->getVolume();

		denominator += wrapper->getContainableVolume();
	}

	return numerator / denominator;
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto WrapperArray::TAG() const -> string
{
	return "wrapperArray";
}
auto WrapperArray::CHILD_TAG() const -> string
{
	return "instance";
}