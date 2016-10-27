#pragma once
#include <samchon/protocol/EntityGroup.hpp>
#include <vector>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An EntityGroup with vector container and children capsuled in unique pointers.
	 *
	 * @tparam T A type of children Entity. Must be a class derived from an Entity or Entity itself.
	 *
	 * @details
	 * <p> For a case of inheritance of UniqueEntityArray and the inherited UniqueEntityArray also
	 * contains entity objects which are inherited from original child entity type, UniqueEntityArray
	 * supports macros changing returned type of accessor methods. </p>
	 *
	 *	\li UNIQUE_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER
	 *	\li UNIQUE_ENTITY_ARRAY_ELEMENT_ACCESSOR_BODY
	 *
	 * @note
	 * <p> std::unique_ptr doesn't allow copy construction. It allows only move construction. When inserts
	 * children objcets not by construct() method but by your hand, Be careful to insert deriving copy
	 * construction. You've use move constructor. </p>
	 *
	 *	\li <i> UniqueEntityArray<?>::push_back( move( std::unique_ptr<?>(new ?(...)) ) ); </i>
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T, typename Key = std::string>
	using UniqueEntityArray = EntityGroup<std::vector<std::unique_ptr<T>>, T, Key>;
};
};
