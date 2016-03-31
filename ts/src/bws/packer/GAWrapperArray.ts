/// <reference path="WrapperArray.ts" />

namespace bws.packer
{
	export class GAWrapperArray
		extends WrapperArray
	{
		protected instanceArray: InstanceArray;

		protected result: std.HashMap<string, WrapperGroup>;

		private price: number;

		/**
		 * @brief Validity of this sequence list.
		 */
		private valid: boolean;

		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Construct from instances.
		 *
		 * @param instanceArray Instances to be wrapped.
		 */
		public constructor(instanceArray: InstanceArray);

		/**
		 * @brief Copy Constructor.
		 */
		public constructor(genes: GAWrapperArray);

		public constructor(obj: InstanceArray | GAWrapperArray)
		{
			super();

			this.result = new std.HashMap<string, WrapperGroup>();
			this.price = 0.0;

			if (obj instanceof InstanceArray)
			{
				this.instanceArray = <InstanceArray>obj;
			}
			else
			{
				let genes: GAWrapperArray = <GAWrapperArray>obj;

				this.instanceArray = genes.instanceArray;
				this.assign(genes.begin(), genes.end());
			}
		}

		private constructResult(): void
		{
			if (this.result.empty() == false)
				return; // IF RESULT IS ALREADY DEDUCTED

			// INSTANCE AND WRAPPER IS CORRESPOND, 1:1 RELATIONSHIP.
			for (let i: number = 0; i < this.size(); i++)
			{
				let wrapper: Wrapper = this.at(i);
				if (this.result.has(wrapper.key()) == false)
				{
					let wrapperGroup: WrapperGroup = new WrapperGroup(wrapper);
					this.result.set(wrapper.key(), wrapperGroup);
				}

				let wrapperGroup: WrapperGroup = this.result.get(wrapper.key());
				let instance: Instance = this.instanceArray.at(i);

				if (wrapperGroup.allocate(instance) == false)
				{
					// THE INSTANCE IS GREATER THAN THE WRAPPER
					// THIS GENE IS NOT VALID SO THAT CANNOT PARTICIPATE IN THE OPTIMIZATION PROCESS
					this.valid = false;
					return;
				}
			}

			// THE GENE IS VALID, THEN CALCULATE THE COST
			this.price = 0.0;
			this.valid = true;

			for (let it = this.result.begin(); !it.equals(this.result.end()); it = it.next())
			{
				it.second.optimize();
				this.price += it.second.getPrice();
			}
		}

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get optimization result.
		 *
		 * @return result map.
		 */
		public getResult(): std.HashMap<string, WrapperGroup>
		{
			this.constructResult();

			return this.result;
		}

		public less(obj: GAWrapperArray): boolean
		{
			this.constructResult();
			obj.constructResult();

			if (this.valid == true && obj.valid == true)
				return this.price < obj.price;
			else if (this.valid == true && obj.valid == false)
				return true;
			else
				return false;
		}
	}
}