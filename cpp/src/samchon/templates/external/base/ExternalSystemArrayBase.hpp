#pragma once
#include <samchon/API.hpp>

#include <vector>
#include <memory>

namespace samchon
{
namespace templates
{
namespace external
{
	class ExternalSystem;

namespace base
{
	class ExternalSystemArrayBase
	{
	public:
		virtual auto _Get_children() const -> std::vector<std::shared_ptr<ExternalSystem>> = 0;
	};
};
};
};
};