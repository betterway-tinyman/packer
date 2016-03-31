#include <bws/packer/PackerForm.hpp>

#include <bws/packer/InstanceFormArray.hpp>
#include <bws/packer/WrapperArray.hpp>
#include <bws/packer/Packer.hpp>
#include <samchon/library/GAParameters.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
PackerForm::PackerForm()
	: super()
{
	instanceFormArray.reset(new InstanceFormArray());
	wrapperArray.reset(new WrapperArray());
	gaParameters.reset(new GAParameters());
}

void PackerForm::construct(shared_ptr<XML> xml)
{
	instanceFormArray->construct(xml->get(instanceFormArray->TAG())->at(0));
	wrapperArray->construct(xml->get(wrapperArray->TAG())->at(0));
	
	if (xml->has(gaParameters->TAG()))
		gaParameters->construct(xml->get(gaParameters->TAG())->at(0));
	else
		gaParameters.reset(new GAParameters());
}

/* -----------------------------------------------------------
	ACCESSORS
----------------------------------------------------------- */
auto PackerForm::optimize() const -> shared_ptr<WrapperArray>
{
	auto &packer = this->toPacker();

	return packer->optimize(*gaParameters);
}

auto PackerForm::getGAParameters() const -> shared_ptr<GAParameters>
{
	return gaParameters;
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto PackerForm::TAG() const -> string
{
	return "packerForm";
}

auto PackerForm::toXML() const -> shared_ptr<XML>
{
	shared_ptr<XML> &xml = super::toXML();
	xml->push_back(instanceFormArray->toXML());
	xml->push_back(wrapperArray->toXML());
	xml->push_back(gaParameters->toXML());

	return xml;
}
auto PackerForm::toPacker() const -> shared_ptr<Packer>
{
	return make_shared<Packer>(wrapperArray, instanceFormArray->toInstanceArray());
}