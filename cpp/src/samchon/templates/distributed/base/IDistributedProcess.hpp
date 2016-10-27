#pragma once
#include <samchon/API.hpp>

#include <memory>

namespace samchon
{
namespace protocol
{
	class Invoke;
};

namespace templates
{
namespace distributed
{
	class DistributedSystem;

namespace base
{
	class IDistributedProcess
	{
	public:
		virtual auto sendData(std::shared_ptr<protocol::Invoke>, double) -> std::shared_ptr<DistributedSystem> = 0;

		virtual void replyData(std::shared_ptr<protocol::Invoke>) = 0;
	};
};
};
};
};