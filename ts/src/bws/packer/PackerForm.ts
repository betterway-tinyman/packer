namespace bws.packer
{
	export class PackerForm
		extends samchon.protocol.Entity
	{
		private instanceFormArray: InstanceFormArray;
		private wrapperArray: WrapperArray;

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

		public optimize(): WrapperArray
		{
			let packer: Packer = this.toPacker();

			return packer.optimize();
		}

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

	class InstanceFormArray
		extends samchon.protocol.EntityArray<InstanceForm>
	{
		public constructor()
		{
			super();
		}
		protected createChild(xml: samchon.library.XML): InstanceForm
		{
			return new InstanceForm();
		}

		public TAG(): string
		{
			return "instanceFormArray";
		}
		public CHILD_TAG(): string
		{
			return "instanceForm";
		}

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

	class InstanceForm
		extends samchon.protocol.Entity
	{
		private instance: Instance;
		private count: number;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
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

		public toInstanceArray(): InstanceArray
		{
			let instanceArray: InstanceArray = new InstanceArray();
			instanceArray.assign(this.count, this.instance);
			
			return instanceArray;
		}
	}
}