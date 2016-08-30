#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace slave
{
	class SAMCHON_FRAMEWORK_API ParallelSystem
		: public SlaveSystem
	{
	private:
		typedef SlaveSystem super;

	public:
		ParallelSystem();
		virtual ~ParallelSystem();

		virtual void replyData(std::shared_ptr<Invoke>) override;

	protected:
		virtual void replyPieceData(std::shared_ptr<Invoke>, size_t, size_t) = 0;
	};
};
};
};