#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArrayMediator.hpp>
#include <samchon/protocol/external/ExternalServerClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class SAMCHON_FRAMEWORK_API ParallelServerClientArrayMediator
		: public ParallelSystemArrayMediator,
		public external::ExternalServerClientArray
	{
	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		ParallelServerClientArrayMediator();

		virtual ~ParallelServerClientArrayMediator();

		/* ---------------------------------------------------------
			OPEN & CONNECT
		--------------------------------------------------------- */
		virtual void open(int port) override;

		virtual void connect() override;
	};
};
};
};