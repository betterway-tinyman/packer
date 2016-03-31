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
		// * @author Jeongho Nam
		// */
		template <typename _Ty, typename _Pr = std::less<_Ty>>
		using CriticalSet = std::set<_Ty, _Pr, CriticalAllocator<_Ty>>;
	};
};