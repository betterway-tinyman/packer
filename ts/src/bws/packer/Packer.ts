namespace bws.packer
{
	/**
	 * A packer.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export class Packer
		extends samchon.protocol.Entity
	{
		/**
		 * Candidate wrappers who can contain instances.
		 */
		protected wrapperArray: WrapperArray;
		
		/**
		 * Instances trying to pack into the wrapper.
		 */
		protected instanceArray: InstanceArray;

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
		 * @param wrapperArray Candidate wrappers who can contain instances.
		 * @param instanceArray Instances to be packed into some wrappers.
		 */
		public constructor(wrapperArray: WrapperArray, instanceArray: InstanceArray);

		public constructor(wrapperArray: WrapperArray = null, instanceArray: InstanceArray = null)
		{
			super();

			if (wrapperArray == null && instanceArray == null)
			{
				this.wrapperArray = new WrapperArray();
				this.instanceArray = new InstanceArray();
			}
			else
			{
				this.wrapperArray = wrapperArray;
				this.instanceArray = instanceArray;
			}
		}

		/**
		 * @inheritdoc
		 */
		public construct(xml: samchon.library.XML): void
		{
			this.wrapperArray.construct(xml.get(this.wrapperArray.TAG()).at(0));
			this.instanceArray.construct(xml.get(this.instanceArray.TAG()).at(0));
		}

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Get wrapperArray.
		 */
		public getWrapperArray(): WrapperArray
		{
			return this.wrapperArray;
		}

		/**
		 * Get instanceArray.
		 */
		public getInstanceArray(): InstanceArray
		{
			return this.instanceArray;
		}

		/* -----------------------------------------------------------
			OPTIMIZERS
		----------------------------------------------------------- */
		public optimize(): WrapperArray
		{
			let wrappers: WrapperArray = new WrapperArray(); // TO BE RETURNED

			if (this.wrapperArray.size() == 1)
			{
				// ONLY A TYPE OF WRAPPER EXISTS,
				// OPTMIZE IN LEVEL OF WRAPPER_GROUP AND TERMINATE THE OPTIMIZATION
				let wrapperGroup: WrapperGroup = new WrapperGroup(this.wrapperArray.front());

				for (let i: number = 0; i < this.instanceArray.size(); i++)
					if (wrapperGroup.allocate(this.instanceArray.at(i)) == false)
						return null;

				// OPTIMIZE
				wrapperGroup.optimize();

				// ASSIGN WRAPPERS
				wrappers.assign(wrapperGroup.begin(), wrapperGroup.end());
			}
			else
			{
				////////////////////////////////////////
				// WITH GENETIC_ALGORITHM
				////////////////////////////////////////
				// CONSTRUCT INITIAL SET
				let geneArray: GAWrapperArray = this.initGenes();
				
				// EVOLVE
				// IN JAVA_SCRIPT VERSION, GENETIC_ALGORITHM IS NOT IMPLEMENTED YET.
				// HOWEVER, IN C++ VERSION, IT IS FULLY SUPPORTED
				//	http://samchon.github.io/framework/api/cpp/d5/d28/classsamchon_1_1library_1_1GeneticAlgorithm.html
				// IT WILL BE SUPPORTED SOON

				// FETCH RESULT
				let result: std.HashMap<string, WrapperGroup> = geneArray.getResult();
				for (let it = result.begin(); !it.equals(result.end()); it = it.next())
					wrappers.insert(wrappers.end(), it.second.begin(), it.second.end());

				// TRY TO REPACK
				wrappers = this.repack(wrappers);
			}

			// SORT THE WRAPPERS BY ITEMS' POSITION
			for (let i: number = 0; i < wrappers.size(); i++)
				std.sort(wrappers[i].begin(), wrappers[i].end(),
					function (left: Wrap, right: Wrap): boolean
					{
						if (left.getZ() != right.getZ())
							return left.getZ() < right.getZ();
						else if (left.getY() != right.getY())
							return left.getY() < right.getY();
						else
							return left.getX() < right.getX();
					}
				);

			return wrappers;
		}

		/**
		 * Initialize sequence list (gene_array).
		 *
		 * @return Initial sequence list.
		 */
		protected initGenes(): GAWrapperArray
		{
			////////////////////////////////////////////////////
			// LINEAR OPTIMIZATION
			////////////////////////////////////////////////////
			// CONSTRUCT WRAPPER_GROUPS
			let wrapperGroups: std.Vector<WrapperGroup> = new std.Vector<WrapperGroup>();

			for (let i: number = 0; i < this.wrapperArray.size(); i++)
			{
				let wrapper: Wrapper = this.wrapperArray.at(i);

				wrapperGroups.pushBack(new WrapperGroup(wrapper));
			}

			// ALLOCATE INSTNACES BY AUTHORITY
			for (let i: number = 0; i < this.instanceArray.size(); i++)
			{
				let instance: Instance = this.instanceArray.at(i);
				let minCost: number = Number.MAX_VALUE;
				let minIndex: number = 0;

				for (let j: number = 0; j < this.wrapperArray.size(); j++)
				{
					let wrapper: Wrapper = this.wrapperArray.at(j);
					if (wrapper.containable(instance) == false)
						continue; // CANNOT CONTAIN BY ITS GREATER SIZE

					let cost = wrapper.getPrice() / wrapper.getContainableVolume();
					if (cost < minCost)
					{
						// CURRENT WRAPPER'S PRICE PER UNIT VOLUME IS CHEAPER
						minCost = cost;
						minIndex = j;
					}
				}

				// ALLOCATE TO A GROUP WHICH HAS THE MOST CHEAPER PRICE PER UNIT VOLUME
				let wrapperGroup: WrapperGroup = wrapperGroups.at(minIndex);
				wrapperGroup.allocate(instance);
			}

			////////////////////////////////////////////////////
			// ADDICTIONAL OPTIMIZATION BY POST-PROCESS
			////////////////////////////////////////////////////0
			// OPTIMIZE WRAPPER_GROUP
			let wrappers: WrapperArray = new WrapperArray();
			
			for (let i: number = 0; i < wrapperGroups.size(); i++)
			{
				let wrapperGroup: WrapperGroup = wrapperGroups.at(i);

				wrapperGroup.optimize();
				wrappers.insert(wrappers.end(), wrapperGroup.begin(), wrapperGroup.end());
			}

			// DO EARLY POST-PROCESS
			wrappers = this.repack(wrappers);

			////////////////////////////////////////////////////
			// CONSTRUCT GENE_ARRAY
			////////////////////////////////////////////////////
			// INSTANCES AND GENES
			let ga_instances: InstanceArray = new InstanceArray();
			let genes: WrapperArray = new WrapperArray();

			for (let i: number = 0; i < wrappers.size(); i++)
			{
				let wrapper: Wrapper = wrappers.at(i);

				for (let j: number = 0; j < wrapper.size(); j++)
				{
					ga_instances.pushBack(wrapper.at(j).getInstance());
					genes.pushBack(wrapper);
				}
			}

			// GENE_ARRAY
			let geneArray: GAWrapperArray = new GAWrapperArray(ga_instances);
			geneArray.assign(genes.begin(), genes.end());

			return geneArray;
		}

		/**
		 * Try to repack each wrappers to another type.
		 *
		 * @param $wrappers Wrappers to repack.
		 * @return Re-packed wrappers.
		 */
		protected repack($wrappers: WrapperArray): WrapperArray
		{
			let result: WrapperArray = new WrapperArray();

			for (let i: number = 0; i < $wrappers.size(); i++)
			{
				let wrapper: Wrapper = $wrappers.at(i);
				let minGroup: WrapperGroup = new WrapperGroup(wrapper);

				minGroup.pushBack(wrapper);

				for (let j: number = 0; j < this.wrapperArray.size(); j++)
				{
					let myWrapper: Wrapper = this.wrapperArray.at(j);
					if (wrapper.equals(myWrapper))
						continue;

					let valid: boolean = true;

					// CONSTRUCT GROUP OF TARGET
					let myGroup: WrapperGroup = new WrapperGroup(myWrapper);

					for (let k: number = 0; k < wrapper.size(); k++)
						if (myGroup.allocate(wrapper.at(k).getInstance()) == false)
						{
							// IF THERE'S AN INSTANCE CANNOT CONTAIN BY ITS GREATER SIZE
							valid = false;
							break;
						}

					// SKIP
					if (valid == false)
						continue;

					// OPTIMIZATION IN LEVEL OF GROUP
					myGroup.optimize();

					// CURRENT GROUP IS CHEAPER, THEN REPLACE
					if (myGroup.getPrice() < minGroup.getPrice())
						minGroup = myGroup;
				}

				result.insert(result.end(), minGroup.begin(), minGroup.end());
			}
			return result;
		}

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public TAG(): string
		{
			return "packer";
		}

		/**
		 * @inheritdoc
		 */
		public toXML(): samchon.library.XML
		{
			let xml: samchon.library.XML = super.toXML();
			xml.push(this.wrapperArray.toXML());
			xml.push(this.instanceArray.toXML());

			return xml;
		}
	}
}