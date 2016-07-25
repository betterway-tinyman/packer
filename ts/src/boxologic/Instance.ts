/// <reference path="../bws/packer/API.ts" />

namespace boxologic
{
	/**
	 * <p> An abstract instance of boxologic. </p>
	 *
	 * <p> {@link st_Instance} represents a physical, tangible instance of 3-dimension. </p>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export abstract class Instance
	{
		////////////////////////////
		// BASIC MEMBERS
		////////////////////////////
		/**
		 * Width of the tangible instance, length on the X-axis in 3D.
		 */
		public width: number;

		/**
		 * Height of the tangible instance, length on the Y-axis in 3D.
		 */
		public height: number;

		/**
		 * Length of the tangible instance, length on the Z-axis in 3D.
		 */
		public length: number;

		////////////////////////////
		// LAYOUT MEMBERS
		////////////////////////////
		/**
		 * Width considering layout placement.
		 */
		public layout_width: number;

		/**
		 * Height considering layout placement.
		 */
		public layout_height: number;

		/**
		 * Length considering layout placement.
		 */
		public layout_length: number;

		////////////////////////////
		// DERIVED MEMBER
		////////////////////////////
		/**
		 * Volume, Width x Height x Length.
		 */
		public volume: number;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Construct from size members.
		 * 
		 * @param width Width, length on the X-axis in 3D.
		 * @param height Height, length on the Y-axis in 3D.
		 * @param length Length, length on the Z-axis in 3D.
		 */
		public constructor(width: number, height: number, length: number)
		{
			// INIT MEMBERS
			this.width = width;
			this.height = height;
			this.length = length;

			// INIT LAYOUTS
			this.layout_width = width;
			this.layout_height = height;
			this.layout_length = length;

			// DERIVED PROPERTY; VOLUME
			this.volume = width * height * length;
		}
	}
}