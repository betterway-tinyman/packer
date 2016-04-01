#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IEntityUniquePtrGroup.hpp>
#include <list>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An EntityGroup with list container and children capsuled in unique pointers.
	 *
	 * @tparam _Ty A type of children Entity. Must be a class derived from an Entity or Entity itself.
	 *
	 * @note
	 * <p> std::unique_ptr doesn't allow copy construction. It allows only move construction. When inserts
	 * children objcets not by construct() method but by your hand, Be careful to insert deriving copy
	 * construction. You've use move constructor. </p>
	 *
	 *	\li <i> UniqueEntityList<?>::push_back( move( std::unique_ptr<?>(new ?(...)) ) ); </i>
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename _Ty = Entity>
	using UniqueEntityList =
		EntityGroup
		<
			std::list<std::unique_ptr<_Ty>>,
			_Ty, std::unique_ptr<_Ty>
		>;
};
};