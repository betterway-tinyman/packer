#pragma once

namespace boxologic
{
	/**
	 * @brief An abstract instance of boxologic. 
	 * @details st_Instance represents a physical, tangible instance of 3-dimension.
	 *
	 * @see bws::packer::Instance
	 * @see bws::packer::Wrap
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	struct st_Instance
	{
	public:
		////////////////////////////
		// BASIC MEMBERS
		////////////////////////////
		/**
		 * @brief Width of the tangible instance, length on the X-axis in 3D.
		 */
		double width;

		/**
		 * @brief Height of the tangible instance, length on the Y-axis in 3D.
		 */
		double height;

		/**
		 * @brief Length of the tangible instance, length on the Z-axis in 3D.
		 */
		double length;

		////////////////////////////
		// LAYOUT MEMBERS
		////////////////////////////
		/**
		 * @brief Width considering layout placement.
		 */
		double layout_width;

		/**
		 * @brief Height considering layout placement.
		 */
		double layout_height;

		/**
		 * @brief Length considering layout placement.
		 */
		double layout_length;

		////////////////////////////
		// DERIVED MEMBER
		////////////////////////////
		/**
		 * @brief Volume, Width x Height x Length.
		 */
		double volume;
	};
};