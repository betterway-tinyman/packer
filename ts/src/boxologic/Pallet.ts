/// <reference path="../bws/packer/API.ts" />

/// <reference path="Instance.ts" />

namespace boxologic
{
	/**
	 * A pallet containing boxes.
	 * 
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 */
	export class Pallet
		extends Instance
	{
		/**
		 * Construct from a wrapper.
		 * 
		 * @param wrapper A wrapper wrapping instances.
		 */
		public constructor(wrapper: bws.packer.Wrapper)
		{
			super
				(
				wrapper.getContainableWidth(),
				wrapper.getContainableHeight(),
				wrapper.getContainableLength()
				);
		}

		/**
		 * Set placement orientation.
		 */
		public set_orientation(orientation: number): void
		{
			switch (orientation)
			{
				case 1:
					this.layout_width = this.width;
					this.layout_height = this.height;
					this.layout_length = this.length;
					break;
				case 2:
					this.layout_width = this.length;
					this.layout_height = this.height;
					this.layout_length = this.width;
					break;
				case 3:
					this.layout_width = this.length;
					this.layout_height = this.width;
					this.layout_length = this.height;
					break;
				case 4:
					this.layout_width = this.height;
					this.layout_height = this.width;
					this.layout_length = this.length;
					break;
				case 5:
					this.layout_width = this.width;
					this.layout_height = this.length;
					this.layout_length = this.height;
					break;
				case 6:
					this.layout_width = this.height;
					this.layout_height = this.length;
					this.layout_length = this.width;
					break;
			}
		}
	}
}