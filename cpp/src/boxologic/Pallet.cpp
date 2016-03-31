#include <boxologic/Pallet.hpp>

using namespace boxologic;

void Pallet::set_orientation(int orientation)
{
	switch (orientation)
	{
	case 1:
		layout_width = width;
		layout_height = height;
		layout_length = length;
		break;
	case 2:
		layout_width = length;
		layout_height = height;
		layout_length = width;
		break;
	case 3:
		layout_width = length;
		layout_height = width;
		layout_length = height;
		break;
	case 4:
		layout_width = height;
		layout_height = width;
		layout_length = length;
		break;
	case 5:
		layout_width = width;
		layout_height = length;
		layout_length = height;
		break;
	case 6:
		layout_width = height;
		layout_height = length;
		layout_length = width;
		break;
	}
}