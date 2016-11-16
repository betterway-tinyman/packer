#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <bws/packer/Wrap.hpp>
#include <bws/packer/Instance.hpp>

#include <array>

namespace bws
{
namespace packer
{
	/**
	 * @brief A wrapper, wrapping (containing) instances.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Wrapper
		: public protocol::SharedEntityArray<Wrap>,
		public Instance
	{
	private:
		typedef protocol::SharedEntityArray<Wrap> array_super;
		typedef Instance instance_super;

	protected:
		/**
		 * @brief Price, cost of using an Wrapper.
		 */
		double price;

		/**
		 * @brief Thickness, margin of a Wrapper causes shrinkness of containable volume.
		 *
		 * @details
		 * The thickness reduces each dimension's containable size (dimension - 2*thickness),
		 * so finally, it reduces total containable volume (-8 * thickness^3).
		 */
		double thickness;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		Wrapper()
			: array_super(),
			instance_super()
		{
			this->price = 0.0;
			this->thickness = 0.0;
		};

		/**
		 * @brief Copy Constructor.
		 *
		 * @details Copy constructor of the Wrapper only copies member variables.
		 *			It doesn't copy children elements; Wrap(s).
		 */
		Wrapper(const Wrapper &wrapper)
			: array_super(),
			instance_super(wrapper)
		{
			price = wrapper.price;
			thickness = wrapper.thickness;
		};

		/**
		 * @brief Construct from members.
		 *
		 * @param name Name, identifier of a Wrapper.
		 * @param price Price, issued cost for a type of the Wrapper.
		 * @param width Width, dimensional length on the X-axis in 3D.
		 * @param height Height, dimensional length on the Y-axis in 3D.
		 * @param length Length, dimensional length on the Z-axis in 3D.
		 * @param thickness A thickness causes shrinkness on containable volume.
		 */
		Wrapper(const std::string &name, double price, double width, double height, double length, double thickness = 0.0)
			: array_super(),
			instance_super(name, width, height, length)
		{
			this->price = price;
			this->thickness = thickness;
		};

		virtual ~Wrapper() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			instance_super::construct(xml);
			price = xml->getProperty<double>("price");
			thickness = xml->getProperty<double>("thickness");

			array_super::construct(xml);
		};

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> Wrap*
		{
			return new Wrap(this);
		};

	public:
		/* ===========================================================
			GETTERS
				- MEMBERS
				- DERIVED PROPERTIES
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		/**
		 * @brief Get price.
		 */
		auto getPrice() const -> double
		{
			return price;
		};

		/**
		 * @brief Get thickness.
		 */
		auto getThickness() const -> double
		{
			return thickness;
		};

		/* -----------------------------------------------------------
			DERIVED PROPERTIES
		----------------------------------------------------------- */
		/**
		 * @brief Get (calculate) containable width, length on the X-axis in 3D. </p>
		 * @details Calculates containable width considering the thickness
		 *
		 * @return width - (2 x thickness)
		 */
		auto getContainableWidth() const -> double
		{
			return width - (2 * thickness);
		};
		
		/**
		 * @brief Get (calculate) containable width, length on the X-axis in 3D. </p>
		 * @details Calculates containable width considering the thickness
		 *
		 * @return width - (2 x thickness)
		 */
		auto getContainableHeight() const -> double
		{
			return height - (2 * thickness);
		};
		
		/**
		 * @brief Get (calculate) containable height, length on the Y-axis in 3D. </p>
		 * @details Calculates containable height considering the thickness
		 *
		 * @return height - (2 x thickness)
		 */
		auto getContainableLength() const -> double
		{
			return length - (2 * thickness);
		};
		
		/**
		 * @brief Get (calculate) containable length, length on the Z-axis in 3D. </p>
		 * @details Calculates containable length considering the thickness
		 *
		 * @return length - (2 x thickness)
		 */
		auto getContainableVolume() const -> double
		{
			return getContainableWidth() * getContainableHeight() * getContainableLength();
		};

		/**
		 * @brief Get utilization ratio of containable volume.
		 *
		 * @return utilization ratio.
		 */
		auto getUtilization() const -> double
		{
			double numerator = 0.0;
			double denominator = getContainableVolume();

			for (size_t i = 0; i < size(); i++)
				numerator += at(i)->getVolume();

			return numerator / denominator;
		};

		/**
		 * @brief Wrapper is enough greater?
		 * @details Test whether the Wrapper is enough greater than an Instance to wrap.
		 *
		 * @param instance An Instance to test.
		 * @return Enough greater or not.
		 */
		auto operator>=(const Instance &instance) const -> bool
		{
			// TEST WHETHER AN INSTANCE IS GREATER THAN WRAPPER
			std::array<double, 3> wapperDims = { getContainableWidth(), getContainableHeight(), getContainableLength() };
			std::array<double, 3> instanceDims = { instance.getWidth(), instance.getHeight(), instance.getLength() };

			sort(wapperDims.begin(), wapperDims.end());
			sort(instanceDims.begin(), instanceDims.end());

			for (size_t i = 0; i < wapperDims.size(); i++)
				if (wapperDims[i] < instanceDims[i])
					return false;

			return true;
		};

		/**
		 * Test whether two Wrapper(s) are same.
		 *
		 * @param wrapper A wrapper to compare.
		 * @return Equals or not.
		 */
		auto operator==(const Wrapper &wrapper) const -> bool
		{
			return price == wrapper.price &&
				width == wrapper.width && height == wrapper.height && length == wrapper.length &&
				thickness == wrapper.thickness;
		};

	protected:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TYPE() const -> std::string override
		{
			return "wrapper";
		};

	public:
		virtual auto CHILD_TAG() const -> std::string override
		{
			return "wrap";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = array_super::toXML();

			xml->insertAllProperties(instance_super::toXML());
			xml->setProperty("price", price);
			xml->setProperty("thickness", thickness);
			xml->setProperty("utilization", getUtilization());

			return xml;
		};
	};
};
};