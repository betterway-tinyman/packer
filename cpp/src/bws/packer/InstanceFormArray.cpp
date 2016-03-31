#include <bws/packer/InstanceFormArray.hpp>

#include <bws/packer/InstanceArray.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
InstanceFormArray::InstanceFormArray()
	: super()
{
}

auto InstanceFormArray::createChild(shared_ptr<XML>) -> InstanceForm*
{
	return new InstanceForm();
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto InstanceFormArray::TAG() const -> string
{
	return "instanceFormArray";
}
auto InstanceFormArray::CHILD_TAG() const -> string
{
	return "instanceForm";
}

auto InstanceFormArray::toInstanceArray() const -> shared_ptr<InstanceArray>
{
	shared_ptr<InstanceArray> instanceArray(new InstanceArray());

	for (size_t i = 0; i < this->size(); i++)
	{
		const auto &myInstances = this->at(i)->toInstanceArray();
		instanceArray->insert(instanceArray->begin(), myInstances->begin(), myInstances->end());
	}

	return instanceArray;
}