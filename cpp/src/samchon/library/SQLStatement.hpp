#pragma once
#include <samchon/API.hpp>

#include <vector>
#include <memory>
#include <samchon/TreeMap.hpp>
#include <string>
#include <samchon/ByteArray.hpp>

namespace samchon
{
namespace library
{
	class SQLi;
	class XML;
		
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
	 * @image html cpp/subset/library_sql.png
	 * @image latex cpp/subset/library_sql.png
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
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API SQLStatement
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
		TreeMap<size_t, SQL_SIZE_T> bindParameterBASizeMap;

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
		SQLStatement(SQLi *sqli);
		void reset(SQLi *sqli);

	public:
		virtual ~SQLStatement();

	public:
		/**
		 * @brief Free the sql statement
		 */
		void free();

		/**
		 * @brief Refresh the sql statement
		 */
		void refresh();

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
		void prepare(const std::string &);
		void prepare(const std::wstring &);

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
		void execute();

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
		void executeDirectly(const std::string&);
		void executeDirectly(const std::wstring&);

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
		auto fetch() const -> bool;

		/**
		 * @brief Move cursor to the next sql-statement
		 *
		 * @return Whether succeded to move cursor to the next statement.
		 */
		auto next() const -> bool;

		/* -------------------------------------------------------------------------
			GET DATA IN A COLUMN
		------------------------------------------------------------------------- */
		/**
		 * @brief Get size of columns
		 *
		 * @details Returns the number of columns in a result set.
		 * @warning Not size of rows.
		 */
		auto size() const -> size_t;

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
			return _atAsString(index);
		}
		template<> auto at(size_t index) const -> std::wstring
		{
			return _atAsWString(index);
		};
		template<> auto at(size_t index) const -> ByteArray
		{
			return _atAsByteArray(index);
		};

		/**
		 * @brief Get a column data by its name
		 * @details Returns column's data from fetchched-recrod by specified column name
		 *
		 * @param name Name of a column wants to get
		 * @return Data stored in the record at the position of specifield column
		 */
		template <typename T> auto get(const std::string &) const -> T
		{
			return this->at<T>(0);
		};

		/**
		 * @brief Result sets to XML
		 *
		 * @details
		 * <p> Converts the records of current sql-statement to XML. </p>
		 * <p> Recommends to override for each DBMS's domain XML rule. </p>
		 *
		 * @return XML representing records of the statement
		 */
		virtual auto toXML() const->std::shared_ptr<XML>;

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
		template<> void bindParameter(const std::string &val);
		template<> void bindParameter(const std::wstring &val);
		template<> void bindParameter(const ByteArray &val);

		/* -------------------------------------------------------------------
			ODBC'S FUNCTION
		------------------------------------------------------------------- */
		void sql_get_data(size_t, short, void*) const;
		void sql_bind_parameter(short, short, void*);

		template <typename T> auto C_TYPE(const T &) const -> short;
		template<> auto C_TYPE(const bool &) const -> short;
		template<> auto C_TYPE(const char &) const -> short;
		template<> auto C_TYPE(const short &) const -> short;
		template<> auto C_TYPE(const long &) const -> short;
		template<> auto C_TYPE(const long long &) const -> short;
		template<> auto C_TYPE(const int &) const -> short;
		template<> auto C_TYPE(const float &) const -> short;
		template<> auto C_TYPE(const double &) const -> short;

		template<> auto C_TYPE(const unsigned char &) const -> short;
		template<> auto C_TYPE(const unsigned short &) const -> short;
		template<> auto C_TYPE(const unsigned long &) const -> short;
		template<> auto C_TYPE(const unsigned long long &) const -> short;
		template<> auto C_TYPE(const unsigned int &) const -> short;
		template<> auto C_TYPE(const long double &) const -> short;

		template<> auto C_TYPE(const std::string &) const -> short;
		template<> auto C_TYPE(const std::wstring &) const -> short;
		template<> auto C_TYPE(const ByteArray &) const -> short;

		template <typename T> auto SQL_TYPE(const T &) const -> short;
		template<> auto SQL_TYPE(const bool &) const -> short;
		template<> auto SQL_TYPE(const char &) const -> short;
		template<> auto SQL_TYPE(const short &) const -> short;
		template<> auto SQL_TYPE(const long &) const -> short;
		template<> auto SQL_TYPE(const long long &) const -> short;
		template<> auto SQL_TYPE(const int &) const -> short;
		template<> auto SQL_TYPE(const float &) const -> short;
		template<> auto SQL_TYPE(const double &) const -> short;

		template<> auto SQL_TYPE(const unsigned char &) const -> short;
		template<> auto SQL_TYPE(const unsigned short &) const -> short;
		template<> auto SQL_TYPE(const unsigned long &) const -> short;
		template<> auto SQL_TYPE(const unsigned long long &) const -> short;
		template<> auto SQL_TYPE(const unsigned int &) const -> short;
		template<> auto SQL_TYPE(const long double &) const -> short;

		template<> auto SQL_TYPE(const std::string &) const -> short;
		template<> auto SQL_TYPE(const std::wstring &) const -> short;
		template<> auto SQL_TYPE(const ByteArray &) const -> short;

	private:
		/* -------------------------------------------------------------------
			HIDDEN TEMPLATE HELPERS
		------------------------------------------------------------------- */
		auto _atAsString(size_t index) const -> std::string;
		auto _atAsWString(size_t index) const -> std::wstring;

		auto _atAsByteArray(size_t index) const -> ByteArray;
	};
};
};