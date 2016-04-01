#pragma once
#include <samchon/library/CriticalMap.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A Dictionary ensures concurrency.
	 *
	 * @see library::CriticalAllocator
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T, typename _Pr = std::less<std::string>>
	using CriticalDictionary = CriticalMap<std::string, T, _Pr>;
};
};