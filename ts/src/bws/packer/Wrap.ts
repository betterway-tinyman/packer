/// <reference path="API.ts" />

namespace bws.packer
{
	/**
	 * <p> Wrap represents an act wrap(ping). </p>
	 *
	 * <p> {@link Wrap} is a class represents an act wrapping an {@link Instance} to an {@link Wrapper}.
	 * To represent the relationship, Wrap uses Bridge and Capsular patterns to links and intermediates 
	 * relationship between Wrapper and Instance. </p>
	 *
	 * <p> Wrap also helps packing optimization and 3d-visualization with its own members 
	 * {@link orientation} and position variables {@link x}, {@link y} and {@link z}. </p> 
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class Wrap
		extends protocol.Entity
	{
		/**
		 * A wrapper wrapping the {@link instance}.
		 */
		protected wrapper: Wrapper;
		
		/**
		 * An instance wrapped into the {@link wrapper}.
		 */
		protected instance: Instance;

		/**
		 * Coordinate-X of the instance placement in the wrapper.
		 */
		protected x: number;
		
		/**
		 * Coordinate-Y of the instance placement in the wrapper.
		 */
		protected y: number;
		
		/**
		 * Coordinate-Z of the instance placement in the wrapper.
		 */
		protected z: number;

		/**
		 * Placement orientation of wrapped {@link instance}.
		 */
		protected orientation: number;

		/* ============================================================
			CONSTRUCTORS & SETTERS
		===============================================================
			CONSTRUCTORS
		------------------------------------------------------------ */
		/**
		 * Construct from a Wrapper.
		 * 
		 * @param wrapper A wrapper who will contain an instance.
		 */
		public constructor(wrapper: Wrapper);

		/**
		 * Construct from a Wrapper and Instance with its position and default orientation 1. 
		 *
		 * @param wrapper A wrapper who contains the instance.
		 * @param instance An instance contained into the wrapper.
		 * @param x Coordinate-X of the {@link instance} placement in the {@link wrapper}.
		 * @param y Coordinate-Y of the {@link instance} placement in the {@link wrapper}.
		 * @param z Coordinate-Z of the {@link instance} placement in the {@link wrapper}.
		 */
		public constructor
			(
				wrapper: Wrapper, instance: Instance,
				x: number, y: number, z: number
			);

		/**
		 * Construct from a Wrapper and Instance with its position and orientation. 
		 *
		 * @param wrapper A wrapper who contains the instance.
		 * @param instance An instance contained into the wrapper.
		 * @param x Coordinate-X of the {@link instance} placement in the {@link wrapper}.
		 * @param y Coordinate-Y of the {@link instance} placement in the {@link wrapper}.
		 * @param z Coordinate-Z of the {@link instance} placement in the {@link wrapper}.
		 * @param orientation Placement orientation of wrapped {@link instance}.
		 */
		public constructor
			(
				wrapper: Wrapper, instance: Instance, 
				x: number, y: number, z: number, orientation: number
			);

		public constructor
			(
				wrapper: Wrapper, instance: Instance = null,
				x: number = 0, y: number = 0, z: number = 0, orientation: number = 1
			)
		{
			super();
			
			this.wrapper = wrapper;
			this.instance = instance;
			
			this.x = x;
			this.y = y;
			this.z = z;
			this.orientation = orientation;
		}

		/**
		 * @inheritdoc
		 */
		public construct(xml: library.XML): void
		{
			super.construct(xml);

			let instanceXML = xml.get("instance").at(0);
			this.instance = this.createInstance(instanceXML.getProperty("type"));
			this.instance.construct(instanceXML);
		}

		/**
		 * Factory method of wrapped Instance. 
		 *
		 * @param type Type of contained Instance's type.
		 */
		protected createInstance(type: string): Instance
		{
			if (type == "product")
				return new Product();
			else if (type == "wrapper")
				return new Wrapper();
			else
				return null;
		}

		/* ===========================================================
			SETTERS
				- MEMBERS
				- ESTIMATERS
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		/**
		 * Set orientation.
		 * 
		 * @param orientation Orientation code (1 to 6).
		 */
		public setOrientation(orientation: number): void
		{
			this.orientation = orientation;
		}
		
		/**
		 * Set position.
		 * 
		 * @param x Coordinate-X of the instance placement in the wrapper.
		 * @param y Coordinate-Y of the instance placement in the wrapper.
		 * @param z Coordinate-Z of the instance placement in the wrapper.
		 */
		public setPosition(x: number, y: number, z: number): void
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}

		/* -----------------------------------------------------------
			ESTIMATERS
		----------------------------------------------------------- */
		/**
		 * @brief Estimate orientation by given size.
		 *
		 * @param width Width by placement.
		 * @param height Height by placement.
		 * @param length Length by placement.
		 */
		public estimateOrientation(width: number, height: number, length: number): void
		{
			if (this.instance.getWidth() == width && this.instance.getHeight() == height)
				this.orientation = 1;
			else if (this.instance.getWidth() == length && this.instance.getHeight() == height)
				this.orientation = 2;
			else if (this.instance.getWidth() == length && this.instance.getHeight() == width)
				this.orientation = 3;
			else if (this.instance.getWidth() == height && this.instance.getHeight() == width)
				this.orientation = 4;
			else if (this.instance.getWidth() == width && this.instance.getHeight() == length)
				this.orientation = 5;
			else
				this.orientation = 6;
		}

		/**
		 * @brief Orientation change is occured in level of the packer.
		 *
		 * @details orientation Packer's new orientation.
		 */
		public changeWrapperOrientation(orientation: number): void
		{
			if (orientation == 1)
				return;

			// DECLARES
			let x: number;
			let y: number;
			let z: number;
			let width: number;
			let height: number;
			let length: number;

			if (orientation == 2)
			{
				width = this.instance.getLength();
				height = this.instance.getHeight();
				length = this.instance.getWidth();

				x = this.z;
				y = this.y;
				z = this.wrapper.getWidth() - (length + this.x);
			}
			else if (orientation == 3)
			{
				width = this.instance.getLength();
				height = this.instance.getWidth();
				length = this.instance.getHeight();

				x = this.z;
				y = this.wrapper.getWidth() - (height + this.x);
				z = this.wrapper.getHeight() - (length + this.y);
			}
			else if (orientation == 4)
			{
				width = this.instance.getHeight();
				height = this.instance.getWidth();
				length = this.instance.getLength();

				x = this.y;
				y = this.x;
				z = this.z;
			}
			else if (orientation == 5)
			{
				width = this.instance.getWidth();
				height = this.instance.getLength();
				length = this.instance.getHeight();

				x = this.x;
				y = this.wrapper.getLength() - (height + this.z);
				z = this.y;
			}
			else
			{
				width = this.instance.getHeight();
				height = this.instance.getLength();
				length = this.instance.getWidth();

				x = this.y;
				y = this.z;
				z = this.wrapper.getWidth() - (length - this.x);
			}

			this.estimateOrientation(width, height, length);
			this.x = x;
			this.y = y;
			this.z = z;
		}

		/* ===========================================================
			GETTERS
		=========================================================== */
		/**
		 * Get wrapper.
		 */
		public getWrapper(): Wrapper
		{
			return this.wrapper;
		}

		/**
		 * Get instance.
		 */
		public getInstance(): Instance
		{
			return this.instance;
		}

		/**
		 * Get x.
		 */
		public getX(): number
		{
			return this.x;
		}

		/**
		 * Get y.
		 */
		public getY(): number
		{
			return this.y;
		}

		/**
		 * Get z.
		 */
		public getZ(): number
		{
			return this.z;
		}

		/**
		 * Get orientation.
		 */
		public getOrientation(): number
		{
			return this.orientation;
		}

		/**
		 * Get width.
		 */
		public getLayoutWidth(): number
		{
			switch (this.orientation)
			{
			case 1:	case 5:
				return this.instance.getWidth();
			case 3:	case 4:
				return this.instance.getHeight();
			default: // 2, 5
				return this.instance.getLength();
			}
		}

		/**
		 * Get height.
		 */
		public getLayoutHeight(): number
		{
			switch (this.orientation)
			{
			case 1:	case 2:
				return this.instance.getHeight();
			case 4:	case 6:
				return this.instance.getWidth();
			default: // 3, 5
				return this.instance.getLength();
			}
		}

		/**
		 * Get length.
		 */
		public getLength(): number
		{
			switch (this.orientation)
			{
			case 1: case 4:
				return this.instance.getLength();
			case 2:	case 3:
				return this.instance.getWidth();
			default: // 5, 6
				return this.instance.getHeight();
			}
		}

		/**
		 * Get volume.
		 */
		public getVolume(): number
		{
			return this.instance.getVolume();
		}

		public get $instanceName(): string
		{
			return this.instance.getName();
		}
		public get $layoutScale(): string
		{
			return this.getLayoutWidth() + ", " + this.getLayoutHeight() + ", " + this.getLength();
		}
		public get $position(): string
		{
			return this.x + ", " + this.y + ", " + this.z;
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "wrap";
		}

		/**
		 * @inheritdoc
		 */
		public toXML(): library.XML
		{
			let xml: library.XML = super.toXML();
			xml.push(this.instance.toXML());
			
			xml.setProperty("layoutWidth", this.getLayoutWidth() + "");
			xml.setProperty("layoutHeight", this.getLayoutHeight() + "");
			xml.setProperty("layoutLength", this.getLength() + "");

			return xml;
		}
	}
}