#include <hiswill/packer/Boxologic.hpp>

#include <hiswill/packer/Wrapper.hpp>
#include <hiswill/packer/InstanceArray.hpp>

#include <algorithm>

using namespace hiswill::packer;

using namespace std;
using namespace samchon::library;
using namespace samchon::protocol;

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
//} *layer_map;

struct Scrappad
{
	struct Scrappad *prev;
	struct Scrappad *next;
	short int cumx, cumz;
};

/* -----------------------------------------------------------
	CONSTRUCTORS
----------------------------------------------------------- */
Boxologic::Boxologic(shared_ptr<Wrapper> wrapper, shared_ptr<InstanceArray> instanceArray)
{
	this->wrapper = wrapper;
	this->instanceArray = instanceArray;
	this->leftInstanceArray = make_shared<InstanceArray>();

	encode();
}
Boxologic::~Boxologic()
{
	delete[] box_array;
	//delete[] layer_map;

	struct Scrappad *scrap = scrap_first;
	while (scrap->next != NULL)
	{
		scrap = scrap->next;
		delete scrap->prev;
	}
	if (scrap != NULL)
		delete scrap;
}

void Boxologic::encode()
{
	// READ_BOXLIST_INPUT
	xx = wrapper->getContainableWidth();
	yy = wrapper->getContainableHeight();
	zz = wrapper->getContainableLength();

	box_array = new struct BoxInfo[instanceArray->size() + 1];
	layer_map.clear();
	total_boxes = instanceArray->size();

	for (size_t i = 0; i < instanceArray->size(); i++)
	{
		shared_ptr<Instance> instance = instanceArray->at(i);
		size_t k = i + 1;

		box_array[k].width = instance->getWidth();
		box_array[k].height = instance->getHeight();
		box_array[k].length = instance->getLength();

		box_array[k].vol = instance->getVolume();
		box_array[k].is_packed = false;
	}

	// INITIALIZE
	temp = 1.0;
	total_pallet_volume = temp * xx * yy * zz;
	total_box_volume = 0.0;

	for (x = 1; x <= total_boxes; x++) 
		total_box_volume = total_box_volume + box_array[x].vol;
	
	scrap_first = (struct Scrappad*)malloc(sizeof(struct Scrappad));
	if (scrap_first == NULL)
	{
		printf("Insufficient memory available\n");
		exit(1);
	}
	scrap_first->prev = NULL;
	scrap_first->next = NULL;
	
	best_solution_volume = 0.0;
	packingbest = 0;
	hundredpercent = 0;
	number_of_iterations = 0;
}

void Boxologic::decode()
{
	wrapper->clear();
	leftInstanceArray->clear();

	for (size_t k = 1; k <= total_boxes; k++)
	{
		const BoxInfo &box = box_array[k];
		shared_ptr<Instance> instance = instanceArray->at(k - 1);

		if (box.is_packed == true)
		{
			Wrap *wrap = new Wrap(wrapper.get(), instance,box.cox, box.coy, box.coz);
			wrap->estimateOrientation(box.layout_width, box.layout_height, box.layout_length);
			
			wrap->setPosition
			(
				wrap->getX() + wrapper->getThickness(),
				wrap->getY() + wrapper->getThickness(),
				wrap->getZ() + wrapper->getThickness()
			);
			wrapper->emplace_back(wrap);
		}
		else
		{
			// 공간 부족으로 포장에 실패한 잔여 물체의 경우
			leftInstanceArray->push_back(instance);
		}
	}

	// 정렬
	sort(wrapper->begin(), wrapper->end(), 
		[](const shared_ptr<Wrap> &left, const shared_ptr<Wrap> &right) -> bool
		{
			if (left->getZ() != right->getZ())
				return left->getZ() < right->getZ();
			else if (left->getY() != right->getY())
				return left->getY() < right->getY();
			else
				return left->getX() < right->getX();
		});
}

/* -----------------------------------------------------------
	OPERATORS
----------------------------------------------------------- */
auto Boxologic::pack() -> pair<shared_ptr<Wrapper>, shared_ptr<InstanceArray>>
{
	execute_iterations();
	report_results();
	
	decode();

	return {wrapper, leftInstanceArray};
}

void Boxologic::execute_iterations(void)
{
	for (variant = 1; variant <= 6; variant++)
	{
		switch (variant)
		{
		case 1:
			pallet_x = xx; pallet_y = yy; pallet_z = zz;
			break;
		case 2:
			pallet_x = zz; pallet_y = yy; pallet_z = xx;
			break;
		case 3:
			pallet_x = zz; pallet_y = xx; pallet_z = yy;
			break;
		case 4:
			pallet_x = yy; pallet_y = xx; pallet_z = zz;
			break;
		case 5:
			pallet_x = xx; pallet_y = zz; pallet_z = yy;
			break;
		case 6:
			pallet_x = yy; pallet_y = zz; pallet_z = xx;
			break;
		}

		list_candidate_layers();

		for (auto it = layer_map.begin(); it != layer_map.end(); it++)
		{
			++number_of_iterations;
			
			packedvolume = 0.0;
			packedy = 0;
			packing = true;
			
			layerthickness = it->first;
			itelayer = it->first;
			remainpy = pallet_y;
			remainpz = pallet_z;
			packednumbox = 0;

			for (x = 1; x <= total_boxes; x++)
			{
				// 모든 박스를 다시 빼냄
				box_array[x].is_packed = false;
			}

			//BEGIN DO-WHILE
			do
			{
				layerinlayer = 0;
				layerdone = 0;
				
				if (pack_layer())
				{
					exit(1);
				}
				packedy = packedy + layerthickness;
				remainpy = pallet_y - packedy;
				
				if (layerinlayer)
				{
					prepackedy = packedy;
					preremainpy = remainpy;
					remainpy = layerthickness - prelayer;
					packedy = packedy - layerthickness + prelayer;
					remainpz = lilz;
					layerthickness = layerinlayer;
					layerdone = 0;

					if (pack_layer())
					{
						exit(1);
					}

					packedy = prepackedy;
					remainpy = preremainpy;
					remainpz = pallet_z;
				}
				find_layer(remainpy);
			} 
			while (packing);
			// END DO-WHILE

			if (packedvolume > best_solution_volume)
			{
				// 최적 packing 정보를 갱신
				// 이 정보들은 따로이 구조화가 필요하다
				best_solution_volume = packedvolume;
				best_variant = variant;
				best_iteration = itelayer;
				number_packed_boxes = packednumbox;
			}

			// 공간을 100% 다 활용했다면 끝냄
			if (hundredpercent)
				break;

			pallet_volume_used_percentage = best_solution_volume * 100 / total_pallet_volume;
		}

		// 공간을 100% 다 활용했다면 끝냄
		if (hundredpercent)
			break;

		// 정육면체라면, orientation은 6으로 고정
		if ((xx == yy) && (yy == zz))
			variant = 6;
	}
}

void Boxologic::list_candidate_layers()
{
	short int exdim; // 기준으로 삼을 축에서의 크기
	short int dimdif;
	short int dimen2, dimen3; // 기준 외 잔여 축에서의 크기
	short int y, z, k;
	long int layereval;

	layerlistlen = 0;

	for (x = 1; x <= total_boxes; x++)
	{
		for (y = 1; y <= 3; y++)
		{
			// 기준 축으로부터 기준 크기 (key) 를 뽑아옴
			switch (y)
			{
			case 1:
				exdim = box_array[x].width;
				dimen2 = box_array[x].height;
				dimen3 = box_array[x].length;
				break;
			case 2:
				exdim = box_array[x].height;
				dimen2 = box_array[x].width;
				dimen3 = box_array[x].length;
				break;
			case 3:
				exdim = box_array[x].length;
				dimen2 = box_array[x].width;
				dimen3 = box_array[x].height;
				break;
			}

			if ((exdim > pallet_y) || (((dimen2 > pallet_x) || (dimen3 > pallet_z)) && ((dimen3 > pallet_x) || (dimen2 > pallet_z))))
			{
				// 박스가 화물칸보다 큰 경우
				continue;
			}
			
			// 기존 레이어와 중복되면 SKIP
			if (layer_map.count(exdim) != 0)
				continue;

			layereval = 0;

			// 모든 박스의 최소 크기 (각 박스의 x, y, z축 내 최소 크기) 를 구하여
			// 현재 레이어에 쌓는다 (layereval에 더한다)
			for (z = 1; z <= total_boxes; z++)
			{
				if (!(x == z))
				{
					dimdif = abs(exdim - box_array[z].width);
					if (abs(exdim - box_array[z].height) < dimdif)
					{
						dimdif = abs(exdim - box_array[z].height);
					}
					if (abs(exdim - box_array[z].length) < dimdif)
					{
						dimdif = abs(exdim - box_array[z].length);
					}
					layereval = layereval + dimdif;
				}
			}

			// 레이어 크기와 기준값을 기록함
			layer_map[exdim] = layereval;
		}
	}
	return;
}

int Boxologic::pack_layer() 
{
	short int lenx, lenz, lpz;

	if (!layerthickness)
	{
		packing = false;
		return 0;
	}

	scrap_first->cumx = pallet_x;
	scrap_first->cumz = 0;

	while (1)
	{
		find_smallest_z();

		if (!scrap_min_z->prev && !scrap_min_z->next)
		{
			//*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***

			lenx = scrap_min_z->cumx;
			lpz = remainpz - scrap_min_z->cumz;
			find_box(lenx, layerthickness, remainpy, lpz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			box_array[cboxi].cox = 0;
			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			
			if (cboxx == scrap_min_z->cumx)
			{
				scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
			}
			else
			{
				scrap_min_z->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
				if (scrap_min_z->next == NULL)
				{
					printf("Insufficient memory available\n");
					return 1;
				}
				scrap_min_z->next->next = NULL;
				scrap_min_z->next->prev = scrap_min_z;

				scrap_min_z->next->cumx = scrap_min_z->cumx;
				scrap_min_z->next->cumz = scrap_min_z->cumz;
				
				scrap_min_z->cumx = cboxx;
				scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
			}
			volume_check();
		}
		else if (!scrap_min_z->prev)
		{
			//*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***

			lenx = scrap_min_z->cumx;
			lenz = scrap_min_z->next->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;
			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			if (cboxx == scrap_min_z->cumx)
			{
				box_array[cboxi].cox = 0;
				if (scrap_min_z->cumz + cboxz == scrap_min_z->next->cumz)
				{
					scrap_min_z->cumz = scrap_min_z->next->cumz;
					scrap_min_z->cumx = scrap_min_z->next->cumx;

					struct Scrappad *trash = scrap_min_z->next;
					scrap_min_z->next = scrap_min_z->next->next;
					if (scrap_min_z->next)
					{
						scrap_min_z->next->prev = scrap_min_z;
					}
					free(trash);
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else
			{
				box_array[cboxi].cox = scrap_min_z->cumx - cboxx;
				if (scrap_min_z->cumz + cboxz == scrap_min_z->next->cumz)
				{
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
				}
				else
				{
					scrap_min_z->next->prev = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					if (scrap_min_z->next->prev == NULL)
					{
						printf("Insufficient memory available\n");
						return 1;
					}
					scrap_min_z->next->prev->next = scrap_min_z->next;
					scrap_min_z->next->prev->prev = scrap_min_z;
					scrap_min_z->next = scrap_min_z->next->prev;
					scrap_min_z->next->cumx = scrap_min_z->cumx;
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
					scrap_min_z->next->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			volume_check();
		}
		else if (!scrap_min_z->next)
		{
			//*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***

			lenx = scrap_min_z->cumx - scrap_min_z->prev->cumx;
			lenz = scrap_min_z->prev->cumz - scrap_min_z->cumz;
			lpz = remainpz - (*scrap_min_z).cumz;
			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			box_array[cboxi].cox = scrap_min_z->prev->cumx;

			if (cboxx == scrap_min_z->cumx - scrap_min_z->prev->cumx)
			{
				if (scrap_min_z->cumz + cboxz == scrap_min_z->prev->cumz)
				{
					scrap_min_z->prev->cumx = scrap_min_z->cumx;
					scrap_min_z->prev->next = NULL;
					free(scrap_min_z);
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == scrap_min_z->prev->cumz)
				{
					scrap_min_z->prev->cumx = scrap_min_z->prev->cumx + cboxx;
				}
				else
				{
					scrap_min_z->prev->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					if (scrap_min_z->prev->next == NULL)
					{
						printf("Insufficient memory available\n");
						return 1;
					}
					scrap_min_z->prev->next->prev = scrap_min_z->prev;
					scrap_min_z->prev->next->next = scrap_min_z;
					scrap_min_z->prev = scrap_min_z->prev->next;
					scrap_min_z->prev->cumx = scrap_min_z->prev->prev->cumx + cboxx;
					scrap_min_z->prev->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			volume_check();
		}
		else if (scrap_min_z->prev->cumz == scrap_min_z->next->cumz)
		{
			//*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES ***

			//*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***

			lenx = scrap_min_z->cumx - scrap_min_z->prev->cumx;
			lenz = scrap_min_z->prev->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			if (cboxx == scrap_min_z->cumx - scrap_min_z->prev->cumx)
			{
				box_array[cboxi].cox = scrap_min_z->prev->cumx;
				if (scrap_min_z->cumz + cboxz == scrap_min_z->next->cumz)
				{
					scrap_min_z->prev->cumx = scrap_min_z->next->cumx;
					if (scrap_min_z->next->next)
					{
						scrap_min_z->prev->next = scrap_min_z->next->next;
						scrap_min_z->next->next->prev = scrap_min_z->prev;
						free(scrap_min_z);
					}
					else
					{
						scrap_min_z->prev->next = NULL;
						free(scrap_min_z);
					}
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else if (scrap_min_z->prev->cumx < pallet_x - scrap_min_z->cumx)
			{
				if (scrap_min_z->cumz + cboxz == scrap_min_z->prev->cumz)
				{
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
					box_array[cboxi].cox = scrap_min_z->cumx - cboxx;
				}
				else
				{
					box_array[cboxi].cox = scrap_min_z->prev->cumx;
					scrap_min_z->prev->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					if (scrap_min_z->prev->next == NULL)
					{
						printf("Insufficient memory available\n");
						return 1;
					}
					scrap_min_z->prev->next->prev = scrap_min_z->prev;
					scrap_min_z->prev->next->next = scrap_min_z;
					scrap_min_z->prev = scrap_min_z->prev->next;
					scrap_min_z->prev->cumx = scrap_min_z->prev->prev->cumx + cboxx;
					scrap_min_z->prev->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == scrap_min_z->prev->cumz)
				{
					scrap_min_z->prev->cumx = scrap_min_z->prev->cumx + cboxx;
					box_array[cboxi].cox = scrap_min_z->prev->cumx;
				}
				else
				{
					box_array[cboxi].cox = scrap_min_z->cumx - cboxx;
					scrap_min_z->next->prev = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					if (scrap_min_z->next->prev == NULL)
					{
						printf("Insufficient memory available\n");
						return 1;
					}
					scrap_min_z->next->prev->next = scrap_min_z->next;
					scrap_min_z->next->prev->prev = scrap_min_z;
					scrap_min_z->next = scrap_min_z->next->prev;
					scrap_min_z->next->cumx = scrap_min_z->cumx;
					scrap_min_z->next->cumz = scrap_min_z->cumz + cboxz;
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
				}
			}
			volume_check();
		}
		else
		{
			//*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***

			lenx = scrap_min_z->cumx - scrap_min_z->prev->cumx;
			lenz = scrap_min_z->prev->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;
			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			box_array[cboxi].cox = scrap_min_z->prev->cumx;
			if (cboxx == scrap_min_z->cumx - scrap_min_z->prev->cumx)
			{
				if (scrap_min_z->cumz + cboxz == scrap_min_z->prev->cumz)
				{
					scrap_min_z->prev->cumx = scrap_min_z->cumx;
					scrap_min_z->prev->next = scrap_min_z->next;
					scrap_min_z->next->prev = scrap_min_z->prev;
					free(scrap_min_z);
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == scrap_min_z->prev->cumz)
				{
					scrap_min_z->prev->cumx = scrap_min_z->prev->cumx + cboxx;
				}
				else if (scrap_min_z->cumz + cboxz == scrap_min_z->next->cumz)
				{
					box_array[cboxi].cox = scrap_min_z->cumx - cboxx;
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
				}
				else
				{
					scrap_min_z->prev->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					if (scrap_min_z->prev->next == NULL)
					{
						printf("Insufficient memory available\n");
						return 1;
					}
					scrap_min_z->prev->next->prev = scrap_min_z->prev;
					scrap_min_z->prev->next->next = scrap_min_z;
					scrap_min_z->prev = scrap_min_z->prev->next;
					scrap_min_z->prev->cumx = scrap_min_z->prev->prev->cumx + cboxx;
					scrap_min_z->prev->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			volume_check();
		}
	}
	return 0;
}

int Boxologic::find_layer(short int thickness)
{
	short int exdim, dimdif, dimen2, dimen3, y, z;
	long int layereval, eval;
	layerthickness = 0;
	eval = 1000000;
	for (x = 1; x <= total_boxes; x++)
	{
		if (box_array[x].is_packed) continue;
		for (y = 1; y  <= 3; y++)
		{
			switch (y)
			{
			case 1:
				exdim = box_array[x].width;
				dimen2 = box_array[x].height;
				dimen3 = box_array[x].length;
				break;
			case 2:
				exdim = box_array[x].height;
				dimen2 = box_array[x].width;
				dimen3 = box_array[x].length;
				break;
			case 3:
				exdim = box_array[x].length;
				dimen2 = box_array[x].width;
				dimen3 = box_array[x].height;
				break;
			}
			layereval = 0;
			if ((exdim <= thickness) && (((dimen2 <= pallet_x) && (dimen3 <= pallet_z)) || ((dimen3 <= pallet_x) && (dimen2 <= pallet_z))))
			{
				for (z = 1; z <= total_boxes; z++)
				{
					if (!(x == z) && !(box_array[z].is_packed))
					{
						dimdif = abs(exdim - box_array[z].width);
						if (abs(exdim - box_array[z].height) < dimdif)
						{
							dimdif = abs(exdim - box_array[z].height);
						}
						if (abs(exdim - box_array[z].length) < dimdif)
						{
							dimdif = abs(exdim - box_array[z].length);
						}
						layereval = layereval + dimdif;
					}
				}
				if (layereval < eval)
				{
					eval = layereval;
					layerthickness = exdim;
				}
			}
		}
	}
	if (layerthickness == 0 || layerthickness > remainpy) packing = false;
	return 0;
}

void Boxologic::find_box(short int hmx, short int hy, short int hmy, short int hz, short int hmz)
{
	bfx = 32767; bfy = 32767; bfz = 32767;
	bbfx = 32767; bbfy = 32767; bbfz = 32767;
	boxi = 0; bboxi = 0;

	for (x = 1; x <= total_boxes; x++)
	{
		if (box_array[x].is_packed) 
			continue;

		if (x > total_boxes) return;
		analyze_box(hmx, hy, hmy, hz, hmz, box_array[x].width, box_array[x].height, box_array[x].length);
		if ((box_array[x].width == box_array[x].length) && (box_array[x].length == box_array[x].height)) continue;
		analyze_box(hmx, hy, hmy, hz, hmz, box_array[x].width, box_array[x].length, box_array[x].height);
		analyze_box(hmx, hy, hmy, hz, hmz, box_array[x].height, box_array[x].width, box_array[x].length);
		analyze_box(hmx, hy, hmy, hz, hmz, box_array[x].height, box_array[x].length, box_array[x].width);
		analyze_box(hmx, hy, hmy, hz, hmz, box_array[x].length, box_array[x].width, box_array[x].height);
		analyze_box(hmx, hy, hmy, hz, hmz, box_array[x].length, box_array[x].height, box_array[x].width);
	}
}

void Boxologic::analyze_box(short int hmx, short int hy, short int hmy, short int hz, short int hmz, short int dim1, short int dim2, short int dim3)
{
	if (dim1 <= hmx && dim2 <= hmy && dim3 <= hmz)
	{
		if (dim2 <= hy)
		{
			if (hy - dim2 < bfy)
			{
				boxx = dim1;
				boxy = dim2;
				boxz = dim3;
				bfx = hmx - dim1;
				bfy = hy - dim2;
				bfz = abs(hz - dim3);
				boxi = x;
			}
			else if (hy - dim2 == bfy && hmx - dim1 < bfx)
			{
				boxx = dim1;
				boxy = dim2;
				boxz = dim3;
				bfx = hmx - dim1;
				bfy = hy - dim2;
				bfz = abs(hz - dim3);
				boxi = x;
			}
			else if (hy - dim2 == bfy && hmx - dim1 == bfx && abs(hz - dim3) < bfz)
			{
				boxx = dim1;
				boxy = dim2;
				boxz = dim3;
				bfx = hmx - dim1;
				bfy = hy - dim2;
				bfz = abs(hz - dim3);
				boxi = x;
			}
		}
		else
		{
			if (dim2 - hy < bbfy)
			{
				bboxx = dim1;
				bboxy = dim2;
				bboxz = dim3;
				bbfx = hmx - dim1;
				bbfy = dim2 - hy;
				bbfz = abs(hz - dim3);
				bboxi = x;
			}
			else if (dim2 - hy == bbfy && hmx - dim1 < bbfx)
			{
				bboxx = dim1;
				bboxy = dim2;
				bboxz = dim3;
				bbfx = hmx - dim1;
				bbfy = dim2 - hy;
				bbfz = abs(hz - dim3);
				bboxi = x;
			}
			else if (dim2 - hy == bbfy && hmx - dim1 == bbfx && abs(hz - dim3) < bbfz)
			{
				bboxx = dim1;
				bboxy = dim2;
				bboxz = dim3;
				bbfx = hmx - dim1;
				bbfy = dim2 - hy;
				bbfz = abs(hz - dim3);
				bboxi = x;
			}
		}
	}
}

void Boxologic::find_smallest_z()
{
	struct Scrappad *scrapmemb = scrap_first;
	scrap_min_z = scrapmemb;

	while (!(scrapmemb->next == NULL))
	{
		if (scrapmemb->next->cumz < scrap_min_z->cumz)
		{
			scrap_min_z = scrapmemb->next;
		}
		scrapmemb = scrapmemb->next;
	}
	return;
}

void Boxologic::checkfound()
{
	evened = 0;
	if (boxi)
	{
		cboxi = boxi;
		cboxx = boxx;
		cboxy = boxy;
		cboxz = boxz;
	}
	else
	{
		if ((bboxi > 0) && (layerinlayer || (!scrap_min_z->prev && !scrap_min_z->next)))
		{
			if (!layerinlayer)
			{
				prelayer = layerthickness;
				lilz = scrap_min_z->cumz;
			}
			cboxi = bboxi;
			cboxx = bboxx;
			cboxy = bboxy;
			cboxz = bboxz;
			layerinlayer = layerinlayer + bboxy - layerthickness;
			layerthickness = bboxy;
		}
		else
		{
			if (!scrap_min_z->prev && !scrap_min_z->next)
			{
				layerdone = 1;
			}
			else
			{
				evened = 1;
				if (!scrap_min_z->prev)
				{
					struct Scrappad *trash = scrap_min_z->next;
					scrap_min_z->cumx = scrap_min_z->next->cumx;
					scrap_min_z->cumz = scrap_min_z->next->cumz;
					scrap_min_z->next = scrap_min_z->next->next;
					if (scrap_min_z->next)
					{
						scrap_min_z->next->prev = scrap_min_z;
					}
					free(trash);
				}
				else if (!scrap_min_z->next)
				{
					scrap_min_z->prev->next = NULL;
					scrap_min_z->prev->cumx = scrap_min_z->cumx;
					free(scrap_min_z);
				}
				else
				{
					if (scrap_min_z->prev->cumz == scrap_min_z->next->cumz)
					{
						scrap_min_z->prev->next = scrap_min_z->next->next;
						if (scrap_min_z->next->next)
						{
							scrap_min_z->next->next->prev = scrap_min_z->prev;
						}
						scrap_min_z->prev->cumx = scrap_min_z->next->cumx;
						free(scrap_min_z->next);
						free(scrap_min_z);
					}
					else
					{
						scrap_min_z->prev->next = scrap_min_z->next;
						scrap_min_z->next->prev = scrap_min_z->prev;
						if (scrap_min_z->prev->cumz < scrap_min_z->next->cumz)
						{
							scrap_min_z->prev->cumx = scrap_min_z->cumx;
						}
						free(scrap_min_z);
					}
				}
			}
		}
	}
	return;
}

void Boxologic::volume_check()
{
	box_array[cboxi].is_packed = true;
	box_array[cboxi].layout_width = cboxx;
	box_array[cboxi].layout_height = cboxy;
	box_array[cboxi].layout_length = cboxz;
	
	packedvolume = packedvolume + box_array[cboxi].vol;
	packednumbox++;
	
	if (packingbest)
	{
		// boxologic은 따로이 최적 방향을 저장하지 않으니
		// 별도의 핸들링이 필요하다
		write_boxlist_file();
	}
	else if (packedvolume == total_pallet_volume || packedvolume == total_box_volume)
	{
		packing = false;
		hundredpercent = 1;
	}
	return;
}

void Boxologic::report_results(void)
{
	switch (best_variant)
	{
	case 1:
		pallet_x = xx; pallet_y = yy; pallet_z = zz;
		break;
	case 2:
		pallet_x = zz; pallet_y = yy; pallet_z = xx;
		break;
	case 3:
		pallet_x = zz; pallet_y = xx; pallet_z = yy;
		break;
	case 4:
		pallet_x = yy; pallet_y = xx; pallet_z = zz;
		break;
	case 5:
		pallet_x = xx; pallet_y = zz; pallet_z = yy;
		break;
	case 6:
		pallet_x = yy; pallet_y = zz; pallet_z = xx;
		break;
	}
	packingbest = 1;

	packed_box_percentage = best_solution_volume * 100 / total_box_volume;
	pallet_volume_used_percentage = best_solution_volume * 100 / total_pallet_volume;
	
	list_candidate_layers();
	packedvolume = 0.0;
	packedy = 0;
	packing = true;

	layerthickness = best_iteration;
	remainpy = pallet_y;
	remainpz = pallet_z;

	for (x = 1; x <= total_boxes; x++)
	{
		box_array[x].is_packed = false;
	}

	do
	{
		layerinlayer = 0;
		layerdone = 0;
		pack_layer();
		packedy = packedy + layerthickness;
		remainpy = pallet_y - packedy;
		
		if (layerinlayer)
		{
			prepackedy = packedy;
			preremainpy = remainpy;
			remainpy = layerthickness - prelayer;
			packedy = packedy - layerthickness + prelayer;
			remainpz = lilz;
			layerthickness = layerinlayer;
			layerdone = 0;
			pack_layer();
			packedy = prepackedy;
			remainpy = preremainpy;
			remainpz = pallet_z;
		}
		find_layer(remainpy);
	} 
	while (packing);
}

void Boxologic::write_boxlist_file()
{
	short int x, y, z, bx, by, bz;
	
	switch (best_variant)
	{
	case 1:
		x = box_array[cboxi].cox;
		y = box_array[cboxi].coy;
		z = box_array[cboxi].coz;
		bx = box_array[cboxi].layout_width;
		by = box_array[cboxi].layout_height;
		bz = box_array[cboxi].layout_length;
		break;
	case 2:
		x = box_array[cboxi].coz;
		y = box_array[cboxi].coy;
		z = box_array[cboxi].cox;
		bx = box_array[cboxi].layout_length;
		by = box_array[cboxi].layout_height;
		bz = box_array[cboxi].layout_width;
		break;
	case 3:
		x = box_array[cboxi].coy;
		y = box_array[cboxi].coz;
		z = box_array[cboxi].cox;
		bx = box_array[cboxi].layout_height;
		by = box_array[cboxi].layout_length;
		bz = box_array[cboxi].layout_width;
		break;
	case 4:
		x = box_array[cboxi].coy;
		y = box_array[cboxi].cox;
		z = box_array[cboxi].coz;
		bx = box_array[cboxi].layout_height;
		by = box_array[cboxi].layout_width;
		bz = box_array[cboxi].layout_length;
		break;
	case 5:
		x = box_array[cboxi].cox;
		y = box_array[cboxi].coz;
		z = box_array[cboxi].coy;
		bx = box_array[cboxi].layout_width;
		by = box_array[cboxi].layout_length;
		bz = box_array[cboxi].layout_height;
		break;
	case 6:
		x = box_array[cboxi].coz;
		y = box_array[cboxi].cox;
		z = box_array[cboxi].coy;
		bx = box_array[cboxi].layout_length;
		by = box_array[cboxi].layout_width;
		bz = box_array[cboxi].layout_height;
		break;
	}

	box_array[cboxi].cox = x;
	box_array[cboxi].coy = y;
	box_array[cboxi].coz = z;
	box_array[cboxi].layout_width = bx;
	box_array[cboxi].layout_height = by;
	box_array[cboxi].layout_length = bz;
}

int Boxologic::compute_layer_list(const void *i, const void *j)
{
	return *(long int*)i - *(long int*)j;
}