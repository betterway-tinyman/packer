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
	 * @tparam _Ty A type of children Entity. Must be a class derived from an Entity or Entity itself.
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
	template <typename _Ty = Entity>
	using UniqueEntityArray =
		EntityGroup
		<
			std::vector<std::unique_ptr<_Ty>>,
			_Ty, std::unique_ptr<_Ty>
		>;
};
};

/* ------------------------------------------------------------------------------
	MACROS
------------------------------------------------------------------------------ */
//HEADER
#define UNIQUE_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(CHILD_TYPE) \
auto operator[](size_t) const -> CHILD_TYPE*; \
auto at(size_t) const -> CHILD_TYPE*; \
auto get(const samchon::std::string&) const -> CHILD_TYPE*;

//BODY
#define UNIQUE_ENTITY_ARRAY_ELEMENT_ACCESSOR_BODY(THIS_TYPE, CHILD_TYPE) \
auto THIS_TYPE::operator[](size_t x) const -> CHILD_TYPE* { return dynamic_cast<CHILD_TYPE*>(container_type::operator[](x).get()); }; \
auto THIS_TYPE::at(size_t x) const -> CHILD_TYPE* { return dynamic_cast<CHILD_TYPE*>(container_type::at(x).get()); }; \
auto THIS_TYPE::get(const samchon::std::string &key) const -> CHILD_TYPE* { return dynamic_cast<CHILD_TYPE*>(samchon::protocol::EntityGroup<container_type, entity_type, value_type>::get(key).get()); }