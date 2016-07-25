/// <reference path="../bws/packer/API.ts" />

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

		public overlapped_boxes: std.HashSet<Box> = new std.HashSet<Box>();

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

		public hit_test(obj: Box): boolean
		{
			return (this.cox == obj.cox && this.coy == obj.coy && this.coz == obj.coy) 
				|| (this.hit_test_single(obj) == false && obj.hit_test_single(this) == false);
		}

		private hit_test_single(obj: Box): boolean
		{
			return this.hit_test_point(obj.cox,						obj.coy,						obj.coz)
				|| this.hit_test_point(obj.cox + obj.layout_width,	obj.coy,						obj.coz)
				|| this.hit_test_point(obj.cox,						obj.coy + obj.layout_height,	obj.coz)
				|| this.hit_test_point(obj.cox,						obj.coy,						obj.coz + obj.layout_length)
				|| this.hit_test_point(obj.cox + obj.layout_width,	obj.coy + obj.layout_height,	obj.coz)
				|| this.hit_test_point(obj.cox + obj.layout_width,	obj.coy,						obj.coz + obj.layout_length)
				|| this.hit_test_point(obj.cox,						obj.coy + obj.layout_height,	obj.coz + obj.layout_length)
				|| this.hit_test_point(obj.cox + obj.layout_width,	obj.coy + obj.layout_height,	obj.coz + obj.layout_length);
		}

		private hit_test_point(x: number, y: number, z: number): boolean
		{
			return this.cox < x && x < this.cox + this.layout_width 
				&& this.coy < y && y < this.coy + this.layout_height 
				&& this.coz < z && z < this.coz + this.layout_length; 
		}
	}
}