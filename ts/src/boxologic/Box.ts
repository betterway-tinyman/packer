/// <reference path="Instance.ts" />

namespace boxologic
{
	/**
	 * A box, trying to pack into a {@link Pallet}.
	 * 
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 */
	export class Box
		extends Instance
	{
		/**
		 * Coordinate-X of the box placement in a {@link Pallet}.
		 */
		public cox: number;

		/**
		 * Coordinate-Y of the box placement in a {@link Pallet}.
		 */
		public coy: number;

		/**
		 * Coordinate-Z of the box placement in a {@link Pallet}.
		 */
		public coz: number;

		/**
		 * Whether the {@link Box} is packed into a {@link Pallet}.
		 */
		public is_packed: boolean;

		/**
		 * Construct from an instance.
		 * 
		 * @param instance An instance adapts with.
		 */
		public constructor(instance: bws.packer.Instance)
		{
			super
				(
				instance.getWidth(),
				instance.getHeight(),
				instance.getLength()
				);

			this.cox = 0;
			this.coy = 0;
			this.coz = 0;

			this.is_packed = false;
		}
	}
}