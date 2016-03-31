#include <bws/packer/InstanceArray.hpp>

#include <bws/packer/Wrapper.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* ---------------------------------------------------------
	CONSTRUCTORS
--------------------------------------------------------- */
auto InstanceArray::createChild(shared_ptr<XML> xml) -> Instance*
{
	if (xml->getProperty("type") == "product")
		return new Product();
	else
		return new Wrapper();
}

/* ---------------------------------------------------------
	EXPORTERS
--------------------------------------------------------- */
auto InstanceArray::TAG() const -> string
{
	return "instanceArray";
}
auto InstanceArray::CHILD_TAG() const -> string
{
	return "instance";
}