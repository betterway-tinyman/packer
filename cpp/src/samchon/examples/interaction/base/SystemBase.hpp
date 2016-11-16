#pragma once
#include <samchon/protocol/IProtocol.hpp>
#include <samchon/protocol/Entity.hpp>

namespace samchon
{
namespace examples
{
namespace interaction
{
namespace base
{
	class SystemBase
		: public virtual protocol::IProtocol,
		public virtual protocol::EntityBase
	{
	private:
		int uid {-1};

	public:
		auto getUID() const -> int
		{
			return uid;
		};

		void setUID(int val)
		{
			uid = val;
		};
	};
};
};
};
};