#pragma once

#include <samchon/TreeMap.hpp>
#include <samchon/library/CriticalAllocator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A TreeMap ensures concurrency.
	 *
	 * @see library::CriticalAllocator
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename _Kty, typename T, typename _Pr = std::less<_Kty>>
	using CriticalMap = TreeMap<_Kty, T, _Pr, CriticalAllocator<std::pair<const std::string, T>>>;
};
};