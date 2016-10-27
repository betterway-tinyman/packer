#include <bws/packer/Wrapper.hpp>

#include <array>

using namespace bws::packer;

using namespace std;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
Wrapper::Wrapper()
	: array_super(), 
	instance_super()
{
	this->price = 0.0;
	this->thickness = 0.0;
}

Wrapper::Wrapper(const Wrapper &wrapper)
	: array_super(),
	instance_super(wrapper)
{
	price = wrapper.price;
	thickness = wrapper.thickness;
}

Wrapper::Wrapper
	(
		const string &name, double price, 
		double width, double height, double length, double thickness
	) : array_super(),
		instance_super(name, width, height, length)
{
	this->price = price;
	this->thickness = thickness;
}

void Wrapper::construct(shared_ptr<XML> xml)
{
	instance_super::construct(xml);
	price = xml->getProperty<double>("price");
	thickness = xml->getProperty<double>("thickness");

	array_super::construct(xml);
}

auto Wrapper::createChild(shared_ptr<XML>) -> Wrap*
{
	return new Wrap(this);
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto Wrapper::getPrice() const -> double
{
	return price;
}
auto Wrapper::getThickness() const -> double
{
	return thickness;
}

auto Wrapper::getContainableWidth() const -> double
{
	return width - (2 * thickness);
}
auto Wrapper::getContainableHeight() const -> double
{
	return height - (2 * thickness);
}
auto Wrapper::getContainableLength() const -> double
{
	return length - (2 * thickness);
}
auto Wrapper::getContainableVolume() const -> double
{
	return getContainableWidth() * getContainableHeight() * getContainableLength();
}

auto Wrapper::getUtilization() const -> double
{
	double numerator = 0.0;
	double denominator = getContainableVolume();

	for (size_t i = 0; i < size(); i++)
		numerator += at(i)->getVolume();

	return numerator / denominator;
}

auto Wrapper::operator>=(const Instance &instance) const -> bool
{
	// TEST WHETHER AN INSTANCE IS GREATER THAN WRAPPER
	array<double, 3> wapperDims = { getContainableWidth(), getContainableHeight(), getContainableLength() };
	array<double, 3> instanceDims = { instance.getWidth(), instance.getHeight(), instance.getLength() };

	sort(wapperDims.begin(), wapperDims.end());
	sort(instanceDims.begin(), instanceDims.end());

	for (size_t i = 0; i < wapperDims.size(); i++)
		if (wapperDims[i] < instanceDims[i])
			return false;

	return true;
}

auto Wrapper::operator==(const Wrapper &wrapper) const -> bool
{
	return price == wrapper.price &&
		width == wrapper.width && height == wrapper.height && length == wrapper.length &&
		thickness == wrapper.thickness;
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto Wrapper::TYPE() const -> string
{
	return "wrapper";
}

auto Wrapper::CHILD_TAG() const -> string
{
	return "wrap";
}

auto Wrapper::toXML() const -> shared_ptr<XML>
{
	auto &xml = array_super::toXML();

	xml->insertAllProperties(instance_super::toXML());
	xml->setProperty("price", price);
	xml->setProperty("thickness", thickness);
	xml->setProperty("utilization", getUtilization());

	return xml;
}