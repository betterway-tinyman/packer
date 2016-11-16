#pragma once
#include <samchon/API.hpp>

#include <mutex>

#ifdef _WIN32
#	ifndef WIN32_LEAN_AND_MEAN 
#		define WIN32_LEAN_AND_MEAN 
#	endif
#	include <Windows.h>
#endif

#define _SQLNCLI_ODBC_
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

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
		 * @brief Handler of DB-connector
		 */
		void *hdbc;

		/**
		 * @brief SQLStatement's pointer linked with the SQLi
		 */
		SQLStatement *stmt;

		/**
		 * @brief Mutex ensuring concurrency with SQLStatement
		 */
		std::mutex stmtMutex;

	protected:
		virtual auto getErrorMessage(short type) const -> std::string = 0;
	};
};
};
};