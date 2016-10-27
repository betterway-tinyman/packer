#pragma once
#include <samchon/protocol/EntityGroup.hpp>
#include <deque>

namespace samchon
{
namespace protocol
{
	template <typename T, typename Key = std::string>
	using SharedEntityDeque = EntityGroup<std::deque<std::shared_ptr<T>>, T, Key>;
};
};

/* ------------------------------------------------------------------------------
	MACROS
------------------------------------------------------------------------------ */
// INLINE
#define SHARED_ENTITY_DEQUE_ELEMENT_ACCESSOR_INLINE(CHILD_TYPE) SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_INLINE(CHILD_TYPE)
#define SHARED_ENTITY_DEQUE_ELEMENT_ACCESSOR_HEADER(CHILD_TYPE) SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(CHILD_TYPE)
#define SHARED_ENTITY_DEQUE_ELEMENT_ACCESSOR_BODY(THIS_TYPE, CHILD_TYPE) SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_BODY(THIS_TYPE, CHILD_TYPE)