#pragma once
#include <samchon/API.hpp>

#include <samchon/library/SQLStatement.hpp>

namespace samchon
{
namespace library
{
	class SQLi;

	/**
	 * @brief A t-sql statement
	 *
	 * @details
	 * <p> TSQLStatement is a SQLStatement for adjusting domain function f T-SQL, 'FOR XML' statement. </p>
	 *
	 * \par [Inherited]
	 * @copydetails library::SQLStatement
	 */
	class TSQLStatement
		: public SQLStatement
	{
		friend class TSQLi;

	private:
		typedef SQLStatement super;

	protected:
		TSQLStatement(SQLi *sqli)
			: super(sqli)
		{
		};

	public:
		virtual ~TSQLStatement();

		/**
		 * @brief Return an XML representing records.
		 *
		 * @details
		 * Get an XML object generated from a sql statement containing 'FOR XML'
		 *
		 * @return XML represents records
		 */
		virtual auto toXML() const -> std::shared_ptr<XML> override
		{
			fetch();
			std::string &str = super::at<std::string>(0);

			std::shared_ptr<XML> xml(new XML(str));
			return xml;
		};
	};
};
};