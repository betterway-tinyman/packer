#pragma once
#include <samchon/API.hpp>

#include <samchon/library/SQLi.hpp>

namespace samchon
{
namespace library
{
	class SQLStatement;

	class SAMCHON_FRAMEWORK_API SQLLiteConnector
		: public SQLi
	{
	public:
		SQLLiteConnector();
		virtual ~SQLLiteConnector();
	};
};
};