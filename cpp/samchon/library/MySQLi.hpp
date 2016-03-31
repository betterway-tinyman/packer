#pragma once
#include <samchon/API.hpp>

#include <samchon/library/SQLi.hpp>

namespace samchon
{
namespace library
{
	class SQLStatement;

	/**
	 * @brief A SQL interface for My-SQL
	 *
	 * @copydetails library::SQLi
	 */
	class SAMCHON_FRAMEWORK_API MySQLi
		: public SQLi
	{
	private:
		typedef SQLi super;

	public:
		/**
		 * @brief Construct from port
		 *
		 * @param port Port number of My-SQL. default is 3306.
		 */
		MySQLi(int port = 3306);
		virtual ~MySQLi();
	};
};
};