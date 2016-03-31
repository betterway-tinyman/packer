#pragma once

#include <boxologic/Pallet.hpp>

#include <vector>
#include <list>
#include <map>
#include <memory>

namespace bws
{
namespace packer
{
	class Wrapper;
	class InstanceArray;
	class Instance;
};
};

/**
 * @brief A set of programs that calculate the best fit for boxes on a pallet migrated from language C. </p>
 *
 * <ul>
 *	<li> Forked from: https://github.com/exad/boxologic </li>
 * </ul>
 * 
 * @author Bill Knechtel, <br>
 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
 */
namespace boxologic
{
	struct st_Instance;
	struct Pallet;
	struct Box;
	struct Scrappad;

	/**
	 * @brief Cumulated lengths of a layer.	
	 * 
	 * @details Scrapped represents an edge of a layer under construction.
	 * 
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 */
	struct Scrappad
	{
	public:
		/**
		 * @brief Cumulated length on the X-axis in 3D.
		 */
		double cumx;

		/**
		 * @brief Cumulated length on the Z-axis in 3D.
		 */
		double cumz;
	};

	/**
	 * @brief A facade class of boxologic.
	 * @details The boloxogic class deducts the best solution of packing boxes to a pallet.
	 *
	 * <ul>
	 *	<li> Reference: https://github.com/exad/boxologic </li>
	 * </ul>
	 *
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 */
	class Boxologic
	{
	private:
		/* ===========================================================
			PARAMETRIC DATA
		=========================================================== */
		/**
		 * A Wrapper to pack instances.
		 */
		std::shared_ptr<bws::packer::Wrapper> wrapper;

		/**
		 * Instances trying to pack into the wrapper.
		 */
		std::shared_ptr<bws::packer::InstanceArray> instanceArray;

		/**
		 * Instances failed to pack by overloading.
		 */
		std::shared_ptr<bws::packer::InstanceArray> leftInstances;

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
		 * @brief A pallet containing {@link Box boxes}.
		 *
		 * @see Wrapper
		 */
		Pallet pallet;

		/** 
		 * @brief Boxes, trying to pack into the #pallet.
		 *
		 * @see Instance
		 */
		std::vector<struct Box> box_array;

		/**
		 * Sum of all boxes' volume.
		 */
		double total_box_volume;

		/**
		 * @brief All different lengths of {@link box_array all box} dimensions along with evaluation values.
		 *
		 * @details 
		 * <p> In other word, the <i>layer_map</i> stores those entries; each {@link Boxbox}'s length on each 
		 * axis as a <i>key</i> (width, height or length) and evaluation value as a <i>value</i>. The evaluation 
		 * value means sum of minimum gaps between the key and other {@link Box boxes}' width, height and length 
		 * </p>
		 *
		 * <code>
		 FOR i to box_array.size()
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
		std::map<double, double> layer_map;
		
		/**
		 * @brief List of Scrapped instances, edges of layers under construction.
		 *
		 * @see Scrapped
		 * @see #scrap_min_z
		 */
		std::list<struct Scrappad> scrap_list;

		/**
		 * @brief The topology Scrapped, the edge of the current layer under construction.
		 *
		 * @see Scrapped
		 * @see #scrap_list
		 */
		std::list<struct Scrappad>::iterator scrap_min_z;

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
		 * @brief Index of the current {@link Box box}.
		 */
		size_t cboxi;

		/**
		 * @brief Candidate {@link Box.layout_width layout_width} of the {@link cboxi current box}.
		 */
		double cbox_layout_width;

		/**
		 * @brief Candidate {@link Box.layout_height layout_height} of the {@link cboxi current box}.
		 */
		double cbox_layout_height;

		/**
		 * @brief Candidate {@link Box.layout_length layout_length} of the {@link cboxi current box}.
		 */
		double cbox_layout_length;
		
		//////////////////////////////////////////////////////////////
		//	LAYER
		//////////////////////////////////////////////////////////////
		/**
		 * @brief Current layer's key on iteration.
		 */
		double layer_thickness;

		/**
		 * @brief Previous layer's key had iterated.
		 */
		double pre_layer;
		
		/**
		 * @brief Key of the unevened layer in the current packing layer.
		 */
		double layer_in_layer;

		/**
		 * @brief Little Z, gotten from {@link Scrapped.cumz cumz} in #min_scrap_z
		 */
		double lilz;

		//////////////////////////////////////////////////////////////
		//	REMAINS & PACKED
		//////////////////////////////////////////////////////////////
		/**
		 * @brief Remained (unfilled) {@link Pallet.layout_height layout_height} of the #pallet.
		 */
		double remain_layout_height;

		/**
		 * @brief Remained (unfilled) {@link Pallet.layout_length layout_length} of the #pallet.
		 */
		double remain_layout_length;

		/**
		 * @brief Packed (filled) {@link Pallet.layout_height layout_height} of the @pallet.
		 */
		double packed_layout_height;

		/**
		 * @brief Packed {@link Pallet.volume volume} of the #pallet
		 */
		double packed_volume;

		//////////////////////////////////////////////////////////////
		//	FINDER & ANALYZER
		//////////////////////////////////////////////////////////////
		size_t boxi;
		size_t bboxi;

		double boxx;
		double boxy;
		double boxz;

		double bboxx;
		double bboxy;
		double bboxz;

		double bfx, bfy, bfz;
		double bbfx, bbfy, bbfz;

		/* -----------------------------------------------------------
			FLAGS FOR TERMINATING ITERATION
		----------------------------------------------------------- */
		/**
		* @brief Whether the packing is on progress.
		*
		* @details The #packing is flag variable for terminating iterations in #iterate_orientations().
		*/
		bool packing;

		/**
		 * Whether packing a layer is done.
		 */
		bool layer_done;

		/**
		 * Whether the current packing layer is evened.
		 */
		bool evened;

		/**
		 * Whether the best solution is deducted.
		 */
		bool packing_best;

		/**
		 * Whether the utilization degree of pallet space is 100%.
		 */
		bool hundred_percent;

		/**
		 * The best orientation of the pallet, which can deduct the {@link best_solution_volume}.
		 */
		int best_orientation;

		/**
		 * The best layer, which can deduct the {@link best_solution_volume}.
		 */
		double best_layer;

		/**
		 * The best volume, fit the best utilization degree of the pallet space.
		 */
		double best_solution_volume;

	public:
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
		 * @param wrapper
		 * @param instanceArray
		 */
		Boxologic(std::shared_ptr<bws::packer::Wrapper>, std::shared_ptr<bws::packer::InstanceArray>);
		
		~Boxologic();

	private:
		/* -----------------------------------------------------------
			ENCODER & DECODER
		----------------------------------------------------------- */
		/**
		 * @brief Encode data.
		 *
		 * @details Encodes {@link bws::packer Packer}'s data to be suitable for the 
		 *			{@link boxologic Boxologic}'s parametric data.
		 */
		void encode();

		/**
		 * @brief Decode data.
		 *
		 * @details Decodes the {@link boxologic Boxologic's optimization result data} to be suitable for the 
		 *			{@link bws::packer Packer}'s own.
		 */
		void decode();

	public:
		/* ===========================================================
			MAIN PROCEDURES
				- OPERATORS
				- GETTERS
				- CHECKERS
				- REPORTERS
		==============================================================
			OPERATORS
		------------------------------------------------------------ */
		/**
		 * <p> Pack instances to the #wrapper. </p>
		 *
		 * <p> The #pack is an adaptor method between {@link bws::packer Packer} and {@link boxologic}. 
		 * It encodes data from {@link bws::packer Packer}, deducts the best packing solution decodes the 
		 * optimization result and returns it. </p>
		 *
		 * <p> The optimization result is returned as a Pair like below: </p>
		 * <ul>
		 *	<li> first: The #wrapper with packed instances. </li>
		 *	<li> second: {@link leftInstances Left instances failed to pack} by overloading. </li>
		 * </ul>
		 *
		 * @return A pair of #wrapper with packed instances and 
		 *		   {@link leftInstances instances failed to pack} by overloading.
		 */
		auto pack() -> std::pair<std::shared_ptr<bws::packer::Wrapper>, std::shared_ptr<bws::packer::InstanceArray>>;

	private:
		/**
		 * @brief Execute iterations by calling proper functions.
		 *
		 * @details Iterations are done and parameters of the best solution are found.
		 */
		void iterate_orientations(); //TODO: Needs a better name yet

		/**
		 * @brief Iterate a layer.
		 *
		 * @param layer_thickness Thickness of the iterating layer.
		 */
		void iterate_layer(double layer_thickness);

		/**
		 * @brief Construct layers.
		 *
		 * @details Creates all possible layer heights by giving a weight value to each of them.
		 */
		void construct_layers();

		/**
		 * @brief Packs the boxes found and arranges all variables and records properly.
		 * 
		 * @details Update the linked list and the Boxlist[] array as a box is packed.
		 */
		void pack_layer();

		/**
		 * @brief Find the most proper layer height.
		 *
		 * @details Find the most proper layer height by looking at the unpacked boxes and 
		 *			the remaining empty space available.
		 */
		void find_layer(double thickness);

		/**
		 * @brief Determine the gap with the samllest z value in the current layer.
		 *
		 * @details Find the most proper boxes by looking at all six possible orientations,
		 *			empty space given, adjacent boxes, and pallet limits.
		 *
		 * @param hmx Maximum available x-dimension of the current gap to be filled.
		 * @param hy Current layer thickness value.
		 * @param hmy Current layer thickness value.
		 * @param hz Z-dimension of the current gap to be filled.
		 * @param hmz Maximum available z-dimension to the current gap to be filled.
		 */
		void find_box(double hmx, double hy, double hmy, double hz, double hmz);

		/**
		 * @brief Analyzes each unpacked Box to find the best fitting one to the empty space.
		 * 
		 * @details Used by #find_box() to analyze box dimensions.
		 *
		 * @param index Index of a Box in the #box_array.
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
		void analyze_box(size_t index, double hmx, double hy, double hmy, double hz, double hmz, double dim1, double dim2, double dim3);

		/* -----------------------------------------------------------
			CHECKERS
		----------------------------------------------------------- */
		/**
		 * After finding each box, the candidate boxes and the condition of the layer
		 * are examined.
		 */
		void check_found(); //TODO: Find better name for this
		
		/**
		 * After packing of each box, 100% packing condition is checked.
		 */
		void volume_check();

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		auto fetch_scrap_min_z_left() -> std::list<struct Scrappad>::iterator;

		auto fetch_scrap_min_z_right() -> std::list<struct Scrappad>::iterator;

		/**
		 * @brief Find the first to be packed gap in the layer edge.
		 *
		 * @details Determine the gap with the {@link #scrap_min_z smallest z} value in the current layer.
		 */
		void find_smallest_z();

		/* -----------------------------------------------------------
			REPORTERS
		----------------------------------------------------------- */
		/**
		 * @brief Determine {@link #box_arrray boxes}.
		 *
		 * Using the parameters found, packs the best solution found and reports.
		 */
		void report_results();

		/**
		 * @brief Determine a Box.
		 *
		 * @details Transforms the found co-ordinate system to the one entered by the user and 
		 *			write them to the report.
		 */
		void write_box_file();
	};
};