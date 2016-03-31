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
	 * @author Jeongho Nam
	 */
	template <typename _Ty>
	using CriticalVector = std::vector<_Ty, CriticalAllocator<_Ty>>;
};
};