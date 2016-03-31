#include <bws/packer/InstanceForm.hpp>

#include <bws/packer/InstanceArray.hpp>
#	include <bws/packer/Product.hpp>
#	include <bws/packer/Wrapper.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
InstanceForm::InstanceForm()
	: super()
{
}
void InstanceForm::construct(shared_ptr<XML> xml)
{
	if (xml->hasProperty("type"))
	{
		instance.reset(createInstance(xml));
		instance->construct(xml);
	}
	else if (xml->has("instance"))
	{
		auto instanceXML = xml->get("instance")->at(0);

		instance.reset(createInstance(instanceXML));
		instance->construct(instanceXML);
	}

	count = xml->getProperty<size_t>("count");
}

auto InstanceForm::createInstance(shared_ptr<XML> xml) -> Instance*
{
	if (xml->getProperty("type") == "product")
		return new Product();
	else
		return new Wrapper();
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto InstanceForm::TAG() const -> string
{
	return "instanceForm";
}

auto InstanceForm::toXML() const -> shared_ptr<XML>
{
	shared_ptr<XML> &xml = super::toXML();
	if (instance != nullptr)
		xml->push_back(instance->toXML());

	xml->setProperty("count", count);

	return xml;
}

auto InstanceForm::toInstanceArray() const -> shared_ptr<InstanceArray>
{
	shared_ptr<InstanceArray> instanceArray(new InstanceArray());
	instanceArray->assign(count, instance);

	return instanceArray;
}