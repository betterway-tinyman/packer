#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/external/ExternalSystemArray.hpp>
#	include <samchon/protocol/external/ExternalServer.hpp>

namespace samchon
{
namespace protocol
{
namespace external
{
	class SAMCHON_FRAMEWORK_API ExternalServerArray
		: public virtual ExternalSystemArray
	{
	public:
		ExternalServerArray();
		virtual ~ExternalServerArray();

		virtual void connect();
	};
};
};
};