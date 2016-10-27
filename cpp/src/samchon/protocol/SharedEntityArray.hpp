#pragma once
#include <samchon/protocol/EntityGroup.hpp>
#include <vector>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An EntityGroup with vector container and children capsuled in shared pointers.
	 *
	 * @tparam T A type of children Entity. Must be a class derived from an Entity or Entity itself.
	 *
	 * @details
	 * <p> For a case of inheritance of SharedEntityArray and the inherited SharedEntityArray also
	 * contains entity objects which are inherited from original child entity type, SharedEntityArray
	 * supports macros changing returned type of accessor methods. </p>
	 *
	 *	\li SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER
	 *	\li SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_BODY
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T, typename Key = std::string>
	using SharedEntityArray = EntityGroup<std::vector<std::shared_ptr<T>>, T, Key>;
};
};

/* ------------------------------------------------------------------------------
	MACROS
------------------------------------------------------------------------------ */
// INLINE
#define SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_INLINE(CHILD_TYPE) \
auto operator[](size_t x) const -> std::shared_ptr<CHILD_TYPE> \
{ \
	return std::dynamic_pointer_cast<CHILD_TYPE>(container_type::operator[](x)); \
}; \
auto at(size_t x) const -> std::shared_ptr<CHILD_TYPE> \
{ \
	return std::dynamic_pointer_cast<CHILD_TYPE>(container_type::at(x)); \
}; \
auto get(const typename child_type::key_type &key) const -> std::shared_ptr<CHILD_TYPE> \
{ \
	return std::dynamic_pointer_cast<CHILD_TYPE>(samchon::protocol::EntityGroup<container_type, child_type, key_type>::get(key)); \
};

// HEADER
#define SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(CHILD_TYPE) \
auto operator[](size_t) const -> std::shared_ptr<CHILD_TYPE>; \
auto at(size_t) const -> std::shared_ptr<CHILD_TYPE>; \
auto get(const typename child_type::key_type&) const -> std::shared_ptr<CHILD_TYPE>;

// BODY
#define SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_BODY(THIS_TYPE, CHILD_TYPE) \
auto THIS_TYPE::operator[](size_t x) const -> std::shared_ptr<CHILD_TYPE> \
{ \
	return std::dynamic_pointer_cast<CHILD_TYPE>(container_type::operator[](x)); \
} \
auto THIS_TYPE::at(size_t x) const -> std::shared_ptr<CHILD_TYPE> \
{ \
	return std::dynamic_pointer_cast<CHILD_TYPE>(container_type::at(x)); \
} \
auto THIS_TYPE::get(const typename child_type::key_type &key) const -> std::shared_ptr<CHILD_TYPE> \
{ \
	return std::dynamic_pointer_cast<CHILD_TYPE>(samchon::protocol::EntityGroup<container_type, child_type, key_type>::get(key)); \
}