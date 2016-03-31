#include <bws/packer/WrapperGroup.hpp>

#include <bws/packer/Wrapper.hpp>
#include <bws/packer/InstanceArray.hpp>

#include <boxologic/Boxologic.hpp>

#include <array>
#include <algorithm>

using namespace boxologic;
using namespace bws::packer;

using namespace std;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
WrapperGroup::WrapperGroup()
	: super()
{
	sample = nullptr;
	allocatedInstanceArray.reset(new InstanceArray());
}
WrapperGroup::WrapperGroup
	(
		const string &name, double price,
		double width, double height, double length, double thickness = 0.0
	) : WrapperGroup()
{
	sample.reset(new Wrapper(name, price, width, height, length, thickness));
}
WrapperGroup::WrapperGroup(shared_ptr<Wrapper> sample)
	: WrapperGroup()
{
	this->sample = sample;
}

void WrapperGroup::construct(shared_ptr<XML> xml)
{
	super::construct(xml);

	if (xml->hasProperty("width"))
	{
		if (sample == nullptr)
			sample.reset(new Wrapper());

		sample->construct(xml);
	}
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto WrapperGroup::key() const -> string
{
	if (sample == nullptr)
		return "";
	else
		return sample->key();
}
auto WrapperGroup::getSample() const -> shared_ptr<Wrapper>
{
	return sample;
}
auto WrapperGroup::getAllocatedInstanceArray() const -> shared_ptr<InstanceArray>
{
	return allocatedInstanceArray;
}

auto WrapperGroup::getPrice() const -> double
{
	if (sample == nullptr)
		return 0.0;
	else
		return sample->getPrice() * size();
}
auto WrapperGroup::getUtilization() const -> double
{
	double utilization = 0.0;
	for (size_t i = 0; i < size(); i++)
		utilization += at(i)->getUtilization();

	return utilization / (double)size();
}

/* -----------------------------------------------------------
	OPERATORS
----------------------------------------------------------- */
auto WrapperGroup::allocate(shared_ptr<Instance> instance, size_t n) -> bool
{
	// TEST WHETHER A PRODUCT IS NOT LARGER THAN BOX
	if (sample->operator>=(*instance) == false)
		return false;

	// INSERTS TO THE RESERVED ITEMS
	this->allocatedInstanceArray->insert
	(
		allocatedInstanceArray->end(),
		n, instance
	);

	return true;
}

void WrapperGroup::optimize()
{
	// CLEAR PREVIOUS OPTIMIZATION
	this->clear();

	// CONSTRUCT INSTANCES FROM RESERVEDS
	shared_ptr<InstanceArray> instanceArray = this->allocatedInstanceArray;

	// UNTIL UNPACKED INSTANCE DOES NOT EXIST
	while (instanceArray->empty() == false)
		instanceArray = this->pack(instanceArray);
}

auto WrapperGroup::pack(shared_ptr<InstanceArray> instanceArray) -> shared_ptr<InstanceArray>
{
	Boxologic adaptor(make_shared<Wrapper>(*sample), instanceArray);
	auto pair = adaptor.pack();

	this->push_back(pair.first);
	return pair.second;
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto WrapperGroup::TAG() const -> string
{
	return "wrapperGroup";
}

auto WrapperGroup::toXML() const -> shared_ptr<XML>
{
	shared_ptr<XML> &xml = super::toXML();

	if (sample != nullptr)
	{
		xml->addAllProperty(sample->toXML());

		xml->eraseProperty("type");
		xml->setProperty("price", getPrice());
		xml->setProperty("utilization", getUtilization());
	}

	return xml;
}