#pragma once

#include <samchon/HashMap.hpp>
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
	using CriticalHashMap = HashMap<Key, T, std::hash<Key>, std::equal_to<Key>, CriticalAllocator<std::pair<const Key, T>>>;
};
};