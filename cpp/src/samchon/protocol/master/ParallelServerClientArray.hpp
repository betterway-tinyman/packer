#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArray.hpp>
#include <samchon/protocol/external/ExternalServerClientArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelServerClientArray
		: public ParallelSystemArray,
		public external::ExternalServerClientArray
	{
	public:
		ParallelServerClientArray()
			: ParallelSystemArray(),
			external::ExternalServerClientArray()
		{
		};

		virtual ~ParallelServerClientArray()
		{
		};
	};
};
};
};