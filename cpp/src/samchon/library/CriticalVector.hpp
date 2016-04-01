#pragma once

#include <vector>
#include <samchon/library/CriticalAllocator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A std::vector ensures concurrency.
	 *
	 * @see library::CriticalAllocator
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T>
	using CriticalVector = std::vector<T, CriticalAllocator<T>>;
};
};