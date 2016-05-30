#pragma once
#include <samchon/TreeMap.hpp>
#include <string>

namespace samchon
{
	/**
	 * @brief A TreeMap key is string.
	 *
	 * @see library::TreeMap
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T, typename _Pr = std::less<std::string>, typename _Alloc = std::allocator<std::pair<const std::string, T>>>
	using Dictionary = TreeMap<std::string, T, _Pr, _Alloc>;
};