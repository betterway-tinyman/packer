#pragma once
#include <samchon/API.hpp>

#include <vector>
#include <memory>
#include <samchon/library/RWMutex.hpp>

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
	private:
		library::RWMutex mtx;

	public:
		virtual auto _Get_children() const -> std::vector<std::shared_ptr<ExternalSystem>> = 0;

		auto getMutex() -> library::RWMutex& { return mtx; };
		auto getMutex() const -> const library::RWMutex& { return mtx; };
	};
};
};
};
};