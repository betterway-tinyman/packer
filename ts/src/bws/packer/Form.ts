/// <reference path="API.ts" />

namespace bws.packer
{
	/**
	 * Bridge of {@link Packer} for {@link InstanceForm repeated instances}.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class PackerForm
		extends samchon.protocol.Entity
	{
		/**
		 * Form of Instances to pack.
		 */
		private instanceFormArray: InstanceFormArray;

		/**
		 * Type of Wrappers to be used.
		 */
		private wrapperArray: WrapperArray;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor();

		/**
		 * Initializer Constructor.
		 * 
		 * @param instanceFormArray Form of Instances to pack.
		 * @param wrapperArray Type of Wrappers to be used.
		 */
		public constructor(instanceFormArray: InstanceFormArray, wrapperArray: WrapperArray);
		
		public constructor
			(
				instanceFormArray: InstanceFormArray = new InstanceFormArray(), 
				wrapperArray: WrapperArray = new WrapperArray()
			)
		{
			super();

			this.instanceFormArray = instanceFormArray;
			this.wrapperArray = wrapperArray;
		}

		public construct(xml: samchon.library.XML): void
		{
			this.instanceFormArray.construct(xml.get(this.instanceFormArray.TAG()).at(0));
			this.wrapperArray.construct(xml.get(this.wrapperArray.TAG()).at(0));
		}

		/* -----------------------------------------------------------
			ACCESSORS
		----------------------------------------------------------- */
		public optimize(): WrapperArray
		{
			let packer: Packer = this.toPacker();

			return packer.optimize();
		}

		public getInstanceFormArray(): InstanceFormArray
		{
			return this.instanceFormArray;
		}
		public getWrapperArray(): WrapperArray
		{
			return this.wrapperArray;
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		public TAG(): string
		{
			return "packerForm";
		}
		public toXML(): samchon.library.XML
		{
			let xml: samchon.library.XML = super.toXML();
			xml.push(this.instanceFormArray.toXML());
			xml.push(this.wrapperArray.toXML());

			return xml;
		}

		public toPacker(): Packer
		{
			let packer: Packer = new Packer(this.wrapperArray, this.instanceFormArray.toInstanceArray());
			return packer;
		}
	}

	/**
	 * An array of {@link InstanceForm} objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class InstanceFormArray
		extends samchon.protocol.EntityArrayCollection<InstanceForm>
	{
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();
		}
		public createChild(xml: samchon.library.XML): InstanceForm
		{
			return new InstanceForm();
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		public TAG(): string
		{
			return "instanceFormArray";
		}
		public CHILD_TAG(): string
		{
			return "instanceForm";
		}

		/**
		 * Convert {@link InstanceForm} objects to {@link InstanceArray}.
		 *
		 * @return An array of instance containing repeated instances in {@link InstanceForm} objects.
		 */
		public toInstanceArray(): InstanceArray
		{
			let instanceArray: InstanceArray = new InstanceArray();

			for (let i: number = 0; i < this.size(); i++)
			{
				let myInstances: InstanceArray = this.at(i).toInstanceArray();
				instanceArray.insert(instanceArray.end(), myInstances.begin(), myInstances.end());
			}

			return instanceArray;
		}
	}


	/**
	 * <p> A repeated Instance. </p>
	 *
	 * <p> InstanceForm is an utility class for repeated {@link Instance}. It is designed for shrinking 
	 * volume of network message I/O by storing {@link count repeated count}. </p>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class InstanceForm
		extends samchon.protocol.Entity
	{
		/**
		 * A duplicated Instance.
		 */
		private instance: Instance;

		/**
		 * Repeated count of the {@link instance}.
		 */
		private count: number;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor(instance: Instance = new Product("No name", 10, 10, 10), count: number = 1)
		{
			super();

			this.instance = instance;
			this.count = count;
		}

		/**
		 * @inheritdoc
		 */
		public construct(xml: samchon.library.XML): void
		{
			super.construct(xml);

			if (xml.hasProperty("type"))
			{
				this.instance = this.createInstance(xml);
				this.instance.construct(xml);
			}
			else if (xml.has("instance"))
			{
				let instanceXML: samchon.library.XML = xml.get("instance").at(0);

				this.instance = this.createInstance(instanceXML);
				this.instance.construct(instanceXML);
			}
		}

		private createInstance(xml: samchon.library.XML): Instance
		{
			if (xml.getProperty("type") == "product")
				return new Product();
			else
				return new Wrapper();
		}

		/* -----------------------------------------------------------
			ACCESSORS
		----------------------------------------------------------- */
		public key(): any
		{
			return this.instance.getName();
		}

		public getInstance(): Instance
		{
			return this.instance;
		}

		public getCount(): number
		{
			return this.count;
		}

		public setCount(val: number): void
		{
			this.count = val;
		}

		public get $name(): string		{	return this.instance.getName();				}
		public get $width(): string		{	return this.instance.getWidth() + "";		}
		public get $height(): string	{	return this.instance.getHeight() + "";		}
		public get $length(): string	{	return this.instance.getLength() + "";		}
		public get $count(): string		{	return this.count + "";						}

		public set $name(val: string)	{	this.instance.setName(val);					}
		public set $width(val: string)	{	this.instance.setWidth(parseFloat(val));	}
		public set $height(val: string)	{	this.instance.setHeight(parseFloat(val));	}
		public set $length(val: string)	{	this.instance.setLength(parseFloat(val));	}
		public set $count(val: string)	{	this.count = parseInt(val);					}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "instanceForm";
		}

		/**
		 * @inheritdoc
		 */
		public toXML(): samchon.library.XML
		{
			let xml = super.toXML();

			if (this.instance != null)
				xml.push(this.instance.toXML());

			return xml;
		}

		/**
		 * <p> Repeated {@link instance} to {@link InstanceArray}.
		 *
		 * @details 
		 * <p> Contains the {@link instance repeated instance} to an {@link InstanceArray} to make 
		 * {@link instance} to participate in the packing process. The returned {@link InstanceArray} will be 
		 * registered on {@link Packer.instanceArray}.
		 *
		 * @return An array of instance containing repeated {@link instance}.
		 */
		public toInstanceArray(): InstanceArray
		{
			let instanceArray: InstanceArray = new InstanceArray();
			instanceArray.assign(this.count, this.instance);
			
			return instanceArray;
		}
	}
}