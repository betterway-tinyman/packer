#pragma once

#include <vector>
#include <list>
#include <map>

#include <memory>

struct BoxInfo;
struct Layer;
struct Scrappad;

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
		
		//virtual ~Boxologic() = default;

	private:
		void encode();

		void decode();

	public:
		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		auto pack() -> std::pair<std::shared_ptr<Wrapper>, std::shared_ptr<InstanceArray>>;

	private:
		/* -----------------------------------------------------------
			3D_BIN_PACKER
		----------------------------------------------------------- */
		std::vector<struct BoxInfo> box_list;
		std::map<long int, long int> layer_list;

		std::list<struct Scrappad> scrap_list;
		std::list<struct Scrappad>::iterator scrap_min_z;

		/**
		 * Packing을 계속 진행할 지에 대한 flag 값.
		 * execute_iterations() 의 레이어 내 iteration 및 packing 과정에서 최적화 중단 여부를 기록하기 위한 flag이다.
		 *
		 * Target to be local.
		 */
		bool packing;
		bool layer_done;
		bool evened;

		/**
		 * 배치 방향 정보, orientation.
		 *
		 * Target to be local.
		 */
		char variant;

		/**
		 * 최적 packing case의 배치 방향.
		 */
		char best_variant;

		bool best_packing;
		bool hundred_percent;

		short int boxx, boxy, boxz, boxi;
		short int bboxx, bboxy, bboxz, bboxi;

		/**
		 * 현재 탐색 중인 박스의 위치정보 및 index (current_box_???).
		 */
		short int cboxx, cboxy, cboxz, cboxi;
		short int bfx, bfy, bfz;
		short int bbfx, bbfy, bbfz;

		/**
		 * 화물칸의 크기 정보.
		 *
		 * Wrapper의 width, length, height에 해당함
		 */
		short int xx, yy, zz;

		/**
		 * 화물칸의 방향별 크기 정보.
		 *
		 * 화물칸도 orientation을 가질 수 있는 것 같은데, 이에 대하여 더 알아봐야 한다.
		 */
		short int pallet_x, pallet_y, pallet_z;

		/**
		 * 전체 박스 수량.
		 *
		 * Wrapper::reserveds->size() 에 해당.
		 */
		short int total_boxes;

		/**
		 * index i 처럼 쓰인다.
		 *
		 * Target to be local.
		 */
		short int x;
		
		short int layerinlayer;
		short int prelayer;
		short int lilz;

		/**
		 * 화물칸의 남은 구간의 크기.
		 */
		short int remainpx, remainpy, remainpz;
		short int packedy;
		short int prepackedy;
		short int layerthickness;
		short int itelayer;
		short int preremainpy;

		/**
		 * 최적 packing case가 사용한 layer의 key (dim)
		 */
		long int best_layer;
		short int packednumbox;

		/**
		 * 최적 packing case에서 담은 박스 개수.
		 */
		short int number_packed_boxes;

		double packedvolume;

		/**
		 * 최적 packing case의 활용 부피.
		 */
		double best_solution_volume;

		/**
		 * 화물칸의 부피. xx x yy x zz).
		 *
		 * Wrapper의 getVolume() 에 해당.
		 */
		double total_pallet_volume;

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
		int find_layer(short int thickness);

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
		void find_box(short int hmx, short int hy, short int hmy, short int hz, short int hmz);

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
		void analyze_box(short int hmx, short int hy, short int hmy, short int hz, short int hmz, short int dim1, short int dim2, short int dim3);

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
		void check_found(); //TODO: Find better name for this
		
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

/* -----------------------------------------------------------
STRUCTURES
----------------------------------------------------------- */
/**
* 박스 정보.
*
* Packer에서의 Instance + Wrap 에 해당함
*/
struct BoxInfo
{
	/**
	* 포장 되었는 지 여부
	*/
	bool is_packed;

	/**
	* 박스 고유의 사이즈 정보.
	*
	* Instance의 width, length, height에 해당한다.
	*/
	short int width, height, length;

	/**
	* 현재 박스의 화물칸 내 좌표.
	*
	* Wrap의 getX(), getY(), getZ() 에 해당
	*/
	short int cox, coy, coz;

	/**
	* 현재 박스의 화물칸 내 사이즈 정보.
	* 화물칸 내 배치 방향이 반영된 사이즈 정보이다.
	*
	* Wrap의 getWidth(), getLength(), getHeight() 에 해당
	*/
	short int layout_width, layout_height, layout_length;

	/**
	* 부피. dim1 x dim2 x dim3
	*/
	long int vol;
};

///**
// * 단일 dimension의 레이어.
// *
// * 특정 상품과 그 상품의 특정 축을 기준으로, 모든 상품들을 쌓아나가 구성한 레이어이다.
// */
//struct Layer
//{
//	/**
//	* 레이어이 크기 (높이).
//	* 모든 박스를 층층이 쌓아올렸을 때 필요한 최소 크기이다.
//	*
//	* 모든 Instance의 width, length, height로부터 각기 가장 작은 값을 추출해 모두 더함.
//	*/
//	long int layereval;
//
//	/**
//	 * 기준 크기, unique key.
//	 * 기준 축에서의 특정 박스의 크기로부터 구한다. 단, 중복될 수 없다.
//	 *
//	 * Instance의 width, length, height 중 하나. 단, 유일해야 함.
//	 */
//	short int layerdim;
//} *layer_list;

struct Scrappad
{
	short int cumx;
	short int cumz;
};