#pragma once
#include <boxologic\Instance.hpp>

namespace boxologic
{
	/**
	 * @brief A pallet containing boxes.
	 *
	 * @see bws::packer::Wrapper
	 *
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 *
	 */
	struct Pallet
		: public st_Instance
	{
	public:
		/**
		 * Set placement orientation.
		 */
		void set_orientation(int);
	};
};