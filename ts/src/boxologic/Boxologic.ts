/**
 * <p> A set of programs that calculate the best fit for boxes on a pallet migrated from language C. </p>
 *
 * <ul>
 *	<li> Original Boxologic: https://github.com/exad/boxologic </li>
 * </ul>
 * 
 * @author Bill Knechtel, <br>
 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
 */
namespace boxologic
{
	/**
	 * <p> A facade class of boxologic. </p>
	 *
	 * <p> The Boxologic class dudcts the best solution of packing boxes to a pallet. </p>
	 * 
	 * <ul>
	 *	<li> Reference: https://github.com/exad/boxologic </li>
	 * </ul>
	 *
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 */
	export class Boxologic
	{
		/* ===========================================================
			PARAMETRIC DATA
		=========================================================== */
		/**
		 * A Wrapper to pack instances.
		 */
		private wrapper: bws.packer.Wrapper;

		/**
		 * Instances trying to put into the wrapper.
		 */
		private instanceArray: bws.packer.InstanceArray;

		/**
		 * Instances failed to pack by overloading.
		 */
		private leftInstances: bws.packer.InstanceArray;

		/* ===========================================================
			BACKGROUND DATA
				- STRUCTURES
				- VARIABLES FOR ITERATION
				- FLAGS FOR TERMINATING ITERATION
				- THE BEST
		==============================================================
			STRUCTURES
		----------------------------------------------------------- */
		/**
		 * A pallet containing {@link Box boxes}.
		 *
		 * @see Wrapper
		 */
		private pallet: Pallet;

		/** 
		 * Boxes, trying to pack into the {@link pallet}.
		 */
		private box_array: std.Vector<Box>;

		/**
		 * Sum of all boxes' volume.
		 */
		private total_box_volume: number;

		/**
		 * <p> All different lengths of {@link box_array all box} dimensions along with evaluation values. </p>
		 * 
		 * <p> In other word, the <i>layer_map</i> stores those entries; each {@link Boxbox}'s length on each 
		 * axis as a <i>key</i> (width, height or length) and evaluation value as a <i>value</i>. The evaluation 
		 * value means sum of minimum gaps between the key and other {@link Box boxes}' width, height and length 
		 * </p>
		 *
		 * <code>
		 FOR i := 0 to box_array.size()
			 WHILE key IN width, length and height in box_array[i]
			 BEGIN
		 		value := 0
		 		FOR j to box_array.size()
		 			value += min
		 			(
		 				abs(key - box_array[j].width), 
		 				abs(key - box_array[j].height), 
		 				abs(key - box_array[j].length)
		 			)
		 			layer_map.insert({key, value});
			END
		 * </code>
		 * 
		 * <ul>
		 *	<li> key: A dimension value </li>
		 *	<li> value: Evaluation weight value for the corresponding key. </li>
		 * </ul>
		 */
		private layer_map: std.HashMap<number, number>;

		/**
		 * {@link List} of {@link Scrapped} instances, edges of layers under construction.
		 *
		 * @see Scrapped
		 * @see scrap_min_z
		 */
		private scrap_list: std.base.ILinearContainer<Scrap>;

		/**
		 * The topology {@link Scrapped}, the edge of the current layer under construction.
		 *
		 * @see Scrapped
		 * @see scrap_list
		 */
		private scrap_min_z: std.base.Iterator<Scrap>;

		/* -----------------------------------------------------------
			VARIABLES FOR ITERATIONS
				- BOX
				- LAYER
				- REMAINS & PACKED
				- FINDER & ANALYZER
		--------------------------------------------------------------
		//	BOX
		/////////////////////////////////////////////////////////// */
		/**
		 * Index of the current {@link box}.
		 */
		private cboxi: number;

		/**
		 * Candidate {@link Box.layout_width layout_width} of the {@link cboxi current box}.
		 */
		private cbox_layout_width: number;
		
		/**
		 * Candidate {@link Box.layout_height layout_height} of the {@link cboxi current box}.
		 */
		private cbox_layout_height: number;
		
		/**
		 * Candidate {@link Box.layout_length layout_length} of the {@link cboxi current box}.
		 */
		private cbox_layout_length: number;

		//////////////////////////////////////////////////////////////
		//	LAYER
		//////////////////////////////////////////////////////////////
		/**
		 * Current layer's key on iteration.
		 */
		private layer_thickness: number;

		/**
		 * Previous layer's key had iterated.
		 */
		private pre_layer: number;

		/**
		 * Key of the unevened layer in the current packing layer.
		 */
		private layer_in_layer: number;
		
		/**
		 * Little Z, gotten from {@link Scrapped.cumz cumz} in {@link min_scrap_z}
		 */
		private lilz: number;

		//////////////////////////////////////////////////////////////
		//	REMAINS & PACKED
		//////////////////////////////////////////////////////////////
		/**
		 * Remained (unfilled) {@link Pallet.layout_height layout_height} of the {@link pallet}.
		 */
		private remain_layout_height: number;

		/**
		 * Remained (unfilled) {@link Pallet.layout_length layout_length} of the {@link pallet}.
		 */
		private remain_layout_length: number;

		/**
		 * Packed (filled) {@link Pallet.layout_height layout_height} of the {@link pallet}.
		 */
		private packed_layout_height: number;

		/**
		 * Packed {@link Pallet.vo1lume volume} of the {@lnk pallet}.
		 */
		private packed_volume: number;

		//////////////////////////////////////////////////////////////
		//	FINDER & ANALYZER
		//////////////////////////////////////////////////////////////
		private boxi: number;
		private bboxi: number;

		private boxx: number;
		private boxy: number;
		private boxz: number;

		private bboxx: number;
		private bboxy: number;
		private bboxz: number;

		private bfx: number;
		private bfy: number;
		private bfz: number;
		private bbfx: number;
		private bbfy: number;
		private bbfz: number;

		/* -----------------------------------------------------------
			FLAGS FOR TERMINATING ITERATION
		----------------------------------------------------------- */
		/**
		 * <p> Whether the packing is on progress. </p>
		 *
		 * <p> The {@link packing} is a flag variable for terminating iterations in 
		 * {@link iterate_orientations iterate_orientations()}, who deducts the best packing solution. </p>
		 */
		private packing: boolean;
		
		/**
		 * Whether packing a layer is done.
		 */
		private layer_done: boolean;
		
		/**
		 * Whether the current packing layer is evened.
		 */
		private evened: boolean;
		
		/**
		 * Whether the best solution is deducted.
		 */
		private packing_best: boolean;

		/**
		 * Whether the utilization degree of pallet space is 100%.
		 */
		private hundred_percent: boolean;

		/**
		 * The best orientation of the pallet, which can deduct the {@link best_solution_volume}.
		 */
		private best_orientation: number;
		
		/**
		 * The best layer, which can deduct the {@link best_solution_volume}.
		 */
		private best_layer: number;
		
		/**
		 * The best volume, fit the best utilization degree of the pallet space.
		 */
		private best_solution_volume: number;

		/* ===========================================================
			CONSTRUCTORS
				- CONSTRUCTOR
				- ENCODER & DECODER
		==============================================================
			CONSTRUCTOR
		----------------------------------------------------------- */
		/**
		 * Construct from a wrapper and instances. 
		 *
		 * @param wrapper A Wrapper to pack instances.
		 * @param instanceArray Instances trying to put into the wrapper.
		 */
		public constructor(wrapper: bws.packer.Wrapper, instanceArray: bws.packer.InstanceArray)
		{
			this.wrapper = wrapper;
			this.instanceArray = instanceArray;

			this.leftInstances = new bws.packer.InstanceArray();
		}

		/* -----------------------------------------------------------
			ENCODER & DECODER
		----------------------------------------------------------- */
		/**
		 * <p> Encode data </p>
		 *
		 * <p> Encodes {@link bws.packer Packer}'s data to be suitable for the 
		 * {@link boxologic Boxologic}'s parametric data. </p>
		 */
		private encode(): void
		{
			/////////////////////////////////////
			// STRUCTURES
			/////////////////////////////////////
			this.pallet = new Pallet(this.wrapper);
			this.box_array = new std.Vector<Box>();
			this.total_box_volume = 0.0;

			this.layer_map = new std.HashMap<number, number>();
			this.scrap_list = new std.Vector<Scrap>();

			// CHILDREN ELEMENTS - BOX
			this.box_array.assign(this.instanceArray.size(), null);
			for (let i: number = 0; i < this.instanceArray.size(); i++)
			{
				let box: Box = new Box(this.instanceArray.at(i));

				this.total_box_volume += box.volume;
				this.box_array[i] = box;
			}

			// SCRAP_LIST
			this.scrap_list.push_back(new Scrap());

			/////////////////////////////////////
			// BEST VARIABLES
			/////////////////////////////////////
			this.best_solution_volume = 0.0;
			this.packing_best = false;
			this.hundred_percent = false;
		}

		/**
		 * <p> Decode data </p>
		 *
		 * <p> Decodes the Boxologic's optimization result data to be suitable for the Packer's own. </p>
		 */
		private decode(): void
		{
			this.wrapper.clear();
			this.leftInstances.clear();
			
			for (let i: number = 0; i < this.box_array.size(); i++)
			{
				let instance: bws.packer.Instance = this.instanceArray.at(i);
				let box: Box = this.box_array.at(i);

				if (box.is_packed == true)
				{
					let wrap: bws.packer.Wrap = 
						new bws.packer.Wrap
						(
							this.wrapper, instance,
							box.cox, box.coy, box.coz
						);
					wrap.estimateOrientation(box.layout_width, box.layout_height, box.layout_length);

					if (this.wrapper.getThickness() != 0)
						wrap.setPosition
						(
							wrap.getX() + this.wrapper.getThickness(),
							wrap.getY() + this.wrapper.getThickness(),
							wrap.getZ() + this.wrapper.getThickness()
						);

					this.wrapper.push_back(wrap);
				}
				else
				{
					// NOT WRAPED INSTANCES BY LACK OF VOLUME
					this.leftInstances.push_back(instance);
				}
			}
		}

		/* ===========================================================
			MAIN PROCEDURES
				- OPERATORS
				- CHECKERS
				- GETTERS
				- REPORTERS
		==============================================================
			OPERATORS
		------------------------------------------------------------ */
		/**
		 * <p> Pack instances to the {@link wrapper}. </p>
		 *
		 * <p> The {@link Boxologic.pack} is an adaptor method between {@link bws.packer Packer} and 
		 * {@link boxologic}. It encodes data from {@link bws.packer Packer}, deducts the best packing 
		 * solution decodes the optimization result and returns it. </p>
		 *
		 * <p> The optimization result is returned as a {@link Pair} like below: </p>
		 * <ul>
		 *	<li> first: The {@link wrapper} with packed instances. </li>
		 *	<li> second: {@link leftInstances Left instances failed to pack} by overloading. </li>
		 * </ul>
		 *
		 * @return A pair of {@link wrapper} with packed instances and 
		 *		   {@link leftInstances instances failed to pack} by overloading.
		 */
		public pack(): std.Pair<bws.packer.Wrapper, bws.packer.InstanceArray>
		{
			this.encode();

			this.iterate_orientations();
			this.report_results();
			
			this.decode();

			return new std.Pair<bws.packer.Wrapper, bws.packer.InstanceArray>(this.wrapper, this.leftInstances);
		}
		
		/**
		 * <p> Execute iterations by calling proper functions. </p>
		 *
		 * <p> Iterations are done and parameters of the best solution are found. </p>
		 */
		private iterate_orientations(): void
		{
			for (let orientation: number = 1; orientation <= 6; orientation++)
			{
				this.pallet.set_orientation(orientation);

				// CONSTRUCT LAYERS
				this.construct_layers();

				// ITERATION IN LAYERS
				for (let it = this.layer_map.begin(); !it.equal_to(this.layer_map.end()); it = it.next())
				{
					// BEGINS PACKING
					this.iterate_layer(it.first);

					if (this.packed_volume > this.best_solution_volume)
					{
						// NEW VOLUME IS THE BEST
						this.best_solution_volume = this.packed_volume;
						this.best_orientation = orientation;
						this.best_layer = it.first;
					}

					if (this.hundred_percent)
						break; // SUCCESS TO UTILIZE ALL
				}
		
				if (this.hundred_percent)
					break; // SUCCESS TO UTILIZE ALL

				// IF THE PALLET IS REGULAR CUBE,
				if (this.pallet.width == this.pallet.height && this.pallet.height == this.pallet.length)
					orientation = 6; // DON'T ITERATE ALL ORIENTATIONS
			}
		}

		/**
		 * Iterate a layer.
		 * 
		 * @param thickness Thickness of the iterating layer.
		 */
		private iterate_layer(thickness: number): void
		{
			// INIT PACKED
			this.packing = true;

			this.packed_volume = 0.0;
			this.packed_layout_height = 0;
			this.layer_thickness = thickness;

			// SET REMAINS FROM PALLET'S DIMENSIONS
			this.remain_layout_height = this.pallet.layout_height;
			this.remain_layout_length = this.pallet.layout_length;

			// UNPACK ALL BOXES
			for (let i: number = 0; i < this.box_array.size(); i++)
				this.box_array[i].is_packed = false;

			do
			{
				// INIT VARS OF LAYER ITERATION
				this.layer_in_layer = 0;
				this.layer_done = false;

				// PACK_LAYER AND POST-PROCESS
				this.pack_layer();

				this.packed_layout_height += this.layer_thickness;
				this.remain_layout_height = this.pallet.layout_height - this.packed_layout_height;
		
				if (this.layer_in_layer != 0)
				{
					// STORE ORDINARY PACKING VARS
					let pre_packed_y: number = this.packed_layout_height;
					let pre_remain_py: number = this.remain_layout_height;

					// STORE CAUCLATED RESULTS
					this.remain_layout_height = this.layer_thickness - this.pre_layer;
					this.packed_layout_height -= this.layer_thickness + this.pre_layer;
					this.remain_layout_length = this.lilz;
					this.layer_thickness = this.layer_in_layer;

					// ITERATION IS NOT FINISHED YET
					this.layer_done = false;

					// RE-CALL PACK_LAYER
					this.pack_layer();

					// REVERT TO THE STORED ORDINARIES
					this.packed_layout_height = pre_packed_y;
					this.remain_layout_height = pre_remain_py;
					this.remain_layout_length = this.pallet.layout_length;
				}

				// CALL FIND_LAYER
				this.find_layer(this.remain_layout_height);
			} 
			while (this.packing);
		}

		/**
		 * <p> Construct layers. </p>
		 * 
		 * <p> Creates all possible layer heights by giving a weight value to each of them. </p>
		 */
		private construct_layers(): void
		{
			this.layer_map.clear();

			for (let i: number = 0; i < this.box_array.size(); i++)
			{
				let box: Box = this.box_array[i];

				for (let j: number = 1; j <= 3; j++)
				{
					let ex_dim: number; // STANDARD LENGTH ON THE DIMENSION
					let dimen2: number; // THE SECOND, LENGTH ON A RESIDUAL DIMENSION
					let dimen3: number; // THE THIRD, LENGTH ON A RESIDUAL DIMENSION

					let layer_eval: number = 0; // SUM OF LAYERS (height)

					// FETCH STANDARD DIMENSIONS FROM EACH AXIS
					switch (j)
					{
					case 1:
						ex_dim = box.width;
						dimen2 = box.height;
						dimen3 = box.length;
						break;
					case 2:
						ex_dim = box.height;
						dimen2 = box.width;
						dimen3 = box.length;
						break;
					case 3:
						ex_dim = box.length;
						dimen2 = box.width;
						dimen3 = box.height;
						break;
					}

					// A DIMENSIONAL LENGTH IS GREATER THAN THE PALLET ?
					if (ex_dim > this.pallet.layout_height || 
						(
							(dimen2 > this.pallet.layout_width || dimen3 > this.pallet.layout_length) &&
							(dimen3 > this.pallet.layout_width || dimen2 > this.pallet.layout_length)
						))
					{
						// A DIMENSIONAL LENGTH IS GREATER THAN THE PALLET
						continue;
					}

					// WHEN A DUPLICATED LAYER EXISTS, SKIPS
					if (this.layer_map.has(ex_dim) == true)
						continue;

					// ABOUT ALL BOXES, FIND THE MINIMUM LENGTH OF GAP ~,
					// STACK ON THE CURRENT LAYER (ADD ON TO THE LAYER_EVAL)
					for (let k: number = 0; k < this.box_array.size(); k++)
					{
						// SAME INSTANCE WITH THE SAME INDEX
						if (i == k)
							continue;

						let my_box: Box = this.box_array[k];
						let dim_diff: number = Math.min
							(
								Math.abs(ex_dim - my_box.width),
								Math.abs(ex_dim - my_box.height),
								Math.abs(ex_dim - my_box.length)
							);

						layer_eval += dim_diff;
					}

					// RECORD THE SUM
					this.layer_map.set(ex_dim, layer_eval);
				}
			}
		}

		/**
		 * <p> Packs the boxes found and arranges all variables and records properly. </p>
		 *
		 * <p> Update the linked list and the Boxlist[] array as a box is packed. </p>
		 */
		private pack_layer(): void
		{
			if (this.layer_thickness == 0)
			{
				this.packing = false;
				return;
			}
			else if (this.scrap_list.empty() == true)
				return;

			let lenx: number;
			let lenz: number;
			let lpz: number;

			this.scrap_list.begin().value.cumx = this.pallet.layout_width;
			this.scrap_list.begin().value.cumz = 0;

			while (true)
			{
				// INIT SCRAP_MIN_Z
				this.find_smallest_z();

				// FETCH LEFT AND RIGHT OF SCRAP_MIN_Z
				let prev = this.scrap_min_z.prev();
				let next = this.scrap_min_z.next();

				if (this.scrap_min_z.equal_to(this.scrap_list.end()))
				{
					break;
				}
				if (prev.equal_to(this.scrap_list.end()) && next.equal_to(this.scrap_list.end()))
				{
					/////////////////////////////////////////////////////////
					// NO LEFT AND RIGHT
					/////////////////////////////////////////////////////////
					//*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***
					lenx = this.scrap_min_z.value.cumx;
					lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;

					// CALL FIND_BOX AND CHECK_FOUND
					this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lpz, lpz);
					this.check_found();

					// BREAK ?
					if (this.layer_done) break;
					if (this.evened) continue;

					// UPDATE CURRENT BOX
					let box: Box = this.box_array[this.cboxi];
					box.cox = 0;
					box.coy = this.packed_layout_height;
					box.coz = this.scrap_min_z.value.cumz;
			
					if (this.cbox_layout_width == this.scrap_min_z.value.cumx)
					{
						// CUMULATE
						this.scrap_min_z.value.cumz += this.cbox_layout_length;
					}
					else
					{
						// CREATE A NEW NODE AND IT'S THE NEW MIN_Z
						// ORDINARY MIN_Z WILL BE SHIFTED TO THE RIGHT
						let scrap: Scrap = new Scrap
						(
							this.cbox_layout_width,
							this.scrap_min_z.value.cumz + this.cbox_layout_length
						);

						// SHIFTS ORDINARY MIN_Z TO RIGHT
						// AND THE NEW NODE'S ITERATOR IS THE NEW MIN_Z FROM NOW ON
						this.scrap_min_z = this.scrap_list.insert(this.scrap_min_z, scrap);
					}
				}
				else if (prev.equal_to(this.scrap_list.end()))
				{
					/////////////////////////////////////////////////////////
					// NO LEFT, BUT RIGHT
					/////////////////////////////////////////////////////////
					//*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***
					lenx = this.scrap_min_z.value.cumx;
					lenz = next.value.cumz - this.scrap_min_z.value.cumz;
					lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;

					// CALL FIND_BOX AND CHECK_FOUND
					this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
					this.check_found();

					// BREAK ?
					if (this.layer_done) break;
					if (this.evened) continue;

					// RE-FETCH LEFT AND RIGHT
					next = this.scrap_min_z.next();

					// UPDATE CURRENT BOX
					let box: Box = this.box_array[this.cboxi];
					box.coy = this.packed_layout_height;
					box.coz = this.scrap_min_z.value.cumz;

					if (this.cbox_layout_width == this.scrap_min_z.value.cumx)
					{
						box.cox = 0;

						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz)
						{
							// RIGHT IS THE NEW MIN_Z
							// ORDINARY MIN_Z WILL BE ERASED
							this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
						}
						else
						{
							// CUMULATE
							this.scrap_min_z.value.cumz += this.cbox_layout_length;
						}
					}
					else
					{
						box.cox = this.scrap_min_z.value.cumx - this.cbox_layout_width;

						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz)
						{
							// DE-CUMULATE
							this.scrap_min_z.value.cumx -= this.cbox_layout_width;
						}
						else
						{
							// UPDATE MIN_Z
							this.scrap_min_z.value.cumx -= this.cbox_layout_width;

							// CREATE A NEW NODE BETWEEN MIN_Z AND RIGHT
							let scrap: Scrap = new Scrap
							(
								this.scrap_min_z.value.cumx,
								this.scrap_min_z.value.cumz + this.cbox_layout_length
							);
							this.scrap_list.insert(next, scrap);
						}
					}
				}
				else if (next.equal_to(this.scrap_list.end()))
				{
					////////////////////////////////////////////////////////
					// NO RIGHT BUT LEFT
					/////////////////////////////////////////////////////////
					//*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***
					lenx = this.scrap_min_z.value.cumx - prev.value.cumx;
					lenz = prev.value.cumz - this.scrap_min_z.value.cumz;
					lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;

					// CALL FIND_BOX AND CHECK_FOUND
					this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
					this.check_found();

					// BREAK ?
					if (this.layer_done) break;
					if (this.evened) continue;

					// RE-FETCH LEFT AND RIGHT
					prev = this.scrap_min_z.prev();

					// UPDATE CURRENT BOX
					let box: Box = this.box_array[this.cboxi];
					box.coy = this.packed_layout_height;
					box.coz = this.scrap_min_z.value.cumz;
					box.cox = prev.value.cumx;

					if (this.cbox_layout_width == this.scrap_min_z.value.cumx - prev.value.cumx)
					{
						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz)
						{
							// LEFT FETCHES MIN_Z'S CUM_X
							prev.value.cumx = this.scrap_min_z.value.cumx;

							// ERASE FROM MIN_Z TO END
							this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, this.scrap_list.end());
						}
						else
						{
							// CUMULATE
							this.scrap_min_z.value.cumz += this.cbox_layout_length;
						}
					}
					else
					{
						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz)
						{
							// CUMULATE
							prev.value.cumx += this.cbox_layout_width;
						}
						else
						{
							// CREATE A NEW NODE BETWEEN LEFT AND MIN_Z
							let scrap: Scrap = new Scrap
							(
								prev.value.cumx + this.cbox_layout_width,
								this.scrap_min_z.value.cumz + this.cbox_layout_length
							);
							this.scrap_list.insert(this.scrap_min_z, scrap);
						}
					}
				}
				else if (prev.value.cumz == next.value.cumz)
				{
					////////////////////////////////////////////////////////
					// LEFT AND RIGHT ARE ALL EXIST .value. SAME CUMZ
					/////////////////////////////////////////////////////////
					//*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES ***
					//*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***
					lenx = this.scrap_min_z.value.cumx - prev.value.cumx;
					lenz = prev.value.cumz - this.scrap_min_z.value.cumz;
					lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;

					// CALL FIND_BOX AND CHECK_FOUND
					this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
					this.check_found();

					// BREAK ?
					if (this.layer_done) break;
					if (this.evened) continue;

					// RE-FETCH LEFT AND RIGHT
					prev = this.scrap_min_z.prev();
					next = this.scrap_min_z.next();

					// UPDATE CURRENT BOX
					let box: Box = this.box_array[this.cboxi];
					box.coy = this.packed_layout_height;
					box.coz = this.scrap_min_z.value.cumz;

					if (this.cbox_layout_width == this.scrap_min_z.value.cumx - prev.value.cumx)
					{
						box.cox = prev.value.cumx;

						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz)
						{
							// LEFT FETCHES RIGHT'S CUM_X
							prev.value.cumx = next.value.cumx;

							// ERASE MIN_Z AND RIGHT
							this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, next.next());
						}
						else
						{
							// CUMULATE
							this.scrap_min_z.value.cumz += this.cbox_layout_length;
						}
					}
					else if (prev.value.cumx < this.pallet.layout_width - this.scrap_min_z.value.cumx)
					{
						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz)
						{
							// DE-CUMULATE
							this.scrap_min_z.value.cumx -= this.cbox_layout_width;
							box.cox = this.scrap_min_z.value.cumx;
						}
						else
						{
							box.cox = prev.value.cumx;

							// CREATE A NODE BETWEEN LEFT AND MIN_Z
							let scrap: Scrap = new Scrap
							(
								prev.value.cumx + this.cbox_layout_width,
								this.scrap_min_z.value.cumz + this.cbox_layout_length
							);
							this.scrap_list.insert(this.scrap_min_z, scrap);
						}
					}
					else
					{
						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz)
						{
							// CUMULATE
							prev.value.cumx += this.cbox_layout_width;
							box.cox = prev.value.cumx;
						}
						else
						{
							box.cox = this.scrap_min_z.value.cumx - this.cbox_layout_width;

							// CREATE A NODE BETWEEN MIN_Z AND RIGHT
							let scrap: Scrap = new Scrap
							(
								this.scrap_min_z.value.cumx,
								this.scrap_min_z.value.cumz + this.cbox_layout_length
							);
							this.scrap_list.insert(next, scrap);

							// UPDATE MIN_Z
							this.scrap_min_z.value.cumx -= this.cbox_layout_width;
						}
					}
				}
				else
				{
					////////////////////////////////////////////////////////
					// LEFT AND RIGHT ARE ALL EXIST
					////////////////////////////////////////////////////////
					//*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***
					lenx = this.scrap_min_z.value.cumx - prev.value.cumx;
					lenz = prev.value.cumz - this.scrap_min_z.value.cumz;
					lpz = this.remain_layout_length - this.scrap_min_z.value.cumz;

					// CALL FIND_BOX AND CHECK_FOUND
					this.find_box(lenx, this.layer_thickness, this.remain_layout_height, lenz, lpz);
					this.check_found();

					// BREAK ?
					if (this.layer_done) break;
					if (this.evened) continue;

					// RE-FETCH LEFT AND RIGHT
					prev = this.scrap_min_z.prev();
					next = this.scrap_min_z.next();

					// UPDATE CURRENT BOX
					let box: Box = this.box_array[this.cboxi];
					box.coy = this.packed_layout_height;
					box.coz = this.scrap_min_z.value.cumz;
					box.cox = prev.value.cumx;

					if (this.cbox_layout_width == this.scrap_min_z.value.cumx - prev.value.cumx)
					{
						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz)
						{
							// LEFT FETCHES MIN_Z'S
							prev.value.cumx = this.scrap_min_z.value.cumx;

							// ERASE MIN_Z
							this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
						}
						else
						{
							// CUMULATE
							this.scrap_min_z.value.cumz += this.cbox_layout_length;
						}
					}
					else
					{
						if (this.scrap_min_z.value.cumz + this.cbox_layout_length == prev.value.cumz)
						{
							// CUMULATE
							prev.value.cumx += this.cbox_layout_width;
						}
						else if (this.scrap_min_z.value.cumz + this.cbox_layout_length == next.value.cumz)
						{
							// DE-CUMULATE
							this.scrap_min_z.value.cumx -= this.cbox_layout_width;
							box.cox = this.scrap_min_z.value.cumx;
						}
						else
						{
							// CREATE NODE BETWEEN LEFT AND MIN_Z
							let scrap: Scrap = new Scrap
							(
								prev.value.cumx + this.cbox_layout_width,
								this.scrap_min_z.value.cumz + this.cbox_layout_length
							);
							this.scrap_list.insert(this.scrap_min_z, scrap);
						}
					}
				}
	
				this.volume_check();
			}
		}

		/**
		 * Find the most proper layer height by looking at the unpacked boxes and 
		 * the remaining empty space available.
		 */
		private find_layer(thickness: number): void
		{
			// MINIMUM SUM OF LAYERS (height)
			let min_eval: number = Number.MAX_VALUE;
	
			this.layer_thickness = 0;

			for (let i: number = 0; i < this.box_array.size(); i++)
			{
				let box: Box = this.box_array[i];
				if (box.is_packed) 
					continue;

				for (let j: number = 1; j  <= 3; j++)
				{
					let ex_dim: number; // STANDARD LENGTH ON THE DIMENSION
					let dim2: number; // THE SECOND, LENGTH ON A RESIDUAL DIMENSION
					let dim3: number; // THE THIRD, LENGTH ON A RESIDUAL DIMENSION

					let my_eval: number = 0;

					// FETCH STANDARD DIMENSIONS FROM EACH AXIS
					switch (j)
					{
					case 1:
						ex_dim = box.width;
						dim2 = box.height;
						dim3 = box.length;
						break;
					case 2:
						ex_dim = box.height;
						dim2 = box.width;
						dim3 = box.length;
						break;
					case 3:
						ex_dim = box.length;
						dim2 = box.width;
						dim3 = box.height;
						break;
					}

					// ABOUT ALL BOXES, FIND THE MINIMUM LENGTH OF GAP ~,
					// STACK ON THE CURRENT LAYER (ADD ON TO THE LAYER_EVAL)
					if (
							ex_dim <= thickness && 
							(
								(dim2 <= this.pallet.layout_width && dim3 <= this.pallet.layout_length) || 
								(dim3 <= this.pallet.layout_width && dim2 <= this.pallet.layout_length)
							)
						)
					{
						for (let k: number = 0; k < this.box_array.size(); k++)
						{
							let my_box: Box = this.box_array[k];

							// SAME INSTANCE WITH THE SAME INDEX OR ALREADY PACKED
							if (i == k || my_box.is_packed == true)
								continue;

							let dim_diff: number = Math.min
								(
									Math.abs(ex_dim - my_box.width),
									Math.abs(ex_dim - my_box.height),
									Math.abs(ex_dim - my_box.length)
								);
							my_eval += dim_diff;
						}
						if (my_eval < min_eval)
						{
							min_eval = my_eval;
							this.layer_thickness = ex_dim;
						}
					}
				}
			}
			if (this.layer_thickness == 0 || this.layer_thickness > this.remain_layout_height) 
				this.packing = false;
		}

		/**
		 * <p> Determine the gap with the samllest z value in the current layer. </p>
		 *
		 * <p> Find the most proper boxes by looking at all six possible orientations,
		 * empty space given, adjacent boxes, and pallet limits. </p>
		 *
		 * @param hmx Maximum available x-dimension of the current gap to be filled.
		 * @param hy Current layer thickness value.
		 * @param hmy Current layer thickness value.
		 * @param hz Z-dimension of the current gap to be filled.
		 * @param hmz Maximum available z-dimension to the current gap to be filled.
		 */
		private find_box(hmx: number, hy: number, hmy: number, hz: number, hmz: number): void
		{
			this.boxi = -1;
			this.bboxi = -1;

			this.bfx = Number.MAX_VALUE; 
			this.bfy = Number.MAX_VALUE; 
			this.bfz = Number.MAX_VALUE;
			this.bbfx = Number.MAX_VALUE; 
			this.bbfy = Number.MAX_VALUE; 
			this.bbfz = Number.MAX_VALUE;

			for (let i: number = 0; i < this.box_array.size(); i++)
			{
				let box: Box = this.box_array[i];
				if (box.is_packed) 
					continue;
		
				this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.width, box.height, box.length);

				// WHEN REGULAR CUBE
				if (box.width == box.length && box.length == box.height)
					continue;

				this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.width, box.length, box.height);
				this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.height, box.width, box.length);
				this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.height, box.length, box.width);
				this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.length, box.width, box.height);
				this.analyze_box(i, hmx, hy, hmy, hz, hmz, box.length, box.height, box.width);
			}
		}

		/**
		 * <p> Analyzes each unpacked {@link Box box} to find the best fitting one to the empty space. </p>
		 *
		 * <p> Used by {@link find_box find_box()} to analyze box dimensions. </p> 
		 *
		 * @param x index of a {@link Box box} in the {@link box_array}.
		 *
		 * @param hmx Maximum available x-dimension of the current gap to be filled.
		 * @param hy Current layer thickness value.
		 * @param hmy Current layer thickness value.
		 * @param hz Z-dimension of the current gap to be filled.
		 * @param hmz Maximum available z-dimension to the current gap to be filled.
		 *
		 * @param dim1 X-dimension of the orientation of the box being examined.
		 * @param dim2 Y-dimension of the orientation of the box being examined.
		 * @param dim3 Z-dimension of the orientation of the box being examined.
		 */
		private analyze_box(index: number, hmx: number, hy: number, hmy: number, hz: number, hmz: number, dim1: number, dim2: number, dim3: number): void
		{
			// OUT OF BOUNDARY RANGE
			if (dim1 > hmx || dim2 > hmy || dim3 > hmz)
				return;

			if (dim2 <= hy && 
					(
						hy - dim2 < this.bfy ||
						(hy - dim2 == this.bfy && hmx - dim1 < this.bfx) ||
						(hy - dim2 == this.bfy && hmx - dim1 == this.bfx && Math.abs(hz - dim3) < this.bfz)
					)
				)
			{
				this.boxx = dim1;
				this.boxy = dim2;
				this.boxz = dim3;
				this.bfx = hmx - dim1;
				this.bfy = hy - dim2;
				this.bfz = Math.abs(hz - dim3);
				this.boxi = index;
			}
			else if 
				(dim2 > hy &&
					(
						dim2 - hy < this.bbfy ||
						(dim2 - hy == this.bbfy && hmx - dim1 < this.bbfx) ||
						(dim2 - hy == this.bbfy && hmx - dim1 == this.bbfx && Math.abs(hz - dim3) < this.bbfz)
					)
				)
			{
				this.bboxx = dim1;
				this.bboxy = dim2;
				this.bboxz = dim3;
				this.bbfx = hmx - dim1;
				this.bbfy = dim2 - hy;
				this.bbfz = Math.abs(hz - dim3);
				this.bboxi = index;
			}
		}

		/**
		 * After finding each box, the candidate boxes and the condition of the layer are examined.
		 */
		private check_found(): void
		{
			this.evened = false;

			if (this.boxi != -1)
			{
				this.cboxi = this.boxi;
				this.cbox_layout_width = this.boxx;
				this.cbox_layout_height = this.boxy;
				this.cbox_layout_length = this.boxz;
			}
			else
			{
				let prev = this.scrap_min_z.prev();
				let next = this.scrap_min_z.next();

				if (this.bboxi != -1 && // IN RANGE
					(
						this.layer_in_layer != 0 || 
						(
							// NO LEFT AND RIGHT EXISTS
							prev.equal_to(this.scrap_list.end()) && next.equal_to(this.scrap_list.end())
						)
					))
				{
					////////////////////////////////////////////
					// ~ OR SCRAP_MIN_Z HAS NO NEIGHBOR
					////////////////////////////////////////////
					if (this.layer_in_layer == 0)
					{
						this.pre_layer = this.layer_thickness;
						this.lilz = this.scrap_min_z.value.cumz;
					}

					this.cboxi = this.bboxi;
					this.cbox_layout_width = this.bboxx;
					this.cbox_layout_height = this.bboxy;
					this.cbox_layout_length = this.bboxz;

					this.layer_in_layer += this.bboxy - this.layer_thickness;
					this.layer_thickness = this.bboxy;
				}
				else
				{
					if (prev.equal_to(this.scrap_list.end()) && next.equal_to(this.scrap_list.end()))
					{
						///////////////////////////////////////////
						// SCRAP_MIN_Z HAS NO NEIGHBOR
						///////////////////////////////////////////
						// IN RANGE & NO NEIGHBOR
						// LAYER HAS DONE.
						this.layer_done = true;
					}
					else
					{
						this.evened = true;

						if (prev.equal_to(this.scrap_list.end()))
						{
							///////////////////////////////////////////
							// NO LEFT, BUT RIGHT
							///////////////////////////////////////////
							// ERASE SCRAP_MIN_Z
							// RIGHT IS THE NEW SCRAP_MIN_Z
							this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
						}
						else if (next.equal_to(this.scrap_list.end()))
						{
							///////////////////////////////////////////
							// NO RIGHT, BUT LEFT
							///////////////////////////////////////////
							// ERASE CURRENT SCRAP_MIN_Z
							// THE LEFT ITEM FETCHES MIN'S CUM_X
							prev.value.cumx = this.scrap_min_z.value.cumx;

							// ERASE FROM MIN_Z TO END
							this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, this.scrap_list.end());
						}
						else
						{
							///////////////////////////////////////////
							// LEFT & RIGHT ARE ALL EXIST
							///////////////////////////////////////////
							if (prev.value.cumz == next.value.cumz)
							{
								// ----------------------------------------
								// LEFT AND RIGHT'S CUM_Z ARE EQUAL
								// ----------------------------------------
								// LEFT FETCHES THE RIGHT'S CUM_X
								prev.value.cumx = next.value.cumx;

								// ERASE MIN AND ITS RIGHT
								this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z, next.next());
							}
							else
							{
								// ----------------------------------------
								// LEFT AND RIGHT'S CUM_Z ARE NOT EQUAL
								// ----------------------------------------
								if (prev.value.cumz == next.value.cumz)
									prev.value.cumx = this.scrap_min_z.value.cumx;

								// ERASE SCRAP_MIN_Z
								this.scrap_min_z = this.scrap_list.erase(this.scrap_min_z);
							}
						}
					}
				}
			}
		}
		
		/**
		 * After packing of each box, 100% packing condition is checked.
		 */
		private volume_check(): void
		{
			let box: Box = this.box_array[this.cboxi];

			box.is_packed = true;
			box.layout_width = this.cbox_layout_width;
			box.layout_height = this.cbox_layout_height;
			box.layout_length = this.cbox_layout_length;
	
			this.packed_volume += box.volume;
	
			if (this.packing_best)
			{
				// BOXOLOGIC DOESN'T MEMORIZE OPTIMIZED ORIENTATION
				// THUS IT NEEDS ADDITIONAL PROCEDURES FOR EXPORTING
				this.write_box_file();
			}
			else if (this.packed_volume == this.pallet.volume || this.packed_volume == this.total_box_volume)
			{
				this.packing = false;
				this.hundred_percent = true;
			}
		}

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * <p> Find the first to be packed gap in the layer edge. </p>
		 * 
		 * <p> Determine the gap with the {@link scrap_min_z smallest z} value in the current layer. </p>
		 */
		private find_smallest_z(): void
		{
			this.scrap_min_z = this.scrap_list.begin();

			for (let it = this.scrap_min_z; !it.equal_to(this.scrap_list.end()); it = it.next())
				if (it.value.cumz < this.scrap_min_z.value.cumz)
					this.scrap_min_z = it;
		}

		/* -----------------------------------------------------------
			REPORTERS
		----------------------------------------------------------- */
		/**
		 * <p> Determine {@link box_arrray boxes}. </p>
		 *
		 * <p> Using the parameters found, packs the best solution found and reports. </p>
		 */
		private report_results(): void
		{
			////////////////////////////////////////////////////
			// BEGINS RE-PACKING FOLLOWING THE BEST VARS
			////////////////////////////////////////////////////
			this.packing_best = true;

			this.pallet.set_orientation(this.best_orientation);
			this.construct_layers();
			this.iterate_layer(this.best_layer);
		}

		/**
		 * <p> Determine a {@link Box}. </p>
		 *
		 * <p> Transforms the found co-ordinate system to the one entered by the user and write them to the 
		 * report. </p>
		 */
		private write_box_file(): void
		{
			let box: Box = this.box_array[this.cboxi];

			let cox: number;
			let coy: number;
			let coz: number;

			let layout_width: number;
			let layout_height: number;
			let layout_length: number;
	
			switch (this.best_orientation)
			{
			case 1:
				cox = box.cox;
				coy = box.coy;
				coz = box.coz;
				layout_width = box.layout_width;
				layout_height = box.layout_height;
				layout_length = box.layout_length;
				break;
			case 2:
				cox = box.coz;
				coy = box.coy;
				coz = box.cox;
				layout_width = box.layout_length;
				layout_height = box.layout_height;
				layout_length = box.layout_width;
				break;
			case 3:
				cox = box.coy;
				coy = box.coz;
				coz = box.cox;
				layout_width = box.layout_height;
				layout_height = box.layout_length;
				layout_length = box.layout_width;
				break;
			case 4:
				cox = box.coy;
				coy = box.cox;
				coz = box.coz;
				layout_width = box.layout_height;
				layout_height = box.layout_width;
				layout_length = box.layout_length;
				break;
			case 5:
				cox = box.cox;
				coy = box.coz;
				coz = box.coy;
				layout_width = box.layout_width;
				layout_height = box.layout_length;
				layout_length = box.layout_height;
				break;
			case 6:
				cox = box.coz;
				coy = box.cox;
				coz = box.coy;
				layout_width = box.layout_length;
				layout_height = box.layout_width;
				layout_length = box.layout_height;
				break;
			}

			box.cox = cox;
			box.coy = coy;
			box.coz = coz;
			box.layout_width = layout_width;
			box.layout_height = layout_height;
			box.layout_length = layout_length;
		}
	}
}