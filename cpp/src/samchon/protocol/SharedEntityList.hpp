#pragma once

#include <samchon/protocol/EntityGroup.hpp>
#include <list>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An EntityGroup with list container and children capsuled in shared pointers.
	 *
	 * @tparam _Ty A type of children Entity. Must be a class derived from an Entity or Entity itself.
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam
	 */
	template <typename _Ty = Entity>
	using SharedEntityList = 
		EntityGroup
		<
			std::list<std::shared_ptr<_Ty>>, 
			_Ty, std::shared_ptr<_Ty>
		>;
};
};