#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArrayMediator.hpp>
#include <samchon/protocol/external/ExternalClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API ParallelClientArrayMediator
		: public ParallelSystemArrayMediator,
		public external::ExternalClientArray
	{
	public:
		ParallelClientArrayMediator();
		virtual ~ParallelClientArrayMediator();

		virtual void open(int port) override;
	};
};
};
};