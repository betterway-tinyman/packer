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
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T>
	using CriticalList = std::list<T, CriticalAllocator<T>>;
};
};