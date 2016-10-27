#pragma once
#include <samchon/API.hpp>

#include <mutex>

namespace samchon
{
namespace library
{
	class SQLi;
	class SQLStatement;

namespace base
{
	class SQLiBase
	{
		friend class SQLi;
		friend class SQLStatement;

	private:
		/**
		 * @brief SQLStatement's pointer linked with the SQLi
		 */
		SQLStatement *stmt;

		/**
		 * @brief Mutex ensuring concurrency with SQLStatement
		 */
		std::mutex stmtMutex;
	};
};
};
};