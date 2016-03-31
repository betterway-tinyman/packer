#pragma once
#include <samchon/Map.hpp>
#include <string>

namespace samchon
{
	/**
	 * @brief A Map key is string.
	 *
	 * @see library::Map
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T, typename _Pr = std::less<std::string>, typename _Alloc = std::allocator<std::pair<const std::string, T>>>
	using Dictionary = Map<std::string, T, _Pr, _Alloc>;
};