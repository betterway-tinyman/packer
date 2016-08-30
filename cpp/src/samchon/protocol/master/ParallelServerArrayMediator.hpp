#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArrayMediator.hpp>
#include <samchon/protocol/external/ExternalServerArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API ParallelServerArrayMediator
		: public ParallelSystemArrayMediator,
		public external::ExternalServerArray
	{
	public:
		ParallelServerArrayMediator();
		virtual ~ParallelServerArrayMediator();

		virtual void connect() override;
	};
};
};
};