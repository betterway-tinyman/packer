#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	class SAMCHON_FRAMEWORK_API SlaveSystem
		: public virtual external::ExternalSystem
	{
	private:
		typedef external::ExternalSystem super;

	public:
		SlaveSystem();
		virtual ~SlaveSystem();

		virtual void replyData(std::shared_ptr<Invoke>) override;
	};
};
};
};