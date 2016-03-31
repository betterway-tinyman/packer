#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <bws/packer/Wrap.hpp>
#include <bws/packer/Instance.hpp>

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
		Wrapper();
		
		/**
		 * @brief Copy Constructor.
		 * 
		 * @details Copy constructor of the Wrapper only copies member variables. 
		 *			It doesn't copy children elements; Wrap(s).
		 */
		Wrapper(const Wrapper &);

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
		Wrapper(const std::string &, double, double, double, double, double = 0.0);
		
		virtual ~Wrapper() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;
	
	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> Wrap*;

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
		auto getPrice() const -> double;
		
		/**
		 * @brief Get thickness.
		 */
		auto getThickness() const -> double;

		/* -----------------------------------------------------------
			DERIVED PROPERTIES
		----------------------------------------------------------- */
		/**
		 * @brief Get (calculate) containable width, length on the X-axis in 3D. </p>
		 * @details Calculates containable width considering the thickness
		 *
		 * @return width - (2 x thickness)
		 */
		auto getContainableWidth() const -> double;
		
		/**
		 * @brief Get (calculate) containable width, length on the X-axis in 3D. </p>
		 * @details Calculates containable width considering the thickness
		 *
		 * @return width - (2 x thickness)
		 */
		auto getContainableHeight() const -> double;
		
		/**
		 * @brief Get (calculate) containable height, length on the Y-axis in 3D. </p>
		 * @details Calculates containable height considering the thickness
		 *
		 * @return height - (2 x thickness)
		 */
		auto getContainableLength() const -> double;
		
		/**
		 * @brief Get (calculate) containable length, length on the Z-axis in 3D. </p>
		 * @details Calculates containable length considering the thickness
		 *
		 * @return length - (2 x thickness)
		 */
		auto getContainableVolume() const -> double;

		/**
		 * @brief Get utilization ratio of containable volume.
		 *
		 * @return utilization ratio.
		 */
		auto getUtilization() const -> double;	

		/**
		 * @brief Wrapper is enough greater?
		 * @details Test whether the Wrapper is enough greater than an Instance to wrap.
		 *
		 * @param instance An Instance to test.
		 * @return Enough greater or not.
		 */
		auto operator>=(const Instance &) const -> bool;

		/**
		 * Test whether two Wrapper(s) are same.
		 *
		 * @param wrapper A wrapper to compare.
		 * @return Equals or not.
		 */
		auto operator==(const Wrapper &) const -> bool;

	protected:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TYPE() const -> std::string override;

	public:
		virtual auto CHILD_TAG() const -> std::string override;

		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};