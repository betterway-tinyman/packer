namespace bws.packer
{
	export class WrapperArray
		extends samchon.protocol.EntityArray<Wrapper>
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

		/**
		 * @inheritdoc
		 */
		public createChild(xml: samchon.library.XML): Wrapper
		{
			return new Wrapper();
		}

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Get (calculate) price.
		 */
		public getPrice(): number
		{
			let price: number = 0.0;
			for (let i: number = 0; i < this.size(); i++)
				price += this.at(i).getPrice();

			return price;
		}

		/**
		 * Get (calculate) utilization rate.
		 */
		public getUtilization(): number
		{
			if (this.empty() == true)
				return -1.0;

			let numerator: number = 0.0;
			let denominator: number = 0.0;

			for (let i: number = 0; i < this.size(); i++)
			{
				let wrapper: Wrapper = this.at(i);
				
				denominator += wrapper.getContainableVolume();
				for (let j = 0; j < wrapper.size(); j++)
					numerator += wrapper.getContainableVolume();
			}

			return numerator / denominator;
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "wrapperArray";
		}

		/**
		 * @inheritdoc
		 */
		public CHILD_TAG(): string
		{
			return "instance";
		}
	}
}