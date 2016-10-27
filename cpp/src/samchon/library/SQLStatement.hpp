#pragma once
#include <samchon/API.hpp>

#ifdef _WIN32
#	ifndef WIN32_LEAN_AND_MEAN 
#		define WIN32_LEAN_AND_MEAN 
#	endif
#	include <Windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#define _SQLNCLI_ODBC_
#include <sqlncli.h>

#include <samchon/library/base/SQLiBase.hpp>

#include <vector>
#include <memory>
#include <string>

#include <samchon/ByteArray.hpp>
#include <samchon/HashMap.hpp>
#include <samchon/library/XML.hpp>

namespace samchon
{
namespace library
{
	class SQLi;
		
	/**
	 * @brief A sql statement
	 *
	 * @details
	 * <p> A SQLStatement instance is used to executing a SQL statement and returning the results 
	 * it produces against a SQL database that is opened through a SQLi instance. </p>
	 *
	 * <p> Through the SQLi, it's the reason why a principle of DBMS, DBMS system
	 * doesn't allow simultaneous query from a session (connection). Only a query (process) is allowed
	 * at a time. If you try to simultaneous query from a SQLi, ODBC throws error. To avoid the error,
	 * an execute of query from SQLStatement will lock a mutex of SQLi to ensure exclusiveness. </p>
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_data.png)
	 *
	 * @note
	 * <p> To ensure the exclusiveness, you've to make SQLStatement from SQLi. Do not make SQLStatement 
	 * by yourself. call SQLi::createSQLStatement() instead. Even you make a derived class from
	 * SQLStatement, don't make its constructor to have public accessor. </p>
	 *
	 * <p> Becuase execution of a sql statement causes lock on mutex in SQLi, you've to destruct the
	 * SQLStatement or call free() method(). If you don't, the mutex will not be unlocked,
	 * thus you can't do anything by the SQLi. </p>
	 *
	 * @handbook [Library - SQL Driver](https://github.com/samchon/framework/wiki/CPP-Library-SQL_Driver)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SQLStatement
	{
		friend class SQLi;
	private:
#ifdef _WIN64
		typedef long long SQL_SIZE_T;
#else
		typedef long SQL_SIZE_T;
#endif

	protected:
		/* --------------------------------------------------------------
			BASIC MEMBER VARIABLES
		-------------------------------------------------------------- */
		/**
		 * @brief SQLi who created the SQLStatement
		 */
		SQLi *sqli;

		/**
		 * @brief Handler of sql statement (OBDC)
		 */
		void *hstmt;

		/* --------------------------------------------------------------
			MEMBER VARIABLES FOR BINDING
		-------------------------------------------------------------- */
		/**
		 * @brief Count of binded parameters\n
		 */
		size_t bindParameterCount;

		/**
		 * @brief A map for binary size
		 *
		 * @details
		 * When calls ByteArray::size(), the returned size_t value can't be kept until
		 */
		HashMap<size_t, SQL_SIZE_T> bindParameterBASizeMap;

	protected:
		/**
		 * @brief Protected Constructor
		 *
		 * @details SQLStatement's constructor have to created by SQLi::createStatement().
		 *
		 * @note
		 * <p> Don't create SQLStatement by yourself. </p>
		 * <p> SQLStatement has to be created by SQLi::createStatement(). </p>
		 *
		 * @param sqli Parent SQLi who created the SQLStatement
		 */
		SQLStatement(SQLi *sqli)
		{
			this->sqli = sqli;
			bindParameterCount = 0;

			SQLAllocHandle(SQL_HANDLE_STMT, sqli->hdbc, &hstmt);
		};
		void reset(SQLi *sqli)
		{
			free();
			this->sqli = sqli;

			SQLAllocHandle(SQL_HANDLE_STMT, sqli->hdbc, &hstmt);
		};

	public:
		virtual ~SQLStatement()
		{
			free();
		};

	public:
		/**
		 * @brief Free the sql statement
		 */
		void free()
		{
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

			bindParameterCount = 0;

			if (sqli == nullptr || ((base::SQLiBase*)sqli)->stmt != this)
				return;

			((base::SQLiBase*)sqli)->stmt = nullptr;
			((base::SQLiBase*)sqli)->stmtMutex.unlock();
		};

		/**
		 * @brief Refresh the sql statement
		 */
		void refresh()
		{
			reset(this->sqli);
		};

		/* -----------------------------------------------------------------------
			QUERY
		----------------------------------------------------------------------- */
		/**
		 * @brief Prepare a sql statement
		 * @details Prepare a sql statement with parameters to bind for execution
		 *
		 * @warning Be careful for destructions of binded parameters
		 *
		 * @param sql A sql-statement to prepare
		 * @param ... args The parameters to bind
		 */
		template <typename T, typename ... _Args>
		void prepare(const std::string &sql, const T& val, const _Args& ... args)
		{
			prepare(sql);

			bindParameter(val);
			bindParameter(args...);
		};
		template <typename T> void prepare(const std::string &str, const T& val)
		{
			prepare(str);

			bindParameter(val);
		};
		void prepare(const std::string &sql)
		{
			refresh();

			((base::SQLiBase*)sqli)->stmtMutex.lock();
			((base::SQLiBase*)sqli)->stmt = this;

			SQLPrepareA(hstmt, (SQLCHAR*)&sql[0], SQL_NTS);
		};
		void prepare(const std::wstring &sql)
		{
			refresh();

			((base::SQLiBase*)sqli)->stmtMutex.lock();
			((base::SQLiBase*)sqli)->stmt = this;

			SQLPrepareW(hstmt, (SQLWCHAR*)&sql[0], SQL_NTS);
		};

		template <typename T, typename ... _Args>
		void prepare(const std::wstring &sql, const T& val, const _Args& ... args)
		{
			prepare(sql);

			bindParameter(val);
			bindParameter(args...);
		};
		template <typename T> void prepare(const std::wstring &str, const T& val)
		{
			prepare(str);

			bindParameter(val);
		};

		/**
		 * @brief Execute the prepared sql statement
		 *
		 * @throw exception Error message from DBMS
		 */
		void execute()
		{
			SQLRETURN res = SQLExecute(hstmt);

			if (res == SQL_ERROR)
				throw exception(sqli->getErrorMessage(SQL_HANDLE_STMT).c_str());

			// device or resource busy
			// it means network disconnection in almost case
		};

		/**
		 * @brief Execute sql-statement direclty
		 *
		 * @details
		 * Executes the given sql-statement without preparing or binding any parameter
		 *
		 * @warning
		 *	\li Cannot use if prepare statement has already defined. Use execute instead.
		 *  \li Not recommended when the case of dynamic sql and the sql-statement
		 *		is not for procedure but for direct access to table.
		 *		Use prepare and execute instead.
		 *
		 * @param sql sql-statement you want to execute
		 * @throw exception Error message from DBMS
		 * @throw exception Method prepare is already called
		 */
		void executeDirectly(const std::string &sql)
		{
			prepare(sql);
			execute();
		};
		void executeDirectly(const std::wstring &sql)
		{
			prepare(sql);
			execute();
		};

		/* -----------------------------------------------------------------------
			CURSOR
		----------------------------------------------------------------------- */
		/**
		 * @brief Fetch a record
		 *
		 * @return
		 * <p> Whether succeded to fetch a record. </p>
		 * <p> False means there's not any record or previous record was the last. </p>
		 */
		auto fetch() const -> bool
		{
			do
			{
				SQLSMALLINT colSize = 0;
				SQLNumResultCols(hstmt, &colSize);

				if (colSize > 0)
					break;
			} while (next() == true);

			return (SQLFetch(hstmt) == SQL_SUCCESS);
		};

		/**
		 * @brief Move cursor to the next sql-statement
		 *
		 * @return Whether succeded to move cursor to the next statement.
		 */
		auto next() const -> bool
		{
			return (SQLMoreResults(hstmt) != SQL_NO_DATA);
		};

		/* -------------------------------------------------------------------------
			GET DATA IN A COLUMN
		------------------------------------------------------------------------- */
		/**
		 * @brief Get size of columns
		 *
		 * @details Returns the number of columns in a result set.
		 * @warning Not size of rows.
		 */
		auto size() const -> size_t
		{
			SQLSMALLINT val = 0;

			SQLNumResultCols(hstmt, &val);
			return (size_t)val;
		};

		/**
		 * @brief Get column's data by its index
		 * @details Returns column's data from fetched-record by specified column index
		 *
		 * @param index Index number of a column wants to get
		 * @return Data stored in the record at the position of specifield column
		 */
		template <typename T> auto at(size_t index) const -> T
		{
			T val;
			sql_get_data(index + 1, C_TYPE(T()), &val);

			return val;
		};
		template<> auto at(size_t index) const -> std::string
		{
			index++;

			std::string str(1, NULL);
			SQLLEN size = 0;

			if (::SQLGetData(hstmt, (SQLUSMALLINT)index, SQL_C_CHAR, &str[0], 0, &size) != SQL_SUCCESS && size != 0)
			{
				// IMPORTANT, FOR NULL-TERMINATED-STRING
				size++;

				str.assign(size, NULL);
				::SQLGetData(hstmt, (SQLUSMALLINT)index, SQL_C_CHAR, &str[0], sizeof(char)*size, NULL);
			}

			// ERASE ZERO-CHARS
			long long i;
			for (i = str.size() - 1; i >= 0; i--)
				if (str[i] != NULL)
					break;

			if (i != str.size() - 1)
				str.erase(str.begin() + i + 1, str.end());

			return str;
		}
		template<> auto at(size_t index) const -> std::wstring
		{
			index++;

			std::wstring str(1, NULL);
			SQLLEN size = 0;

			if (::SQLGetData(hstmt, (SQLUSMALLINT)index, SQL_C_WCHAR, &str[0], 0, &size) != SQL_SUCCESS && size != 0)
			{
				// IMPORTANT, FOR NULL-TERMINATED-STRING
				size++;

				str.assign((size_t)size, NULL);
				::SQLGetData(hstmt, (SQLUSMALLINT)index, SQL_C_WCHAR, &str[0], sizeof(wchar_t)*size, NULL);
			}

			// ERASE ZERO-CHARS
			long long i;
			for (i = str.size() - 1; i >= 0; i--)
				if (str[i] != NULL)
					break;

			if (i != str.size() - 1)
				str.erase(str.begin() + i + 1, str.end());

			return str;
		};
		template<> auto at(size_t index) const -> ByteArray
		{
			index++;

			ByteArray data;
			SQLLEN size = 0;

			if (::SQLGetData(hstmt, (SQLUSMALLINT)index, SQL_C_BINARY, &data[0], 0, &size) != SQL_SUCCESS && size != 0)
			{
				data.assign(size, NULL);
				::SQLGetData(hstmt, (SQLUSMALLINT)index, SQL_C_BINARY, &data[0], data.size(), NULL);
			}
			return data;
		};

		/**
		 * @brief Get a column data by its name
		 * @details Returns column's data from fetchched-recrod by specified column name
		 *
		 * @param name Name of a column wants to get
		 * @return Data stored in the record at the position of specifield column
		 */
		/*template <typename T> auto get(const std::string &) const -> T
		{
			return this->at<T>(0);
		};*/

		/**
		 * @brief Result sets to XML
		 *
		 * @details
		 * <p> Converts the records of current sql-statement to XML. </p>
		 * <p> Recommends to override for each DBMS's domain XML rule. </p>
		 *
		 * @return XML representing records of the statement
		 */
		//virtual auto toXML() const -> std::shared_ptr<XML>;

	private:
		/* -------------------------------------------------------------------
			BIND
		------------------------------------------------------------------- */
		template <typename T, typename ... _Args>
		void bindParameter(const T& val, const _Args& ... args)
		{
			bindParameter(val);
			bindParameter(args...);
		};
		template <typename T> void bindParameter(const T &val)
		{
			sql_bind_parameter(C_TYPE(val), SQL_TYPE(val), (void*)&val);
		};
		template<> void bindParameter(const std::string &val)
		{
			::SQLBindParameter(hstmt, (SQLUSMALLINT)++bindParameterCount, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, val.size(), 0, (SQLPOINTER)val.data(), val.size(), NULL);
		};
		template<> void bindParameter(const std::wstring &val)
		{
			::SQLBindParameter(hstmt, (SQLUSMALLINT)++bindParameterCount, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, val.size(), 0, (SQLPOINTER)val.data(), val.size(), NULL);
		};
		template<> void bindParameter(const ByteArray &val)
		{
			bindParameterBASizeMap.set(++bindParameterCount, (SQL_SIZE_T)val.size());

			::SQLBindParameter(hstmt, (SQLUSMALLINT)bindParameterCount, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, val.size(), 0, (unsigned char*)&val[0], 0, &bindParameterBASizeMap.get(bindParameterCount));
		};

		/* -------------------------------------------------------------------
			ODBC'S FUNCTION
		------------------------------------------------------------------- */
		void sql_get_data(size_t index, short type, void *listener) const
		{
			::SQLGetData(hstmt, (SQLUSMALLINT)index, type, listener, 0, nullptr);
		};
		void sql_bind_parameter(short cppType, short sqlType, void *val)
		{
			::SQLBindParameter(hstmt, (SQLUSMALLINT)++bindParameterCount, SQL_PARAM_INPUT, cppType, sqlType, 0, 0, val, 0, nullptr);
		};

		template <typename T> auto C_TYPE(const T &) const -> short;
		template<> auto C_TYPE(const bool &) const -> short
		{
			return SQL_C_BIT;
		}
		template<> auto C_TYPE(const char &) const -> short
		{
			return SQL_CHAR;
		}
		template<> auto C_TYPE(const short &) const -> short
		{
			return SQL_C_SSHORT;
		}
		template<> auto C_TYPE(const long &) const -> short
		{
			return SQL_C_SLONG;
		}
		template<> auto C_TYPE(const long long &) const -> short
		{
			return SQL_C_SBIGINT;
		}
		template<> auto C_TYPE(const int &) const -> short
		{
			return SQL_C_SLONG;
		}
		template<> auto C_TYPE(const float &) const -> short
		{
			return SQL_C_FLOAT;
		}
		template<> auto C_TYPE(const double &) const -> short
		{
			return SQL_C_DOUBLE;
		}

		template<> auto C_TYPE(const unsigned char &) const -> short
		{
			return SQL_C_BINARY;
		}
		template<> auto C_TYPE(const unsigned short &) const -> short
		{
			return SQL_C_USHORT;
		}
		template<> auto C_TYPE(const unsigned long &) const -> short
		{
			return SQL_C_ULONG;
		}
		template<> auto C_TYPE(const unsigned long long &) const -> short
		{
			return SQL_C_UBIGINT;
		}
		template<> auto C_TYPE(const unsigned int &) const -> short
		{
			return SQL_C_UBIGINT;
		}
		template<> auto C_TYPE(const long double &) const -> short
		{
			return SQL_C_DOUBLE;
		}

		template<> auto C_TYPE(const string &) const -> short
		{
			return SQL_C_CHAR;
		}
		template<> auto C_TYPE(const wstring &) const -> short
		{
			return SQL_C_WCHAR;
		}
		template<> auto C_TYPE(const ByteArray &) const -> short
		{
			return SQL_C_BINARY;
		}

		template <typename T> auto SQL_TYPE(const T &) const -> short;
		template<> auto SQL_TYPE(const bool &) const -> short
		{
			return SQL_BIT;
		}
		template<> auto SQL_TYPE(const char &) const -> short
		{
			return SQL_CHAR;
		}
		template<> auto SQL_TYPE(const short &) const -> short
		{
			return SQL_TINYINT;
		}
		template<> auto SQL_TYPE(const long &) const -> short
		{
			return SQL_INTEGER;
		}
		template<> auto SQL_TYPE(const long long &) const -> short
		{
			return SQL_BIGINT;
		}
		template<> auto SQL_TYPE(const int &) const -> short
		{
			return SQL_INTEGER;
		}
		template<> auto SQL_TYPE(const float &) const -> short
		{
			return SQL_FLOAT;
		}
		template<> auto SQL_TYPE(const double &) const -> short
		{
			return SQL_DOUBLE;
		}

		template<> auto SQL_TYPE(const unsigned char &) const -> short
		{
			return SQL_BINARY;
		}
		template<> auto SQL_TYPE(const unsigned short &) const -> short
		{
			return SQL_TINYINT;
		}
		template<> auto SQL_TYPE(const unsigned long &) const -> short
		{
			return SQL_INTEGER;
		}
		template<> auto SQL_TYPE(const unsigned long long &) const -> short
		{
			return SQL_BIGINT;
		}
		template<> auto SQL_TYPE(const unsigned int &) const -> short
		{
			return SQL_INTEGER;
		}
		template<> auto SQL_TYPE(const long double &) const -> short
		{
			return SQL_DOUBLE;
		}

		template<> auto SQL_TYPE(const string &) const -> short
		{
			return SQL_CHAR;
		}
		template<> auto SQL_TYPE(const wstring &) const -> short
		{
			return SQL_WCHAR;
		}
		template<> auto SQL_TYPE(const ByteArray &) const -> short
		{
			return SQL_BINARY;
		}
	};
};
};