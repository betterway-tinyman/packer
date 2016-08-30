/// <reference path="API.ts" />

namespace bws.packer
{
	/**
	 * A product.
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class Product 
		extends protocol.Entity
		implements Instance
	{
		/**
		 * <p> Name, key of the Product. </p>
		 *
		 * <p> The name must be unique because a name identifies a {@link Product}. </p>
		 */
		protected name: string = "";

		/**
		 * Width of the Product, length on the X-axis in 3D.
		 */
		protected width: number = 0.0;
		
		/**
		 * Height of the Product, length on the Y-axis in 3D.
		 */
		protected height: number = 0.0;
		
		/**
		 * Length of the Product, length on the Z-axis in 3D.
		 */
		protected length: number = 0.0;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor();

		/**
		 * Construct from members.
		 * 
		 * @param name Name, identifier of the Product.
		 * @param width Width, length on the X-axis in 3D.
		 * @param height Height, length on the Y-axis in 3D.
		 * @param length Length, length on the Z-axis in 3D.
		 */
		public constructor(name: string, width: number, height: number, length: number);

		public constructor(name: string = "No Name", width: number = 0, height: number = 0, length: number = 0)
		{
			super();

			this.name = name;
			
			this.width = width;
			this.height = height;
			this.length = length;
		}

		/* -----------------------------------------------------------
			ACCESSORS
		----------------------------------------------------------- */
		/**
		 * Key of a Product is its name.
		 */
		public key(): any
		{
			return this.name;
		}

		/**
		 * @inheritdoc
		 */
		public getName(): string
		{
			return this.name;
		}

		/**
		 * @inheritdoc
		 */
		public getWidth(): number
		{
			return this.width;
		}

		/**
		 * @inheritdoc
		 */
		public getHeight(): number
		{
			return this.height;
		}

		/**
		 * @inheritdoc
		 */
		public getLength(): number
		{
			return this.length;
		}

		/**
		 * @inheritdoc
		 */
		public getVolume(): number
		{
			return this.width * this.height * this.length;
		}

		/**
		 * @inheritdoc
		 */
		public setName(val: string): void
		{
			this.name = val;
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

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TYPE(): string
		{
			return "product";
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
		public toXML(): library.XML
		{
			let xml: library.XML = super.toXML();
			xml.setProperty("type", this.TYPE());

			return xml;
		}
	}
}