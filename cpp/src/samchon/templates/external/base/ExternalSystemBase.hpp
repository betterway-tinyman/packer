#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/base/ExternalSystemArrayBase.hpp>

namespace samchon
{
namespace templates
{
namespace external
{
namespace base
{
	class ExternalSystemBase
	{
	protected:
		ExternalSystemArrayBase *system_array_;

	public:
		/**
		 * Get parent {@link ExternalSystemArray} object.
		 */
		template <typename SystemArray>
		auto getSystemArray() const -> SystemArray*
		{
			return (SystemArray*)system_array_;
		};
	};
};
};
};
};