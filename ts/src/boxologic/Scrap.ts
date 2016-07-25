/// <reference path="../bws/packer/API.ts" />

namespace boxologic
{
	/**
	 * <p> Cumulated lengths of current layer. </p>
	 *
	 * <p> {@link Scrapped} represents an edge of the current layer under construction. </p>
	 * 
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 */
	export class Scrap
	{
		/**
		 * Cumulated length on the X-axis in 3D.
		 */
		public cumx: number;

		/**
		 * Cumulated length on the Z-axis in 3D.
		 */
		public cumz: number;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor();

		/**
		 * Initialization Constructor.
		 * 
		 * @param cumx Cumulated length on the x-axis.
		 * @param cumz Cumulated length on the z-axis.
		 */
		public constructor(cumx: number, cumz: number);

		public constructor(cumx: number = 0, cumz: number = 0)
		{
			this.cumx = cumx;
			this.cumz = cumz;
		}
	}
}