#pragma once
#include <samchon/protocol/StaticEntityGroup.hpp>
#include <deque>

namespace samchon
{
namespace protocol
{
	template <typename T, typename Key = std::string>
	using StaticEntityDeque = StaticEntityGroup<std::deque<T>, Key>;
};
};