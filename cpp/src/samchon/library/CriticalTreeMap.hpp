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
	template <typename Key, typename T, typename _Pr = std::less<Key>>
	using CriticalTreeMap = TreeMap<Key, T, _Pr, CriticalAllocator<std::pair<const Key, T>>>;
};
};