#pragma once

#include <samchon/Map.hpp>
#include <samchon/library/CriticalAllocator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A Map ensures concurrency.
	 *
	 * @see library::CriticalAllocator
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename _Kty, typename T, typename _Pr = std::less<_Kty>>
	using CriticalMap = Map<_Kty, T, _Pr, CriticalAllocator<std::pair<const std::string, T>>>;
};
};