namespace bws.packer
{
	/**
	 * A group of {@link Wrapper Wrappers} with same type.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class WrapperGroup
		extends WrapperArray
	{
		/**
		 * <p> A sample, standard of the WrapperGroup. </p>
		 *
		 * <p> The sample represents what type of Wrappers are grouped into the WrapperGroup. </p>
		 */
		protected sample: Wrapper;

		/**
		 * Allocated instances.
		 */
		protected allocatedInstanceArray: InstanceArray;

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
		public constructor(wrapperGroup: WrapperGroup);

		/**
		 * Construct from a sample.
		 *
		 * @param sample A sample, standard of the WrapperGroup.
		 */
		public constructor(sample: Wrapper);
		
		/**
		 * Construct from members of the {@link sample}. 
		 *
		 * @param name Name, identifier of the sample.
		 * @param price Price, issued cost for a type of the sample.
		 * @param width Width, dimensional length on the X-axis in 3D, of the sample.
		 * @param height Height, dimensional length on the Y-axis in 3D, of the sample.
		 * @param length Length, dimensional length on the Z-axis in 3D, of the sample.
		 * @param thickness A thickness, causes shrinkness on containable volume, of the sample.
		 */
		public constructor
		(
			name: string, price: number,
			width: number, height: number, length: number, thickness: number
		);

		public constructor(...args: any[])
		{
			super();

			if (args.length == 0)
			{
				this.sample = new Wrapper();
			}
			else if (args.length == 1 && args[0] instanceof Wrapper)
			{
				this.sample = args[0];
			}
			else if (args.length == 6)
			{
				this.sample = new Wrapper(args[0], args[1], args[2], args[3], args[4], args[5]);
			}

			this.allocatedInstanceArray = new InstanceArray();
		}

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Key of a WrapperGroup is dependent on its sample.
		 */
		public key(): any
		{
			return this.sample.key();
		}

		/**
		 * Get sample.
		 */
		public getSample(): Wrapper
		{
			return this.sample;
		}

		/**
		 * Get allocated instances.
		 */
		public getAllocatedInstanceArray(): InstanceArray
		{
			return this.allocatedInstanceArray;
		}

		/**
		 * Get (calculate) price.
		 *
		 * @return (Price of the sample) x (numbers of children Wrappers)
		 */
		public getPrice(): number
		{
			return this.sample.getPrice() * this.size();
		}

		/**
		 * @inheritdoc
		 */
		public getUtilization(): number
		{
			let utilization: number = 0.0;
			for (let i = 0; i < this.size(); i++)
				utilization += this.at(i).getUtilization();

			return utilization / this.size();
		}

		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		/**
		 * <p> Allocate instance(s) to the WrapperGroup. </p>
		 *
		 * <p> Inspect the instance is enough small to be wrapped into an empty wrapper. If the instance 
		 * is enough small, registers the instance (or repeated instances) to the {@link reserveds} and 
		 * returns <code>true</code>. If the instance is too large to be capsuled, returns <code>false</code>. </p>
		 *
		 * <h4>Note</h4>
		 * <p> The word <u>the instance is enough small to be wrapped into the empty wrapper</u> means 
		 * the instance can be contained into an empty, a new wrapper contaning nothing literally. </p>
		 *
		 * <p> In the method allocate(), it doesn't consider how many instances are wrapped into ordinary 
		 * wrapper and how much volumes are consumed.  </p>
		 * 
		 * @param instance An Instance to allocate.
		 * @param n Repeating number of the <i>instance</i>.
		 *
		 * @return Whether the instance is enough small to be wrapped into a (new) wrapper
		 *		   of same type with the sample.
		 */
		public allocate(instance: Instance, n: number = 1): boolean
		{
			// TEST WHETHER A PRODUCT IS NOT LARGER THAN BOX
			if (this.sample.containable(instance) == false)
				return false;

			// INSERTS TO THE RESERVED ITEMS
			this.allocatedInstanceArray.insert
			(
				this.allocatedInstanceArray.end(), 
				n, instance
			);
			return true;
		}

		/**
		 * <p> Run optimization in level of the group. </p>
		 *
		 * <p> The optimization routine begins by creating a {@link Wrapper} like the {@link sample}. Then 
		 * try to pack {@link allocatedInstanceArray allocated instances} to the {@link Wrapper} as a lot as 
		 * possible. If there're some {@link Wrappers} can't be packed by overloading, then create a new 
		 * {@link Wrapper} again and try to pack {@link allocatedInstanceArray instances} again, too. </p>
		 *
		 * <p> Repeats those steps until all {@link alloctedInstanceArray instances} are {@link Wrap packed} 
		 * so that there's not any {@link Instance instance} left. </p>
		 *
		 * <h4> Warning </h4>
		 * <p> When call this {@link optimize optimize()} method, ordinary children {@link Wrapper} objects 
		 * in the {@link WrapperGroup} will be substituted with the newly optimized {@link Wrapper} objects. </p>
		 */
		public optimize(): void
		{
			this.clear();

			let instanceArray: InstanceArray = new InstanceArray();
			instanceArray.assign(this.allocatedInstanceArray.begin(), this.allocatedInstanceArray.end());
			
			while (instanceArray.empty() == false)
				instanceArray = this.pack(instanceArray);
		}

		/**
		 * <p> Wrap allocated instances into <b>a new</b> {@link Wrapper}. </p>
		 *
		 * <p> {@link Wrap Wraps} instances to a new Wrapper which is copied from the sample. </p>
		 * <p> After the wrapping is done, the new {@link Wrapper} is registered to the {@link WrapperGroup} 
		 * as a child and instances failed to wrap by overloading is returned. </p>
		 * 
		 * @param instanceArray instances to {@link Wrap wrap} into <b>a new</b> {@link Wrapper}.
		 * 
		 * @return Instances failed to {@link Wrap wrap} by overloading.
		 * @see boxologic
		 */
		private pack(instanceArray: InstanceArray): InstanceArray
		{
			let boxo: boxologic.Boxologic = new boxologic.Boxologic(new Wrapper(this.sample), instanceArray);
			let resultPair: std.Pair<Wrapper, InstanceArray> = boxo.pack();

			this.push_back(resultPair.first);
			return resultPair.second;
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "wrapperGroup";
		}
	}
}