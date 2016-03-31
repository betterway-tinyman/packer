#include <bws/packer/Wrap.hpp>

#include <bws/packer/Wrapper.hpp>
#include <bws/packer/Instance.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
Wrap::Wrap(Wrapper *wrapper)
	: super()
{
	this->wrapper = wrapper;
	//this->duplicated = 1;
}

Wrap::Wrap
	(
		Wrapper *wrapper, shared_ptr<Instance> instance,
		double x, double y, double z, int direction
	) : Wrap(wrapper)
{
	this->instance = instance;

	this->x = x;
	this->y = y;
	this->z = z;

	this->orientation = direction;
}

void Wrap::construct(shared_ptr<XML> xml)
{
	// 키값을 통해 WRAPPER와 PRODUCT를 가져올 방법을 특정해야 함
	// Wrapper->Packer->productArray로 가도록 한다

	x = xml->getProperty<double>("x");
	y = xml->getProperty<double>("y");
	z = xml->getProperty<double>("z");

	orientation = xml->getProperty<int>("orientation");
	//duplicated = xml->getProperty<size_t>("duplicated");
}

void Wrap::setPosition(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
void Wrap::setOrientation(int val)
{
	this->orientation = val;
}

void Wrap::estimateOrientation(double width, double height, double length)
{
	if (instance->getWidth() == width && instance->getHeight() == height)
		orientation = 1;
	else if (instance->getWidth() == length && instance->getHeight() == height)
		orientation = 2;
	else if (instance->getWidth() == length && instance->getHeight() == width)
		orientation = 3;
	else if (instance->getWidth() == height && instance->getHeight() == width)
		orientation = 4;
	else if (instance->getWidth() == width && instance->getHeight() == length)
		orientation = 5;
	else
		orientation = 6;
}

void Wrap::changeWrapperOrientation(int orietation)
{
	if (orientation == 1)
		return;

	// DECLARES
	double x, y, z;
	double width, height, length;

	if (orientation == 2)
	{
		width = instance->getLength();
		height = instance->getHeight();
		length = instance->getWidth();

		x = this->z;
		y = this->y;
		z = wrapper->getWidth() - (length + this->x);
	}
	else if (orientation == 3)
	{
		width = instance->getLength();
		height = instance->getWidth();
		length = instance->getHeight();

		x = this->z;
		y = wrapper->getWidth() - (height + this->x);
		z = wrapper->getHeight() - (length + this->y);
	}
	else if (orientation == 4)
	{
		width = instance->getHeight();
		height = instance->getWidth();
		length = instance->getLength();

		x = this->y;
		y = this->x;
		z = this->z;
	}
	else if (orientation == 5)
	{
		width = instance->getWidth();
		height = instance->getLength();
		length = instance->getHeight();

		x = this->x;
		y = wrapper->getLength() - (height + this->z);
		z = this->y;
	}
	else
	{
		width = instance->getHeight();
		height = instance->getLength();
		length = instance->getWidth();

		x = this->y;
		y = this->z;
		z = wrapper->getWidth() - (length - this->x);
	}

	this->estimateOrientation(width, height, length);
	this->x = x;
	this->y = y;
	this->z = z;
}

/* -----------------------------------------------------------
	GETTERS
----------------------------------------------------------- */
auto Wrap::getWrapper() const -> Wrapper*
{
	return wrapper;
}
auto Wrap::getInstance() const -> shared_ptr<Instance>
{
	return instance;
}

auto Wrap::getOrientation() const -> int
{
	return orientation;
}

auto Wrap::getX() const -> double
{
	return x;
}
auto Wrap::getY() const -> double
{
	return y;
}
auto Wrap::getZ() const -> double
{
	return z;
}

auto Wrap::getWidth() const -> double
{
	switch (orientation)
	{
	case 1: case 5:
		return instance->getWidth();
	case 3: case 4:
		return instance->getHeight();
	default: // 2, 5
		return instance->getLength();
	}
}
auto Wrap::getHeight() const -> double
{
	switch (orientation)
	{
	case 1: case 2:
		return instance->getHeight();
	case 4: case 6:
		return instance->getWidth();
	default: // 3, 5
		return instance->getLength();
	}
}
auto Wrap::getLength() const -> double
{
	switch (orientation)
	{
	case 1: case 4:
		return instance->getLength();
	case 2: case 3:
		return instance->getWidth();
	default: // 5, 6
		return instance->getHeight();
	}
}
auto Wrap::getVolume() const -> double
{
	return instance->getVolume();
}

/* -----------------------------------------------------------
	EXPORTERS
----------------------------------------------------------- */
auto Wrap::TAG() const -> string
{
	return "wrap";
}

auto Wrap::toXML() const -> shared_ptr<XML>
{
	shared_ptr<XML> &xml = super::toXML();
	xml->setProperty("instance", instance->key());

	// 나중에 없애야 함
	xml->push_back(instance->toXML());

	xml->setProperty("x", x);
	xml->setProperty("y", y);
	xml->setProperty("z", z);
	xml->setProperty("orientation", orientation);
	//xml->setProperty("duplicated", duplicated);

	return xml;
}