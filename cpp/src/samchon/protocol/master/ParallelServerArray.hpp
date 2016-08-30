#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArray.hpp>
#include <samchon/protocol/external/ExternalServerArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelServerArray
		: public ParallelSystemArray,
		public external::ExternalServerArray
	{
	public:
		ParallelServerArray()
			: ParallelSystemArray(),
			external::ExternalServerArray()
		{
		};

		virtual ~ParallelServerArray()
		{
		};
	};
};
};
};