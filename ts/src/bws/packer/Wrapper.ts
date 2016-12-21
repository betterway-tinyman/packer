/// <reference path="API.ts" />

namespace bws.packer
{
	/**
	 * A wrapper wrapping instances.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class Wrapper
		extends protocol.EntityDeque<Wrap>
		implements Instance
	{
		/**
		 * <p> Name, key of the Wrapper. </p>
		 *
		 * <p> The name represents a type of Wrapper and identifies the Wrapper. </p>
		 */
		protected name: string = "No Name";

		/**
		 * Price, cost of using an Wrapper.
		 */
		protected price: number = 1000.0;

		/**
		 * Width of the Wrapper, length on the X-axis in 3D.
		 */
		protected width: number = 10.0;

		/**
		 * Height of the Wrapper, length on the Y-axis in 3D.
		 */
		protected height: number = 10.0;

		/**
		 * Length of the Wrapper, length on the Z-axis in 3D.
		 */
		protected length: number = 10.0;

		/**
		 * <p> Thickness, margin of a Wrapper causes shrinkness of containable volume. </p>
		 *
		 * <p> The thickness reduces each dimension's containable size (dimension - 2*thickness),
		 * so finally, it reduces total containable volume (-8 * thickness^3). </p>
		 */
		protected thickness: number = 0.0;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor();

		/**
		 * Copy Constructor.
		 */
		public constructor(wrapper: Wrapper);

		/**
		 * Construct from members. 
		 *
		 * @param name Name, identifier of a Wrapper.
		 * @param price Price, issued cost for a type of the Wrapper.
		 * @param width Width, dimensional length on the X-axis in 3D.
		 * @param height Height, dimensional length on the Y-axis in 3D.
		 * @param length Length, dimensional length on the Z-axis in 3D.
		 * @param thickness A thickness causes shrinkness on containable volume.
		 */
		public constructor
			(
				name: string, price: number,
				width: number, height: number, length: number, thickness: number
			);

		public constructor(...args: any[])
		{
			super();

			if (args.length == 1 && args[0] instanceof Wrapper)
			{
				let wrapper: Wrapper = args[0];

				this.name = wrapper.name;
				this.price = wrapper.price;

				this.width = wrapper.width;
				this.height = wrapper.height;
				this.length = wrapper.length;
				this.thickness = wrapper.thickness;
			}
			else if (args.length == 6)
			{
				this.name = args[0];
				this.price = args[1];

				this.width = args[2];
				this.height = args[3];
				this.length = args[4];
				this.thickness = args[5];
			}
		}

		/**
		 * @inheritdoc
		 */
		public createChild(xml: library.XML): Wrap
		{
			return new Wrap(this);
		}

		/* ===========================================================
			ACCESSORS
				- MEMBERS
				- DERIVED PROPERTIES
				- COMPARISON
				- SETTERS
				- COLUMN ITEMS
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		/**
		 * Key of a Wrapper is its name.
		 */
		public key(): any
		{
			return this.name;
		}

		/**
		 * Get name.
		 */
		public getName(): string
		{
			return this.name;
		}

		/**
		 * Get price.
		 */
		public getPrice(): number
		{
			return this.price;
		}

		/**
		 * Get width, length on X-axis in 3D.
		 */
		public getWidth(): number
		{
			return this.width;
		}

		/**
		 * Get height, length on Y-axis in 3D.
		 */
		public getHeight(): number
		{
			return this.height;
		}

		/**
		 * Get length, length on Z-axis in 3D.
		 */
		public getLength(): number
		{
			return this.length;
		}
		
		/**
		 * Get thickness.
		 */
		public getThickness(): number
		{
			return this.thickness;
		}

		/* -----------------------------------------------------------
			DERIVED PROPERTIES
		----------------------------------------------------------- */
		/**
		 * <p> Get (calculate) containable width, length on the X-axis in 3D. </p>
		 *
		 * <p> Calculates containable width considering the {@link thickness}. </p>
		 *
		 * @return width - (2 x thickness)
		 */
		public getContainableWidth(): number
		{
			return this.width - (2 * this.thickness);
		}

		/**
		 * <p> Get (calculate) containable height, length on the Y-axis in 3D. </p>
		 *
		 * <p> Calculates containable height considering the {@link thickness}. </p>
		 *
		 * @return height - (2 x thickness)
		 */
		public getContainableHeight(): number
		{
			return this.height - (2 * this.thickness);
		}
		
		/**
		 * <p> Get (calculate) containable length, length on the Z-axis in 3D. </p>
		 *
		 * <p> Calculates containable length considering the {@link thickness}. </p>
		 *
		 * @return length - (2 x thickness)
		 */
		public getContainableLength(): number
		{
			return this.length - (2 * this.thickness);
		}
		
		/**
		 * <p> Get (calculate) volume. </p>
		 *
		 * <h4> Notice </h4>
		 * <p> If {@link thickness} of the Wrapper is not 0, the volume does not mean containable volume. 
		 * In that case, use {@link containableVolume} instead. </p>
		 *
		 * @return width x height x length
		 */
		public getVolume(): number
		{
			return this.width * this.height * this.length;
		}

		/**
		 * <p> Get (calculate) containable volume. </p>
		 *
		 * <p> Calculates containable volume considering the {@link thickness}. </p>
		 *
		 * @return volume - {(2 x thickness) ^ 3}
		 */
		public getContainableVolume(): number
		{
			return this.getContainableWidth() * this.getContainableHeight() * this.getContainableLength();
		}

		/**
		 * Get utilization ratio of containable volume.
		 *
		 * @return utilization ratio.
		 */
		public getUtilization(): number
		{
			let volume: number = 0.0;
			for (let i = 0; i < this.size(); i++)
				volume += this.at(i).getVolume();

			return volume / this.getContainableVolume();
		}

		/* -----------------------------------------------------------
			COMPARISON
		----------------------------------------------------------- */
		public equals(obj: Wrapper): boolean
		{
			return this.price == obj.price
				&& this.width == obj.width && this.height == obj.height && this.length == obj.length 
				&& this.thickness == obj.thickness;
		}

		/**
		 * <p> Wrapper is enough greater? </p>
		 * 
		 * <p> Test whether the Wrapper is enough greater than an Instance to contain. </p>
		 *
		 * @param instance An Instance to test.
		 * @return Enough greater or not.
		 */
		public containable(instance: Instance): boolean
		{
			// TEST WHETHER AN INSTANCE IS GREATER THAN WRAPPER
			let myDims = new std.Vector<number>([this.getContainableWidth(), this.getContainableHeight(), this.getContainableLength()]);
			let instanceDims = new std.Vector<number>([instance.getWidth(), instance.getHeight(), instance.getLength()]);

			std.sort(myDims.begin(), myDims.end());
			std.sort(instanceDims.begin(), instanceDims.end());

			for (let i: number = 0; i < myDims.size(); i++)
				if (myDims.at(i) < instanceDims.at(i))
					return false;

			return true;
		}

		/* -----------------------------------------------------------
			SETTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public setName(val: string): void
		{
			this.name = val;
		}

		/**
		 * Set price.
		 */
		public setPrice(val: number): void
		{
			this.price = val;
		}

		/**
		 * @inheritdoc
		 */
		public setWidth(val: number): void
		{
			this.width = val;
		}

		/**
		 * @inheritdoc
		 */
		public setHeight(val: number): void
		{
			this.height = val;
		}

		/**
		 * @inheritdoc
		 */
		public setLength(val: number): void
		{
			this.length = val;
		}

		/**
		 * Set thickness.
		 */
		public setThickness(val: number): void
		{
			this.thickness = val;
		}

		/* -----------------------------------------------------------
			COLUMN ITEMS
		----------------------------------------------------------- */
		public get $name(): string			{	return this.name;					}
		public get $price(): string			{	return this.price + "";				}
		public get $width(): string			{	return this.width + "";			}
		public get $height(): string		{	return this.height + "";			}
		public get $length(): string		{	return this.length + "";			}
		public get $thickness(): string		{	return this.thickness + "";			}
		
		public set $name(val: string)		{	this.name = val;					}
		public set $price(val: string)		{	this.price = parseFloat(val);		}
		public set $width(val: string)		{	this.width = parseFloat(val);		}
		public set $height(val: string)		{	this.height = parseFloat(val);		}
		public set $length(val: string)		{	this.length = parseFloat(val);		}
		public set $thickness(val: string)	{	this.thickness = parseFloat(val);	}

		public get $scale(): string	
		{
			return this.width + ", " + this.height + ", " + this.length;
		}
		public get $spaceUtilization(): string
		{
			return Math.round(this.getUtilization() * 10000) / 100.0 + "%";
		}

		/* ===========================================================
			EXPORTERS
		=========================================================== */
		/**
		 * @inheritdoc
		 */
		public TYPE(): string
		{
			return "wrapper";
		}

		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "instance";
		}

		/**
		 * @inheritdoc
		 */
		public CHILD_TAG(): string
		{
			return "wrap";
		}

		/**
		 * @inheritdoc
		 */
		public toXML(): library.XML
		{
			let xml: library.XML = super.toXML();
			xml.setProperty("type", this.TYPE());

			return xml;
		}
	}
}