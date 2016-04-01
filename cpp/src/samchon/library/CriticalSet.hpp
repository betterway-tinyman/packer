#pragma once

#include <set>
#include <samchon/library/CriticalAllocator.hpp>

namespace samchon
{
	namespace library
	{
		///**
		// * @brief A Set ensures concurrency.
		// *
		// * @see library::CriticalAllocator
		// * @see samchon::library
		// * @author Jeongho Nam <http://samchon.org>
		// */
		template <typename T, typename _Pr = std::less<T>>
		using CriticalSet = std::set<T, _Pr, CriticalAllocator<T>>;
	};
};