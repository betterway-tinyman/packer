#pragma once

#include <list>
#include <samchon/library/CriticalAllocator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A std::list ensures concurrency.
	 *
	 * @see library::CriticalAllocator
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	template <typename _Ty>
	using CriticalList = std::list<_Ty, CriticalAllocator<_Ty>>;
};
};