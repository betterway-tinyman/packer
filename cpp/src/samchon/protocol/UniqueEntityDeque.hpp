#pragma once
#include <samchon/protocol/EntityGroup.hpp>
#include <deque>

namespace samchon
{
namespace protocol
{
	template <typename T, typename Key = std::string>
	using UniqueEntityDeque = EntityGroup<std::deque<std::unique_ptr<T>>, T, Key>;
};
};
