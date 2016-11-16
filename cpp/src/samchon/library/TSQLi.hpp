#pragma once
#include <samchon/API.hpp>

#include <samchon/library/SQLi.hpp>

namespace samchon
{
namespace library
{
	class SQLStatement;

	/**
	 * @brief A SQL interface for T-SQL
	 *
	 * @details
	 * \par [Inherited]
	 * @copydetails library::SQLi
	 */
	class TSQLi
		: public SQLi
	{
	private:
		typedef SQLi super;

	public:
		/**
		 * @brief Construct from port
		 *
		 * @param port Port number of T-SQL. default is 1433.
		 */
		TSQLi(int port = 1433)
			: super("{SQL Server}", port)
		{
		};
		virtual ~TSQLi() = default;

		/**
		 * @brief Factory method for creating T-SQL statement
		 *
		 * @details
		 * <p> To realize domain function in T-SQL, 'FOR XML' statement. </p>
		 *
		 * \par [Inherited]
		 * @coypdoc SQLi::createStatement()
		 * 
		 * @return A T-SQL statement
		 */
		/*virtual auto createStatement() -> std::shared_ptr<SQLStatement>
		{
			return std::make_shared<TSQLStatement>(this);
		};*/
	};
};
};