#pragma once
#include <samchon/protocol/StaticEntityGroup.hpp>
#include <list>

namespace samchon
{
namespace protocol
{
	template <typename T, typename Key = std::string>
	using StaticEntityList = StaticEntityGroup<std::list<T>, Key>;
};
};