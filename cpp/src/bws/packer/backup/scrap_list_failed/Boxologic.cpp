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

void Boxologic::encode()
{
	// READ_BOXLIST_INPUT
	xx = wrapper->getContainableWidth();
	yy = wrapper->getContainableHeight();
	zz = wrapper->getContainableLength();

	box_list.assign(instanceArray->size() + 1, struct BoxInfo());
	total_boxes = instanceArray->size();

	for (size_t i = 0; i < instanceArray->size(); i++)
	{
		shared_ptr<Instance> instance = instanceArray->at(i);
		size_t k = i + 1;

		box_list[k].width = instance->getWidth();
		box_list[k].height = instance->getHeight();
		box_list[k].length = instance->getLength();

		box_list[k].vol = instance->getVolume();
		box_list[k].is_packed = false;
	}

	// INITIALIZE
	temp = 1.0;
	total_pallet_volume = temp * xx * yy * zz;
	total_box_volume = 0.0;

	for (x = 1; x <= total_boxes; x++) 
		total_box_volume = total_box_volume + box_list[x].vol;
	
	scrap_list.emplace_back();
	
	best_solution_volume = 0.0;
	best_packing = false;
	hundred_percent = false;
}

void Boxologic::decode()
{
	wrapper->clear();
	leftInstanceArray->clear();

	for (size_t k = 1; k <= total_boxes; k++)
	{
		const BoxInfo &box = box_list[k];
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

		for (auto it = layer_list.begin(); it != layer_list.end(); it++)
		{
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
				box_list[x].is_packed = false;
			}

			//BEGIN DO-WHILE
			do
			{
				layerinlayer = 0;
				layer_done = false;
				
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
					layer_done = false;

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
				best_layer = itelayer;
				number_packed_boxes = packednumbox;
			}

			// 공간을 100% 다 활용했다면 끝냄
			if (hundred_percent)
				break;

			pallet_volume_used_percentage = best_solution_volume * 100 / total_pallet_volume;
		}

		// 공간을 100% 다 활용했다면 끝냄
		if (hundred_percent)
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

	for (x = 1; x <= total_boxes; x++)
	{
		for (y = 1; y <= 3; y++)
		{
			// 기준 축으로부터 기준 크기 (key) 를 뽑아옴
			switch (y)
			{
			case 1:
				exdim = box_list[x].width;
				dimen2 = box_list[x].height;
				dimen3 = box_list[x].length;
				break;
			case 2:
				exdim = box_list[x].height;
				dimen2 = box_list[x].width;
				dimen3 = box_list[x].length;
				break;
			case 3:
				exdim = box_list[x].length;
				dimen2 = box_list[x].width;
				dimen3 = box_list[x].height;
				break;
			}

			if ((exdim > pallet_y) || (((dimen2 > pallet_x) || (dimen3 > pallet_z)) && ((dimen3 > pallet_x) || (dimen2 > pallet_z))))
			{
				// 박스가 화물칸보다 큰 경우
				continue;
			}
			
			// 기존 레이어와 중복되면 SKIP
			if (layer_list.count(exdim) != 0)
				continue;

			layereval = 0;

			// 모든 박스의 최소 크기 (각 박스의 x, y, z축 내 최소 크기) 를 구하여
			// 현재 레이어에 쌓는다 (layereval에 더한다)
			for (z = 1; z <= total_boxes; z++)
			{
				if (!(x == z))
				{
					dimdif = abs(exdim - box_list[z].width);
					if (abs(exdim - box_list[z].height) < dimdif)
					{
						dimdif = abs(exdim - box_list[z].height);
					}
					if (abs(exdim - box_list[z].length) < dimdif)
					{
						dimdif = abs(exdim - box_list[z].length);
					}
					layereval = layereval + dimdif;
				}
			}

			// 레이어 크기와 기준값을 기록함
			layer_list[exdim] = layereval;
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

	scrap_list.begin()->cumx = pallet_x;
	scrap_list.begin()->cumz = 0;

	while (1)
	{
		find_smallest_z();

		auto left = scrap_min_z; left--;
		auto right = scrap_min_z; right++;

		if (left == scrap_list.end() && right == scrap_list.end())
		{
			/////////////////////////////////////////////////////////
			// NO LEFT AND RIGHT
			/////////////////////////////////////////////////////////
			lenx = scrap_min_z->cumx;
			lpz = remainpz - scrap_min_z->cumz;
			
			find_box(lenx, layerthickness, remainpy, lpz, lpz);
			check_found();

			if (layer_done) break;
			if (evened)  continue;

			box_list[cboxi].cox = 0;
			box_list[cboxi].coy = packedy;
			box_list[cboxi].coz = scrap_min_z->cumz;
			
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

				scrap_min_z = scrap_list.insert(scrap_min_z, node);
			}
			volume_check();
		}
		else if (left == scrap_list.end())
		{
			////////////////////////////////////////////////////////
			// NO LEFT BUT RIGHT
			/////////////////////////////////////////////////////////
			lenx = scrap_min_z->cumx;
			lenz = right->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			check_found();

			if (layer_done) break;
			if (evened)  continue;

			left = scrap_min_z;		left--;
			right = scrap_min_z;	right++;

			box_list[cboxi].coy = packedy;
			box_list[cboxi].coz = scrap_min_z->cumz;

			if (cboxx == scrap_min_z->cumx)
			{
				box_list[cboxi].cox = 0;
				if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					// RIGHT IS THE NEW MIN_Z
					// ORDINARY MIN_Z WILL BE ERASED
					scrap_min_z = scrap_list.erase(scrap_min_z);
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else
			{
				box_list[cboxi].cox = scrap_min_z->cumx - cboxx;

				if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
				}
				else
				{
					// MIN_Z'S CUM_X:
					scrap_min_z->cumx -= cboxx;

					// THE NEW NODE IS ON THE LEFT SIDE OF ORDINARY MIN_Z
					struct Scrappad node = 
					{ 
						scrap_min_z->cumx, 
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(right, node);
				}
			}
			volume_check();
		}
		else if (right == scrap_list.end())
		{
			////////////////////////////////////////////////////////
			// NO RIGHT BUT LEFT
			/////////////////////////////////////////////////////////
			lenx = scrap_min_z->cumx - left->cumx;
			lenz = left->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;
			
			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			check_found();

			if (layer_done) break;
			if (evened)  continue;

			left = scrap_min_z; left--;
			right = scrap_min_z; right++;

			box_list[cboxi].coy = packedy;
			box_list[cboxi].coz = scrap_min_z->cumz;
			box_list[cboxi].cox = left->cumx;

			if (cboxx == scrap_min_z->cumx - left->cumx)
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					// MODIFY LEFT
					left->cumx = scrap_min_z->cumx;

					// ERASE MIN_Z
					scrap_list.erase(scrap_min_z);
					scrap_min_z = scrap_list.end();
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
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
					// CREATE A NEW NODE BETWEEN LEFT AND MIN_Z
					struct Scrappad node = 
					{
						left->cumx + cboxx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(scrap_min_z, node);
				}
			}
			volume_check();
		}
		else if (left->cumz == right->cumz)
		{
			////////////////////////////////////////////////////////
			// LEFT AND RIGHT ARE ALL EXIST
			/////////////////////////////////////////////////////////
			//*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***

			lenx = scrap_min_z->cumx - left->cumx;
			lenz = left->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;

			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			check_found();

			if (layer_done)	break;
			if (evened)	continue;

			left = scrap_min_z; left--;
			right = scrap_min_z; right++;

			box_list[cboxi].coy = packedy;
			box_list[cboxi].coz = scrap_min_z->cumz;
			
			if (cboxx == scrap_min_z->cumx - left->cumx)
			{
				box_list[cboxi].cox = left->cumx;
				if (scrap_min_z->cumz + cboxz == right->cumz)
				{
					left->cumx = right->cumx;
					auto right_of_right = right; right_of_right++;

					// ERASE MIN_Z
					scrap_list.erase(scrap_min_z, right_of_right);
					scrap_min_z = scrap_list.end();
				}
				else
				{
					scrap_min_z->cumz = scrap_min_z->cumz + cboxz;
				}
			}
			else if (left->cumx < pallet_x - scrap_min_z->cumx)
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					scrap_min_z->cumx = scrap_min_z->cumx - cboxx;
					box_list[cboxi].cox = scrap_min_z->cumx - cboxx;
				}
				else
				{
					box_list[cboxi].cox = left->cumx;

					// CREATE NODE BETWEEN LEFT
					struct Scrappad node =
					{
						left->cumx + cboxx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(scrap_min_z, node);
				}
			}
			else
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					left->cumx += cboxx;
					box_list[cboxi].cox = left->cumx;
				}
				else
				{
					box_list[cboxi].cox = scrap_min_z->cumx - cboxx;

					// CREATE BETWEEN MIN_Z AND RIGHT
					struct Scrappad node =
					{
						scrap_min_z->cumx,
						scrap_min_z->cumz + cboxz
					};
					scrap_list.insert(right, node);

					// MIN_Z'S CUM_X: 
					scrap_min_z->cumx -= cboxx;
				}
			}
			volume_check();
		}
		else
		{
			//*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***

			lenx = scrap_min_z->cumx - left->cumx;
			lenz = left->cumz - scrap_min_z->cumz;
			lpz = remainpz - scrap_min_z->cumz;
			
			find_box(lenx, layerthickness, remainpy, lenz, lpz);
			check_found();

			if (layer_done) break;
			if (evened)		continue;

			box_list[cboxi].coy = packedy;
			box_list[cboxi].coz = scrap_min_z->cumz;
			box_list[cboxi].cox = left->cumx;

			if (cboxx == scrap_min_z->cumx - left->cumx)
			{
				if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					left->cumx = scrap_min_z->cumx;

					// ERASE MIN_Z
					scrap_list.erase(scrap_min_z);
					scrap_min_z = scrap_list.end();
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
				else if (scrap_min_z->cumz + cboxz == left->cumz)
				{
					box_list[cboxi].cox = scrap_min_z->cumx - cboxx;
					scrap_min_z->cumx -= cboxx;
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
		if (box_list[x].is_packed) continue;
		for (y = 1; y  <= 3; y++)
		{
			switch (y)
			{
			case 1:
				exdim = box_list[x].width;
				dimen2 = box_list[x].height;
				dimen3 = box_list[x].length;
				break;
			case 2:
				exdim = box_list[x].height;
				dimen2 = box_list[x].width;
				dimen3 = box_list[x].length;
				break;
			case 3:
				exdim = box_list[x].length;
				dimen2 = box_list[x].width;
				dimen3 = box_list[x].height;
				break;
			}
			layereval = 0;
			if ((exdim <= thickness) && (((dimen2 <= pallet_x) && (dimen3 <= pallet_z)) || ((dimen3 <= pallet_x) && (dimen2 <= pallet_z))))
			{
				for (z = 1; z <= total_boxes; z++)
				{
					if (!(x == z) && !(box_list[z].is_packed))
					{
						dimdif = abs(exdim - box_list[z].width);
						if (abs(exdim - box_list[z].height) < dimdif)
						{
							dimdif = abs(exdim - box_list[z].height);
						}
						if (abs(exdim - box_list[z].length) < dimdif)
						{
							dimdif = abs(exdim - box_list[z].length);
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
		if (box_list[x].is_packed) 
			continue;

		if (x > total_boxes) return;
		analyze_box(hmx, hy, hmy, hz, hmz, box_list[x].width, box_list[x].height, box_list[x].length);
		if ((box_list[x].width == box_list[x].length) && (box_list[x].length == box_list[x].height)) continue;
		analyze_box(hmx, hy, hmy, hz, hmz, box_list[x].width, box_list[x].length, box_list[x].height);
		analyze_box(hmx, hy, hmy, hz, hmz, box_list[x].height, box_list[x].width, box_list[x].length);
		analyze_box(hmx, hy, hmy, hz, hmz, box_list[x].height, box_list[x].length, box_list[x].width);
		analyze_box(hmx, hy, hmy, hz, hmz, box_list[x].length, box_list[x].width, box_list[x].height);
		analyze_box(hmx, hy, hmy, hz, hmz, box_list[x].length, box_list[x].height, box_list[x].width);
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
	auto min_it = scrap_list.begin();

	for (auto it = min_it; it != scrap_list.end(); it++)
		if (it->cumz < min_it->cumz)
			it = min_it;

	this->scrap_min_z = min_it;
}

void Boxologic::check_found()
{
	evened = false;
	if (boxi)
	{
		cboxi = boxi;
		cboxx = boxx;
		cboxy = boxy;
		cboxz = boxz;
	}
	else
	{
		auto left = scrap_min_z;	left--;
		auto right = scrap_min_z;	right++;

		if ((bboxi > 0) && (layerinlayer || (left == scrap_list.end() && right == scrap_list.end())))
		{
			// ~ OR SCRAP_MIN_Z HAS NO NEIGHBOR

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
			if (left == scrap_list.end() && right == scrap_list.end())
			{
				// ~ OR SCRAP_MIN_Z HAS NO NEIGHBOR
				layer_done = true;
			}
			else
			{
				evened = true;

				if (left == scrap_list.end())
				{
					///////////////////////////////////////////
					// NO LEFT, BUT RIGHT
					///////////////////////////////////////////
					// THE RIGHT ITEM IS THE NEW SCRAP_MIN_Z
					// AND PREVIOUS WILL BE ERASED
					scrap_min_z = scrap_list.erase(scrap_min_z);
				}
				else if (right == scrap_list.end())
				{
					///////////////////////////////////////////
					// NO RIGHT, BUT LEFT
					///////////////////////////////////////////
					// ERASE CURRENT SCRAP_MIN_Z
					// THE LEFT ITEM FETCHES MIN'S CUM_X
					left->cumx = scrap_min_z->cumx;
					
					scrap_list.erase(scrap_min_z);
					scrap_min_z = scrap_list.end();
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
						// ERASE MIN AND ITS RIGHT
						auto right_of_right = right;
						right_of_right++;

						scrap_list.erase(scrap_min_z, right_of_right);
					}
					else
					{
						// ----------------------------------------
						// LEFT AND RIGHT'S CUM_Z ARE NOT EQUAL
						// ----------------------------------------
						if (left->cumz < right->cumz)
						{
							// IF LEFT'S CUM_Z IS LESS THAN RIGHT'S OWN
							left->cumx = scrap_min_z->cumx;
						}

						// ERASE MIN
						scrap_list.erase(scrap_min_z);
						scrap_min_z = scrap_list.end();
					}
				}
			}
		}
	}
	return;
}

void Boxologic::volume_check()
{
	box_list[cboxi].is_packed = true;
	box_list[cboxi].layout_width = cboxx;
	box_list[cboxi].layout_height = cboxy;
	box_list[cboxi].layout_length = cboxz;
	
	packedvolume = packedvolume + box_list[cboxi].vol;
	packednumbox++;
	
	if (best_packing)
	{
		// boxologic은 따로이 최적 방향을 저장하지 않으니
		// 별도의 핸들링이 필요하다
		write_boxlist_file();
	}
	else if (packedvolume == total_pallet_volume || packedvolume == total_box_volume)
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
	best_packing = true;

	packed_box_percentage = best_solution_volume * 100 / total_box_volume;
	pallet_volume_used_percentage = best_solution_volume * 100 / total_pallet_volume;
	
	list_candidate_layers();
	packedvolume = 0.0;
	packedy = 0;
	packing = true;

	layerthickness = best_layer;
	remainpy = pallet_y;
	remainpz = pallet_z;

	for (x = 1; x <= total_boxes; x++)
	{
		box_list[x].is_packed = false;
	}

	do
	{
		layerinlayer = 0;
		layer_done = false;
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
			layer_done = false;
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
		x = box_list[cboxi].cox;
		y = box_list[cboxi].coy;
		z = box_list[cboxi].coz;
		bx = box_list[cboxi].layout_width;
		by = box_list[cboxi].layout_height;
		bz = box_list[cboxi].layout_length;
		break;
	case 2:
		x = box_list[cboxi].coz;
		y = box_list[cboxi].coy;
		z = box_list[cboxi].cox;
		bx = box_list[cboxi].layout_length;
		by = box_list[cboxi].layout_height;
		bz = box_list[cboxi].layout_width;
		break;
	case 3:
		x = box_list[cboxi].coy;
		y = box_list[cboxi].coz;
		z = box_list[cboxi].cox;
		bx = box_list[cboxi].layout_height;
		by = box_list[cboxi].layout_length;
		bz = box_list[cboxi].layout_width;
		break;
	case 4:
		x = box_list[cboxi].coy;
		y = box_list[cboxi].cox;
		z = box_list[cboxi].coz;
		bx = box_list[cboxi].layout_height;
		by = box_list[cboxi].layout_width;
		bz = box_list[cboxi].layout_length;
		break;
	case 5:
		x = box_list[cboxi].cox;
		y = box_list[cboxi].coz;
		z = box_list[cboxi].coy;
		bx = box_list[cboxi].layout_width;
		by = box_list[cboxi].layout_length;
		bz = box_list[cboxi].layout_height;
		break;
	case 6:
		x = box_list[cboxi].coz;
		y = box_list[cboxi].cox;
		z = box_list[cboxi].coy;
		bx = box_list[cboxi].layout_length;
		by = box_list[cboxi].layout_width;
		bz = box_list[cboxi].layout_height;
		break;
	}

	box_list[cboxi].cox = x;
	box_list[cboxi].coy = y;
	box_list[cboxi].coz = z;
	box_list[cboxi].layout_width = bx;
	box_list[cboxi].layout_height = by;
	box_list[cboxi].layout_length = bz;
}