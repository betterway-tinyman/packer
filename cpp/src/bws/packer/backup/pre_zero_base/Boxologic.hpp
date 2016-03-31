#pragma once

#include <vector>
#include <list>
#include <map>

#include <memory>

struct s_Instance
{
	double width;
	double height;
	double length;

	double layout_width;
	double layout_height;
	double layout_length;

	double volume;
};

struct Pallet
	: public s_Instance
{};

/**
 * 박스 정보.
 *
 * Packer에서의 Instance + Wrap 에 해당함
 */
struct BoxInfo
	: public s_Instance
{
	/**
	 * 현재 박스의 화물칸 내 좌표.
	 *
	 * Wrap의 getX(), getY(), getZ() 에 해당
	 */
	double cox, coy, coz;

	/**
	 * 포장 되었는 지 여부
	 */
	bool is_packed;
};

struct Scrappad
{
	double cumx;
	double cumz;
};

namespace hiswill
{
namespace packer
{
	class Wrapper;
	class InstanceArray;

	class Boxologic
	{
	private:
		/**
		 *
		 */
		std::shared_ptr<Wrapper> wrapper;

		/**
		 *
		 */
		std::shared_ptr<InstanceArray> instanceArray;

		/**
		 *
		 */
		std::shared_ptr<InstanceArray> leftInstanceArray;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Construct from a wrapper and instances.
		 *
		 * @param wrapper
		 * @param instanceArray
		 */
		Boxologic(std::shared_ptr<Wrapper>, std::shared_ptr<InstanceArray>);
		
		~Boxologic();

	private:
		void encode();

		void decode();

	private:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		auto fetch_scrap_min_z_left() -> std::list<struct Scrappad>::iterator;

		auto fetch_scrap_min_z_right() -> std::list<struct Scrappad>::iterator;

	public:
		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		auto pack() -> std::pair<std::shared_ptr<Wrapper>, std::shared_ptr<InstanceArray>>;

	private:
		/* -----------------------------------------------------------
			3D_BIN_PACKER
		----------------------------------------------------------- */
		Pallet pallet;

		std::vector<struct BoxInfo> box_array;

		/**
		 * 단일 dimension의 레이어.
		 *
		 * 특정 상품과 그 상품의 특정 축을 기준으로, 모든 상품들을 쌓아나가 구성한 레이어이다.
		 *
		 * <ul>
		 *	<li>
		 *		레이어이 크기 (높이).
		 *		모든 박스를 층층이 쌓아올렸을 때 필요한 최소 크기이다.
		 *
		 *		모든 Instance의 width, length, height로부터 각기 가장 작은 값을 추출해 모두 더함.
		 *	</li>
		 *	<li>
		 *		기준 크기, unique key.
		 *		기준 축에서의 특정 박스의 크기로부터 구한다. 단, 중복될 수 없다.
		 *
		 *		Instance의 width, length, height 중 하나. 단, 유일해야 함.
		 *	</li>
		 * </ul>
		 */
		std::map<double, double> layer_map;
		
		std::list<struct Scrappad> scrap_list;
		std::list<struct Scrappad>::iterator scrap_min_z;

		/**
		 * Packing을 계속 진행할 지에 대한 flag 값.
		 * execute_iterations() 의 레이어 내 iteration 및 packing 과정에서 최적화 중단 여부를 기록하기 위한 flag이다.
		 *
		 * Target to be local.
		 */
		bool packing;
		bool layerdone;
		bool evened;

		/**
		 * 배치 방향 정보, orientation.
		 *
		 * Target to be local.
		 */
		int variant;

		/**
		 * 최적 packing case의 배치 방향.
		 */
		int best_variant;

		bool packing_best;
		bool hundred_percent;

		double boxx, boxy, boxz;
		double bboxx, bboxy, bboxz;
		size_t boxi, bboxi;

		/**
		 * 현재 탐색 중인 박스의 위치정보 및 index (current_box_???).
		 */
		double cboxx, cboxy, cboxz;
		size_t cboxi;
		double bfx, bfy, bfz;
		double bbfx, bbfy, bbfz;

		/**
		 * 전체 박스 수량.
		 *
		 * Wrapper::reserveds->size() 에 해당.
		 */
		size_t total_boxes;

		double layerinlayer;
		double prelayer;
		double lilz;

		/**
		 * iteration을 수행한 횟수.
		 *
		 * 하나의 iteration은 하나의 packing case를 의미함.
		 */
		size_t number_of_iterations;

		/**
		 * 화물칸의 남은 구간의 크기.
		 */
		double remainpx, remainpy, remainpz;
		double packedy;
		double prepackedy;
		double layer_thickness;
		double itelayer;
		double preremainpy;

		/**
		 * 최적 packing case가 사용한 layer의 index 번호
		 */
		double best_iteration;

		size_t packed_boxes;

		/**
		 * 최적 packing case에서 담은 박스 개수.
		 */
		size_t number_packed_boxes;

		double packed_volume;

		/**
		 * 최적 packing case의 활용 부피.
		 */
		double best_solution_volume;
		
		/**
		 * 전체 박스의 부피 합.
		 *
		 * sum (Instance::getVolume()) 에 해당.
		 */
		double total_box_volume;
		double temp;

		/**
		 * 최적의 packing case의 공간 활용도.
		 */
		double pallet_volume_used_percentage;
		double packed_box_percentage;

		/**
		 * Execute iterations by calling proper functions.
		 *
		 * Iterations are done and parameters of the best solution are found.
		 */
		void execute_iterations(); //TODO: Needs a better name yet

		/**
		 * Construct layers.
		 *
		 * Lists all possible layer heights by giving a weight value to each of them.
		 */
		void list_candidate_layers();

		/**
		 * Update the linked list and the Boxlist[] array as a box is packed.
		 *
		 * Packs the boxes found and arranges all variables and records properly.
		 */
		int pack_layer();

		/**
		 * Find the most proper layer height by looking at the unpacked boxes and 
		 * the remaining empty space available.
		 */
		int find_layer(double thickness);

		/**
		 * Determine the gap with the samllest z value in the current layer.
		 *
		 * Find the most proper boxes by looking at all six possible orientations,
		 * empty space given, adjacent boxes, and pallet limits.
		 *
		 * @param hmx Maximum available x-dimension of the current gap to be filled.
		 * @param hy Current layer thickness value.
		 * @param hmy Current layer thickness value.
		 * @param hz Z-dimension of the current gap to be filled.
		 * @param hmz Maximum available z-dimension to the current gap to be filled.
		 */
		void find_box(double hmx, double hy, double hmy, double hz, double hmz);

		/**
		 * Used by find_box to analyze box dimensions.
		 *
		 * Analyzes each unpacked box to find the best fitting one to the empty space.
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
		void analyze_box(size_t x, double hmx, double hy, double hmy, double hz, double hmz, double dim1, double dim2, double dim3);

		/**
		 * Determine the gap with the smallest z value in the current layer.
		 *
		 * Find the first to be packed gap i9n the layer edge.
		 */
		void find_smallest_z();

		/**
		 * After finding each box, the candidate boxes and the condition of the layer
		 * are examined.
		 */
		void checkfound(); //TODO: Find better name for this
		
		/**
		 * After packing of each box, 100% packing condition is checked.
		 */
		void volume_check();

		/**
		 * Using the parameters found, packs the best solution found and reports.
		 */
		void report_results();

		/**
		 * Transforms the found coordinate system to the one entered by the user and 
		 * write them to the report.
		 */
		void write_boxlist_file();
	};
};
};