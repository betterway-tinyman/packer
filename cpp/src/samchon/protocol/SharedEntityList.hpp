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
	 * @tparam T A type of children Entity. Must be a class derived from an Entity or Entity itself.
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T = Entity>
	using SharedEntityList = EntityGroup<std::list<std::shared_ptr<T>>, T>;
};
};