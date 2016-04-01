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
		private instanceFormArray: InstanceFormArray;
		private wrapperArray: WrapperArray;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();

			this.instanceFormArray = new InstanceFormArray();
			this.wrapperArray = new WrapperArray();
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
	class InstanceFormArray
		extends samchon.protocol.EntityArray<InstanceForm>
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
		protected createChild(xml: samchon.library.XML): InstanceForm
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
				//for (let j: number = 0; j < myInstances.size(); j++)
				//	instanceArray.pushBack(myInstances.at(j));


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
	class InstanceForm
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
		public constructor()
		{
			super();

			this.instance = null;
			this.count = 1;
		}

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
			EXPORTERS
		----------------------------------------------------------- */
		public TAG(): string
		{
			return "instanceForm";
		}
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