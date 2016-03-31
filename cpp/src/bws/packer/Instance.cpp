#include <bws/packer/Instance.hpp>

#include <array>
#include <algorithm>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
Instance::Instance()
	: Instance("", 0, 0, 0)
{
}

Instance::Instance(const Instance &instance)
	: Instance(instance.name, instance.width, instance.height, instance.length)
{
}

Instance::Instance(const string &name, double width, double height, double length)
{
	this->name = name;

	this->width = width;
	this->height = height;
	this->length = length;
}

void Instance::construct(shared_ptr<XML> xml)
{
	name = xml->getProperty<string>("name");

	width = xml->getProperty<double>("width");
	height = xml->getProperty<double>("height");
	length = xml->getProperty<double>("length");
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto Instance::key() const -> string
{
	return name;
}
auto Instance::getName() const -> string
{
	return name;
}

auto Instance::getWidth() const -> double
{
	return width;
}
auto Instance::getHeight() const -> double
{
	return height;
}
auto Instance::getLength() const -> double
{
	return length;
}

auto Instance::getVolume() const -> double
{
	return width * height * length;
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto Instance::TAG() const -> string
{
	return "instance";
}

auto Instance::toXML() const -> shared_ptr<XML>
{
	shared_ptr<XML> &xml = super::toXML();
	xml->setProperty("type", TYPE());
	xml->setProperty("name", name);

	xml->setProperty("width", width);
	xml->setProperty("height", height);
	xml->setProperty("length", length);

	return xml;
}