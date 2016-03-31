#pragma once
#include <boxologic/Instance.hpp>

namespace boxologic
{
	/**
	 * @brief A box, trying to pack into a Pallet.
	 *
	 * @author Bill Knechtel, <br>
	 *		   Migrated and Refactored by Jeongho Nam <http://samchon.org>
	 *
	 * @see bws::packer::Instance
	 * @see bws::packer::Wrap
	 */
	struct Box
		: public st_Instance
	{
	public:
		/**
		 * @brief Coordinate-X of the box placement in a Pallet.
		 */
		double cox;

		/**
		 * @brief Coordinate-Y of the box placement in a Pallet
		 */
		double coy;

		/**
		 * @brief Coordinate-Z of the box placement in a Pallet
		 */
		double coz;

		/**
		 * Whether the Box is packed into a Pallet.
		 */
		bool is_packed;
	};
};