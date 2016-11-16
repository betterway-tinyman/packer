#pragma once
#include <samchon/API.hpp>

#include <samchon/library/base/SQLiBase.hpp>

#include <samchon/library/SQLStatement.hpp>

#include <memory>
#include <samchon/library/StringUtil.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A SQL interface; DBMS connector
	 *
	 * @details
	 * <p> SQLi is an abstract class managing connection (session) with a specific database and manages
	 * SQL statements are executed and results are returned within the context of a connection. </p>
	 *
	 * <p> The reason why SQLStatement(s) are managed by SQLi is because of a principle of DBMS, 
	 * DBMS system doesn't allow simultaneous query from a session (connection). Only a query (process0) 
	 * is allowed at a time. If you try to simultaneous query from a SQLi, ODBC throws error. To avoid 
	 * the error, an execute of query from SQLStatement will lock a mutex of SQLi to ensure 
	 * exclusiveness. </p>
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_data.png)
	 *
	 * @note
	 * <p> To ensure the exclusiveness, you've to make SQLStatement from SQLi. Do not make SQLStatement 
	 * by yourself. call SQLi::createSQLStatement() instead. Even you make a derived class from
	 * SQLStatement, don't make its constructor to have public accessor. </p>
	 *
	 * <p> Becuase execution of a sql statement causes lock on mutex in SQLi, you've to destruct the
	 * SQLStatement or call SQLStatement::free() method(). If you don't, the mutex will not be unlocked,
	 * thus you can't do anything by the SQLi. </p>
	 * 
	 * @handbook [Library - SQL Driver](https://github.com/samchon/framework/wiki/CPP-Library-SQL_Driver)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SQLi : public base::SQLiBase
	{
		friend class SQLStatement;

	protected:
		/**
		 * @brief Driver; name of DBMS used for connection
		 *
		 * @details
		 *  \li MySQL: \"{MySQL Server}\"
		 *	\li MS-SQL: \"{SQL Server}\"
		 *	\li PL-SQL: \"{Oracle}\"
		 */
		std::string driver;

		/**
		 * @brief Port number of DBMS
		 *
		 * @details
		 *	\li MySQL: 3306
		 *	\li MS-SQL: 1433
		 *	\li PL-SQL: 1521
		 */
		int port;

	private:
		/**
		 * @brief Had connected to DBMS.
		 *
		 * @details 
		 * #connected is a flag variable for connection to DBMS. However, it doesn't mean the real-time
		 * status. #connected only stores whether a connection was succeeded or not.
		 */
		bool connected;

	public:
		/**
		 * @brief Construct from driver name and port
		 *
		 * @param driver Driver name of DBMS
		 * @param port Port number of DBMS
		 */
		SQLi(const std::string &driver, int port)
		{
			stmt = nullptr;
			connected = false;

			this->driver = driver;
			this->port = port;
		};

		virtual ~SQLi()
		{
			disconnect();
		};

		/**
		* @brief Factory method for creating SQLStatement
		*
		* @details
		* Recommend to create SQLStatement by this method.
		* Direct creation is not recommended as the reason of domain problem of each DBMS
		*
		* @return A SQLStatement matched for the domain SQLi
		*/
		virtual auto createStatement() -> std::shared_ptr<SQLStatement>
		{
			return std::shared_ptr<SQLStatement>(new SQLStatement(this));
		};

		/**
		 * @brief Connect to the DBMS\n
		 *
		 * @param ip An IP address of the DBMS
		 * @param db A name of database to access
		 * @param id Your own ID authorified from DBMS
		 * @param pwd A password for the ID
		 *
		 * @throw exception Error message from DBMS
		 */
		virtual void connect
			(
				const std::string &ip, const std::string &db,
				const std::string &id, const std::string &pwd
			)
		{
			std::unique_lock<std::mutex> uk(stmtMutex);
			SQLRETURN res;
			SQLHANDLE environment;

			if ((res = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environment)) == SQL_SUCCESS)
				if ((res = SQLSetEnvAttr(environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) == SQL_SUCCESS)
					if ((res = SQLAllocHandle(SQL_HANDLE_DBC, environment, &hdbc)) == SQL_SUCCESS)
					{
						SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)3, NULL);

						res = SQLDriverConnectA
							(
								hdbc, NULL,
								(SQLCHAR*)&StringUtil::substitute
								(
									"DRIVER={0};SERVER={1}, {2};DATABASE={3};UID={4};PWD={5};",
									driver, ip, port, db, id, pwd
								)[0],
								SQL_NTS, NULL, 1024, NULL, SQL_DRIVER_NOPROMPT
							);
					}

			SQLFreeHandle(SQL_HANDLE_DBC, environment);

			if (res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO)
			{
				disconnect();
				throw std::exception(getErrorMessage(SQL_HANDLE_DBC).c_str());
			}
			else
				connected = true;
		};

		/**
		 * @brief Disconnect from DBMS
		 */
		virtual void disconnect()
		{
			//FOR STATIC DESTRUCTION
			std::unique_lock<std::mutex> uk(stmtMutex);
			
			SQLDisconnect(hdbc);
			SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			connected = false;

			stmtMutex.unlock();
		};

		auto isConnected() const -> bool
		{
			if (connected == false)
				return false;

			unsigned int ret = SQL_CD_FALSE;
			SQLGetConnectAttr(hdbc, SQL_COPT_SS_CONNECTION_DEAD, &ret, SQL_IS_UINTEGER, NULL);

			return ret != SQL_CD_TRUE;
		};

	protected:
		/**
		 * @brief Get error message. 
		 * 
		 * @details Gets error message for throwing exception.
		 */
		virtual auto getErrorMessage(short type) const -> std::string
		{
			char state[1024];
			char message[1024];

			if (SQLGetDiagRecA(type, hdbc, 1, (SQLCHAR *)state, NULL, (SQLCHAR *)message, 1024, NULL) == SQL_SUCCESS)
				return message;
			else
				return "Unknown";
		};
	};
};
};