#pragma once
#include <samchon/Map.hpp>
#include <string>

namespace samchon
{
	/**
	 * @brief A Map key is string.
	 *
	 * @see library::Map
	 * @author Jeongho Nam
	 */
	template <typename _Ty, typename _Pr = std::less<std::string>, typename _Alloc = std::allocator<std::pair<const std::string, _Ty>>>
	using Dictionary = Map<std::string, _Ty, _Pr, _Alloc>;
};