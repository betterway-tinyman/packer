#include <hiswill/packer/Boxologic.hpp>

#include <hiswill/packer/Wrapper.hpp>
#include <hiswill/packer/InstanceArray.hpp>

#include <algorithm>

using namespace hiswill::packer;

using namespace std;
using namespace samchon::library;
using namespace samchon::protocol;

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
}

void Boxologic::encode()
{
	// READ_BOXLIST_INPUT
	pallet.width = wrapper->getContainableWidth();
	pallet.height = wrapper->getContainableHeight();
	pallet.length = wrapper->getContainableLength();

	box_array.assign(instanceArray->size() + 1, struct BoxInfo());
	layer_map.clear();
	total_boxes = instanceArray->size();

	for (size_t i = 0; i < instanceArray->size(); i++)
	{
		shared_ptr<Instance> instance = instanceArray->at(i);
		size_t k = i + 1;

		box_array[k].width = instance->getWidth();
		box_array[k].height = instance->getHeight();
		box_array[k].length = instance->getLength();

		box_array[k].volume = instance->getVolume();
		box_array[k].is_packed = false;
	}

	// INITIALIZE
	temp = 1.0;
	pallet.volume = temp * pallet.width * pallet.height * pallet.length;
	total_box_volume = 0.0;

	for (size_t x = 1; x <= total_boxes; x++) 
		total_box_volume = total_box_volume + box_array[x].volume;
	
	scrap_list.emplace_back();

	best_solution_volume = 0.0;
	packing_best = false;
	hundred_percent = false;
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
	GETTERS
----------------------------------------------------------- */
auto Boxologic::fetch_scrap_min_z_left() -> list<struct Scrappad>::iterator
{
	if (scrap_min_z == scrap_list.begin())
		return scrap_list.end();
	else
	{
		auto it = scrap_min_z;
		it--;

		return it;
	}
}

auto Boxologic::fetch_scrap_min_z_right()->list<struct Scrappad>::iterator
{
	auto it = scrap_min_z;
	it++;

	return it;
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
			pallet.layout_width = pallet.width; 
			pallet.layout_height = pallet.height; 
			pallet.layout_length = pallet.length;
			break;
		case 2:
			pallet.layout_width = pallet.length; 
			pallet.layout_height = pallet.height; 
			pallet.layout_length = pallet.width;
			break;
		case 3:
			pallet.layout_width = pallet.length; 
			pallet.layout_height = pallet.width; 
			pallet.layout_length = pallet.height;
			break;
		case 4:
			pallet.layout_width = pallet.height; 
			pallet.layout_height = pallet.width; 
			pallet.layout_length = pallet.length;
			break;
		case 5:
			pallet.layout_width = pallet.width; 
			pallet.layout_height = pallet.length; 
			pallet.layout_length = pallet.height;
			break;
		case 6:
			pallet.layout_width = pallet.height; 
			pallet.layout_height = pallet.length; 
			pallet.layout_length = pallet.width;
			break;
		}

		list_candidate_layers();

		for (auto it = layer_map.begin(); it != layer_map.end(); it++)
		{
			++number_of_iterations;
			
			packed_volume = 0.0;
			packedy = 0;
			packing = true;
			
			layer_thickness = it->first;
			itelayer = it->first;
			remainpy = pallet.layout_height;
			remainpz = pallet.layout_length;
			packed_boxes = 0;

			for (size_t x = 1; x <= total_boxes; x++)
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
				packedy = packedy + layer_thickness;
				remainpy = pallet.layout_height - packedy;
				
				if (layerinlayer)
				{
					prepackedy = packedy;
					preremainpy = remainpy;
					remainpy = layer_thickness - prelayer;
					packedy = packedy - layer_thickness + prelayer;
					remainpz = lilz;
					layer_thickness = layerinlayer;
					layerdone = 0;

					if (pack_layer())
					{
						exit(1);
					}

					packedy = prepackedy;
					remainpy = preremainpy;
					remainpz = pallet.layout_length;
				}
				find_layer(remainpy);
			} 
			while (packing);
			// END DO-WHILE

			if (packed_volume > best_solution_volume)
			{
				// 최적 packing 정보를 갱신
				// 이 정보들은 따로이 구조화가 필요하다
				best_solution_volume = packed_volume;
				best_variant = variant;
				best_iteration = itelayer;
				number_packed_boxes = packed_boxes;
			}

			// 공간을 100% 다 활용했다면 끝냄
			if (hundred_percent)
				break;

			pallet_volume_used_percentage = best_solution_volume * 100 / pallet.volume;
		}

		// 공간을 100% 다 활용했다면 끝냄
		if (hundred_percent)
			break;

		// 정육면체라면, orientation은 6으로 고정
		if ((pallet.width == pallet.height) && (pallet.height == pallet.length))
			variant = 6;
	}
}

void Boxologic::list_candidate_layers()
{
	double exdim; // 기준으로 삼을 축에서의 크기
	double dimdif;
	double dimen2, dimen3; // 기준 외 잔여 축에서의 크기
	size_t x, y, z;
	double layereval;

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

			if ((exdim > pallet.layout_height) || (((dimen2 > pallet.layout_width) || (dimen3 > pallet.layout_length)) && ((dimen3 > pallet.layout_width) || (dimen2 > pallet.layout_length))))
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
	double lenx, lenz, lpz;

	if (!layer_thickness)
	{
		packing = false;
		return 0;
	}

	scrap_list.begin()->cumx = pallet.layout_width;
	scrap_list.begin()->cumz = 0;

	while (1)
	{
		// INIT SCRAP_MIN_Z
		find_smallest_z();

		// FETCH LEFT AND RIGHT OF SCRAP_MIN_Z
		auto &left = fetch_scrap_min_z_left();
		auto &right = fetch_scrap_min_z_right();

		if (left == scrap_list.end() && right == scrap_list.end())
		{
			/////////////////////////////////////////////////////////
			// NO LEFT AND RIGHT
			/////////////////////////////////////////////////////////
			//*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***
			lenx = scrap_min_z->cumx;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layer_thickness, remainpy, lpz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			// RE-FETCH LEFT AND RIGHT
			left = fetch_scrap_min_z_left();
			right = fetch_scrap_min_z_right();

			box_array[cboxi].cox = 0;
			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			
			if (cboxx == scrap_min_z->cumx)
			{
				scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
			}
			else
			{
				// CREATE A NEW NODE AND IT'S THE NEW MIN_Z
				// ORDINARY MIN_Z WILL BE SHIFTED TO THE RIGHT
				struct Scrappad node =
				{
					cboxx,
					scrap_min_z->cumz + cboxz
				};

				// SHIFTS ORDINARY MIN_Z TO RIGHT
				// AND THE NEW NODE'S ITERATOR IS THE NEW MIN_Z FROM NOW ON
				scrap_min_z = scrap_list.insert(scrap_min_z, node);

				/*scrap_min_z->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
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
				scrap_min_z->cumz = scrap_min_z->cumz + cboxz;*/
			}
			volume_check();
		}
		else if (left == scrap_list.end())
		{
			//*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***

			lenx = scrap_min_z->cumx;
			lenz = right->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layer_thickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			// RE-FETCH LEFT AND RIGHT
			left = fetch_scrap_min_z_left();
			right = fetch_scrap_min_z_right();

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;

			if (cboxx == scrap_min_z->cumx)
			{
				box_array[cboxi].cox = 0;

				if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					// RIGHT IS THE NEW MIN_Z
					// ORDINARY MIN_Z WILL BE ERASED
					scrap_min_z = scrap_list.erase(scrap_min_z);

					//// SCRAP_MIN_Z FETCHES RIGHT'S MEMBERS
					//scrap_min_z->cumz = scrap_min_z->next->cumz;
					//scrap_min_z->cumx = scrap_min_z->next->cumx;

					//// AND THE RIGHT IS ERASED
					//// THUS IT MEANS TO ERASE SCRAP_MIN_Z AND
					//// RIGHT IS THE NEW SCRAP_MIN_Z
					//struct Scrappad *trash = scrap_min_z->next;
					//scrap_min_z->next = scrap_min_z->next->next;
					//if (scrap_min_z->next)
					//{
					//	scrap_min_z->next->prev = scrap_min_z;
					//}
					//free(trash);
				}
				else
				{
					scrap_min_z->cumz += cboxz;
				}
			}
			else
			{
				box_array[cboxi].cox = scrap_min_z->cumx - cboxx;

				if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					scrap_min_z->cumx -= cboxx;
				}
				else
				{
					// UPDATE MIN_Z
					scrap_min_z->cumx -= cboxx;

					// CREATE A NEW NODE BETWEEN MIN_Z AND RIGHT
					struct Scrappad node =
					{
						scrap_min_z->cumx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(right, node);

					//scrap_min_z->next->prev = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					//if (scrap_min_z->next->prev == NULL)
					//{
					//	cout << "Insufficient memory available" << endl;
					//	return 1;
					//}

					//// THE NEW NODE IS ON THE LEFT SIDE OF ORDINARY MIN_Z
					//(scrap_min_z->next->prev)->next = scrap_min_z->next;
					//(scrap_min_z->next->prev)->prev = scrap_min_z;
					//scrap_min_z->next = (scrap_min_z->next->prev);

					//// ORDINARY'S CUM_X:
					//scrap_min_z->cumx = scrap_min_z->cumx - cboxx;

					//// THE NEW NODE IS MIN_Z
					//(scrap_min_z->next)->cumx = scrap_min_z->cumx;
					//(scrap_min_z->next)->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			volume_check();
		}
		else if (right == scrap_list.end())
		{
			////////////////////////////////////////////////////////
			// NO RIGHT BUT LEFT
			/////////////////////////////////////////////////////////
			//*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***
			lenx = scrap_min_z->cumx - left->cumx;
			lenz = left->cumz - scrap_min_z->cumz;
			lpz = remainpz - (*scrap_min_z).cumz;

			find_box(lenx, layer_thickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			// RE-FETCH LEFT AND RIGHT
			left = fetch_scrap_min_z_left();
			right = fetch_scrap_min_z_right();

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			box_array[cboxi].cox = left->cumx;

			if (cboxx == scrap_min_z->cumx - left->cumx)
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					// LEFT FETCHES MIN_Z'S CUM_X
					left->cumx = scrap_min_z->cumx;

					// ERASE FROM MIN_Z TO END
					//scrap_list.erase(scrap_min_z, scrap_list.end());
					//scrap_min_z = scrap_list.end();

					scrap_min_z = scrap_list.erase(scrap_min_z, scrap_list.end());

					//scrap_min_z->prev->cumx = scrap_min_z->cumx;
					//scrap_min_z->prev->next = NULL;
					//free(scrap_min_z);
				}
				else
				{
					scrap_min_z->cumz += cboxz;
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					left->cumx += cboxx;
				}
				else
				{
					struct Scrappad node =
					{
						left->cumx + cboxx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(scrap_min_z, node);

					//// CREATE A NEW NODE BETWEEN LEFT AND MIN_Z
					//scrap_min_z->prev->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					//if (scrap_min_z->prev->next == NULL)
					//{
					//	cout << "Insufficient memory available" << endl;
					//	return 1;
					//}

					//// THE NEW NODE BETWEEN LEFT AND MIN_Z
					//(scrap_min_z->prev->next)->prev = scrap_min_z->prev;
					//(scrap_min_z->prev->next)->next = scrap_min_z;
					//scrap_min_z->prev = scrap_min_z->prev->next;

					//// THE NEW NODE'S CUM_X AND CUM_Z:
					//(scrap_min_z->prev)->cumx = scrap_min_z->prev->prev->cumx + cboxx;
					//(scrap_min_z->prev)->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			volume_check();
		}
		else if (left->cumz == right->cumz)
		{
			////////////////////////////////////////////////////////
			// LEFT AND RIGHT ARE ALL EXIST
			/////////////////////////////////////////////////////////
			//*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES ***
			//*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***

			lenx = scrap_min_z->cumx - left->cumx;
			lenz = left->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layer_thickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;

			if (cboxx == scrap_min_z->cumx - left->cumx)
			{
				box_array[cboxi].cox = left->cumx;

				if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					// LEFT FETCHES RIGHT'S CUM_X
					left->cumx = right->cumx;

					// ERASE FROM MIN_Z TO RIGHT
					auto right_of_right = right;
					right_of_right++; // RIGHT'S RIGHT OR END MAYBE

									  //scrap_list.erase(scrap_min_z, right_of_right);
									  //scrap_min_z = scrap_list.end();

					scrap_min_z = scrap_list.erase(scrap_min_z, right_of_right);

					//if (scrap_min_z->next->next)
					//{
					//	// ERASE FROM MIN_Z TO RIGHT
					//	scrap_min_z->prev->next = scrap_min_z->next->next;
					//	scrap_min_z->next->next->prev = scrap_min_z->prev;
					//	free(scrap_min_z);
					//}
					//else
					//{
					//	// ERASE FROM MIN_Z TO END
					//	scrap_min_z->prev->next = NULL;
					//	free(scrap_min_z);
					//}
				}
				else
				{
					scrap_min_z->cumz += cboxz;
				}
			}
			else if (left->cumx < pallet.layout_width - scrap_min_z->cumx)
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					// WEIRDO PART
					// BOX_ARRAY[CBOXI].COX IS DOUBLY MINUSED
					scrap_min_z->cumx -= cboxx;
					box_array[cboxi].cox = scrap_min_z->cumx;

					//box_array[cboxi].cox = scrap_min_z->cumx - cboxx;
				}
				else
				{
					box_array[cboxi].cox = left->cumx;

					// CREATE A NODE BETWEEN LEFT AND MIN_Z
					struct Scrappad node =
					{
						left->cumx + cboxx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(scrap_min_z, node);

					//// CREATE A NODE BETWEEN LEFT AND MIN_Z
					//scrap_min_z->prev->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					//if (scrap_min_z->prev->next == NULL)
					//{
					//	cout << "Insufficient memory available" << endl;
					//	return 1;
					//}
					//(scrap_min_z->prev->next)->prev = scrap_min_z->prev;
					//(scrap_min_z->prev->next)->next = scrap_min_z;
					//scrap_min_z->prev = (scrap_min_z->prev->next);

					//// THE NEW NODE'S MEMBERS ARE:
					//(scrap_min_z->prev)->cumx = scrap_min_z->prev->prev->cumx + cboxx;
					//(scrap_min_z->prev)->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					left->cumx += cboxx;
					box_array[cboxi].cox = left->cumx;
				}
				else
				{
					box_array[cboxi].cox = scrap_min_z->cumx - cboxx;

					// CREATE A NODE BETWEEN MIN_Z AND RIGHT
					struct Scrappad node =
					{
						scrap_min_z->cumx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(right, node);

					// UPDATE MIN_Z
					scrap_min_z->cumx -= cboxx;

					//// CREATE A NODE BETWEEN MIN_Z AND RIGHT
					//scrap_min_z->next->prev = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					//if (scrap_min_z->next->prev == NULL)
					//{
					//	cout << "Insufficient memory available" << endl;
					//	return 1;
					//}
					//(scrap_min_z->next->prev)->next = scrap_min_z->next;
					//(scrap_min_z->next->prev)->prev = scrap_min_z;
					//scrap_min_z->next = (scrap_min_z->next->prev);

					//// THE NEW NODE'S MEMBERS:
					//(scrap_min_z->next)->cumx = scrap_min_z->cumx;
					//(scrap_min_z->next)->cumz = scrap_min_z->cumz + cboxz;
					//
					//// UPDATE MIN_Z
					//scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
				}
			}
			volume_check();
		}
		else
		{
			////////////////////////////////////////////////////////
			// LEFT AND RIGHT ARE ALL EXIST
			////////////////////////////////////////////////////////
			//*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***
			lenx = scrap_min_z->cumx - left->cumx;
			lenz = left->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layer_thickness, remainpy, lenz, lpz);
			checkfound();

			if (layerdone) break;
			if (evened) continue;

			// RE-FETCH LEFT AND RIGHT
			left = fetch_scrap_min_z_left();
			right = fetch_scrap_min_z_right();

			box_array[cboxi].coy = packedy;
			box_array[cboxi].coz = scrap_min_z->cumz;
			box_array[cboxi].cox = left->cumx;

			if (cboxx == scrap_min_z->cumx - left->cumx)
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					// LEFT FETCHES MIN_Z'S
					left->cumx = scrap_min_z->cumx;

					// ERASE MIN_Z
					//scrap_list.erase(scrap_min_z);
					//scrap_min_z = scrap_list.end();

					scrap_min_z = scrap_list.erase(scrap_min_z);

					//// LEFT FETCHES MIN_Z'S
					//scrap_min_z->prev->cumx = scrap_min_z->cumx;

					//// ERASE MIN_Z
					//scrap_min_z->prev->next = scrap_min_z->next;
					//scrap_min_z->next->prev = scrap_min_z->prev;
					//free(scrap_min_z);
				}
				else
				{
					scrap_min_z->cumz += cboxz;
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					left->cumx += cboxx;
				}
				else if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					box_array[cboxi].cox = scrap_min_z->cumx - cboxx;
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
				}
				else
				{
					// CREATE NODE BETWEEN LEFT AND MIN_Z
					struct Scrappad node =
					{
						left->cumx + cboxx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(scrap_min_z, node);

					//// CREATE NODE BETWEEN LEFT AND MIN_Z
					//scrap_min_z->prev->next = (struct Scrappad*)malloc(sizeof(struct Scrappad));
					//if (scrap_min_z->prev->next == NULL)
					//{
					//	cout << "Insufficient memory available" << endl;
					//	return 1;
					//}
					//(scrap_min_z->prev->next)->prev = scrap_min_z->prev;
					//(scrap_min_z->prev->next)->next = scrap_min_z;
					//scrap_min_z->prev = scrap_min_z->prev->next;

					//// MEMBERS OF THE NEW NODE:
					//(scrap_min_z->prev)->cumx = scrap_min_z->prev->prev->cumx + cboxx;
					//(scrap_min_z->prev)->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			volume_check();
		}
	}
	return 0;
}

int Boxologic::find_layer(double thickness)
{
	double exdim, dimdif, dimen2, dimen3;
	double layereval, eval;
	layer_thickness = 0;
	eval = 1000000;

	size_t y, z;

	for (size_t x = 1; x <= total_boxes; x++)
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
			if ((exdim <= thickness) && (((dimen2 <= pallet.layout_width) && (dimen3 <= pallet.layout_length)) || ((dimen3 <= pallet.layout_width) && (dimen2 <= pallet.layout_length))))
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
					layer_thickness = exdim;
				}
			}
		}
	}
	if (layer_thickness == 0 || layer_thickness > remainpy) packing = false;
	return 0;
}

void Boxologic::find_box(double hmx, double hy, double hmy, double hz, double hmz)
{
	bfx = 32767; bfy = 32767; bfz = 32767;
	bbfx = 32767; bbfy = 32767; bbfz = 32767;
	boxi = 0; bboxi = 0;

	for (size_t x = 1; x <= total_boxes; x++)
	{
		if (box_array[x].is_packed) 
			continue;

		if (x > total_boxes) 
			return;

		analyze_box(x, hmx, hy, hmy, hz, hmz, box_array[x].width, box_array[x].height, box_array[x].length);

		if ((box_array[x].width == box_array[x].length) && (box_array[x].length == box_array[x].height)) 
			continue;

		analyze_box(x, hmx, hy, hmy, hz, hmz, box_array[x].width, box_array[x].length, box_array[x].height);
		analyze_box(x, hmx, hy, hmy, hz, hmz, box_array[x].height, box_array[x].width, box_array[x].length);
		analyze_box(x, hmx, hy, hmy, hz, hmz, box_array[x].height, box_array[x].length, box_array[x].width);
		analyze_box(x, hmx, hy, hmy, hz, hmz, box_array[x].length, box_array[x].width, box_array[x].height);
		analyze_box(x, hmx, hy, hmy, hz, hmz, box_array[x].length, box_array[x].height, box_array[x].width);
	}
}

void Boxologic::analyze_box(size_t x, double hmx, double hy, double hmy, double hz, double hmz, double dim1, double dim2, double dim3)
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
	scrap_min_z = scrap_list.begin();

	for (auto it = scrap_list.begin(); it != scrap_list.end(); it++)
		if (it->cumz < scrap_min_z->cumz)
			scrap_min_z = it;

	//struct Scrappad *scrapmemb = scrap_first;
	//scrap_min_z = scrapmemb;

	//while (!(scrapmemb->next == NULL))
	//{
	//	if (scrapmemb->next->cumz < scrap_min_z->cumz)
	//	{
	//		scrap_min_z = scrapmemb->next;
	//	}
	//	scrapmemb = scrapmemb->next;
	//}
	//return;
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
		auto &left = fetch_scrap_min_z_left();
		auto &right = fetch_scrap_min_z_right();

		if ((bboxi > 0) && (layerinlayer || (left == scrap_list.end() && right == scrap_list.end())))
		{
			////////////////////////////////////////////
			// ~ OR SCRAP_MIN_Z HAS NO NEIGHBOR
			////////////////////////////////////////////
			if (!layerinlayer)
			{
				prelayer = layer_thickness;
				lilz = scrap_min_z->cumz;
			}

			cboxi = bboxi;
			cboxx = bboxx;
			cboxy = bboxy;
			cboxz = bboxz;

			layerinlayer = layerinlayer + bboxy - layer_thickness;
			layer_thickness = bboxy;
		}
		else
		{
			if (left == scrap_list.end() && right == scrap_list.end())
			{
				//////
				// ^~ AND SCRAP_MIN_Z HAS NO NEIGHBOR
				//////
				layerdone = true;
			}
			else
			{
				evened = true;

				if (left == scrap_list.end())
				{
					///////////////////////////////////////////
					// NO LEFT, BUT RIGHT
					///////////////////////////////////////////
					// ERASE SCRAP_MIN_Z
					// RIGHT IS THE NEW SCRAP_MIN_Z
					scrap_min_z = scrap_list.erase(scrap_min_z);

					//struct Scrappad *trash = scrap_min_z->next;
					//scrap_min_z->cumx = scrap_min_z->next->cumx;
					//scrap_min_z->cumz = scrap_min_z->next->cumz;
					//scrap_min_z->next = scrap_min_z->next->next;
					//if (scrap_min_z->next)
					//{
					//	scrap_min_z->next->prev = scrap_min_z;
					//}
					//free(trash);
				}
				else if (right == scrap_list.end())
				{
					///////////////////////////////////////////
					// NO RIGHT, BUT LEFT
					///////////////////////////////////////////
					// ERASE CURRENT SCRAP_MIN_Z
					// THE LEFT ITEM FETCHES MIN'S CUM_X
					left->cumx = scrap_min_z->cumx;

					// ERASE FROM MIN_Z TO END
					//scrap_list.erase(scrap_min_z, scrap_list.end());
					//scrap_min_z = scrap_list.end();

					scrap_min_z = scrap_list.erase(scrap_min_z, scrap_list.end());

					//scrap_min_z->prev->next = NULL;
					//scrap_min_z->prev->cumx = scrap_min_z->cumx;
					//free(scrap_min_z);
				}
				else
				{
					///////////////////////////////////////////
					// LEFT & RIGHT ARE ALL EXIST
					///////////////////////////////////////////
					if (left->cumz == right->cumz)
					{
						// ----------------------------------------
						// LEFT AND RIGHT'S CUM_Z ARE EQUAL
						// ----------------------------------------
						// LEFT FETCHES THE RIGHT'S CUM_X
						left->cumx = right->cumx;

						// ERASE MIN AND ITS RIGHT
						auto right_of_right = right;
						right_of_right++;

						//scrap_list.erase(scrap_min_z, right_of_right);
						//scrap_min_z = scrap_list.end();

						scrap_min_z = scrap_list.erase(scrap_min_z, right_of_right);

						/*scrap_min_z->prev->next = scrap_min_z->next->next;
						if (scrap_min_z->next->next)
						{
						scrap_min_z->next->next->prev = scrap_min_z->prev;
						}
						(scrap_min_z->prev)->cumx = scrap_min_z->next->cumx;
						free(scrap_min_z->next);
						free(scrap_min_z);*/
					}
					else
					{
						// ----------------------------------------
						// LEFT AND RIGHT'S CUM_Z ARE NOT EQUAL
						// ----------------------------------------
						if (left->cumz == right->cumz)
							left->cumx = scrap_min_z->cumx;

						//scrap_list.erase(scrap_min_z);
						//scrap_min_z = scrap_list.end();

						scrap_min_z = scrap_list.erase(scrap_min_z);

						//scrap_min_z->prev->next = scrap_min_z->next;
						//scrap_min_z->next->prev = scrap_min_z->prev;
						//if (scrap_min_z->prev->cumz < scrap_min_z->next->cumz)
						//{
						//	// IF LEFT'S CUM_Z IS LESS THAN RIGHT'S OWN
						//	scrap_min_z->prev->cumx = scrap_min_z->cumx;
						//}
						//// ERASE MIN
						//free(scrap_min_z);
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
	
	packed_volume = packed_volume + box_array[cboxi].volume;
	packed_boxes++;
	
	if (packing_best)
	{
		// boxologic은 따로이 최적 방향을 저장하지 않으니
		// 별도의 핸들링이 필요하다
		write_boxlist_file();
	}
	else if (packed_volume == pallet.volume || packed_volume == total_box_volume)
	{
		packing = false;
		hundred_percent = true;
	}
	return;
}

void Boxologic::report_results(void)
{
	switch (best_variant)
	{
	case 1:
		pallet.layout_width = pallet.width; pallet.layout_height = pallet.height; pallet.layout_length = pallet.length;
		break;
	case 2:
		pallet.layout_width = pallet.length; pallet.layout_height = pallet.height; pallet.layout_length = pallet.width;
		break;
	case 3:
		pallet.layout_width = pallet.length; pallet.layout_height = pallet.width; pallet.layout_length = pallet.height;
		break;
	case 4:
		pallet.layout_width = pallet.height; pallet.layout_height = pallet.width; pallet.layout_length = pallet.length;
		break;
	case 5:
		pallet.layout_width = pallet.width; pallet.layout_height = pallet.length; pallet.layout_length = pallet.height;
		break;
	case 6:
		pallet.layout_width = pallet.height; pallet.layout_height = pallet.length; pallet.layout_length = pallet.width;
		break;
	}
	packing_best = true;

	packed_box_percentage = best_solution_volume * 100 / total_box_volume;
	pallet_volume_used_percentage = best_solution_volume * 100 / pallet.volume;
	
	list_candidate_layers();
	packed_volume = 0.0;
	packedy = 0;
	packing = true;

	layer_thickness = best_iteration;
	remainpy = pallet.layout_height;
	remainpz = pallet.layout_length;

	for (size_t x = 1; x <= total_boxes; x++)
	{
		box_array[x].is_packed = false;
	}

	do
	{
		layerinlayer = 0;
		layerdone = 0;
		pack_layer();
		packedy = packedy + layer_thickness;
		remainpy = pallet.layout_height - packedy;
		
		if (layerinlayer)
		{
			prepackedy = packedy;
			preremainpy = remainpy;
			remainpy = layer_thickness - prelayer;
			packedy = packedy - layer_thickness + prelayer;
			remainpz = lilz;
			layer_thickness = layerinlayer;
			layerdone = 0;
			pack_layer();
			packedy = prepackedy;
			remainpy = preremainpy;
			remainpz = pallet.layout_length;
		}
		find_layer(remainpy);
	} 
	while (packing);
}

void Boxologic::write_boxlist_file()
{
	double x, y, z, bx, by, bz;
	
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