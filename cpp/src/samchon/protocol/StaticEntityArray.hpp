#pragma once
#include <samchon/protocol/StaticEntityGroup.hpp>
#include <vector>

namespace samchon
{
namespace protocol
{
	template <typename T, typename Key = std::string>
	using StaticEntityArray = StaticEntityGroup<std::vector<T>, Key>;
};
};