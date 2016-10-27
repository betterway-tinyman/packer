#pragma once
#include <samchon/API.hpp>

#include <string>

namespace samchon
{
namespace templates
{
namespace external
{
	class ExternalSystem;

namespace base
{
	class ExternalServerBase
	{
		friend class ExternalSystem;

	protected:
		/**
		 * IP address of target external system to connect.
		 */
		std::string ip;
		
		/**
		 * Port number of target external system to connect.
		 */
		int port;
	};
};
};
};
};