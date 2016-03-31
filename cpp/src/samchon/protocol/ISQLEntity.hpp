#pragma once
#include <samchon/API.hpp>

#include <string>
#include <memory>

namespace samchon
{
namespace library
{
	class SQLStatement;
};

namespace protocol
{
	/**
	 * @brief An interface supporting DB-I/O
	 *
	 * @details 
	 * <p> ISQLEntity is an interface for interacting with Database. By implementing load() and archive() 
	 * methods, you can make an entity to interact with a Database. 
	 *
	 * <p> If an entity implementing the ISQLEntity is a type of EntityGroup and its children also implemented 
	 * the ISQLEntity, you can realize construction and archiving chain by each child entity to do their
	 * I/O procedures by their own. </p>
	 *
	 * @image html cpp/subset/library_sql.png
	 * @image latex cpp/subset/library_sql.png
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API ISQLEntity
	{
	public:
		/**
		 * @brief Default Constructor.
		 */
		ISQLEntity();
		virtual ~ISQLEntity() = default;

		/**
		 * @brief Load data of entity from DB
		 *
		 * @details
		 * <p> Loads and constructs data of the entity from a SQLStatement. </p>
		 *
		 * <p> If an entity implementing the ISQLEntity is a type of EntityGroup and its children also
		 * implemented the ISQLEntity, you can realize construction chain by each child entity fetches
		 * data of their own by themselves. </p>
		 *
		 * @param stmt SQLStatement storing data of the Entity
		 */
		virtual void load(std::shared_ptr<library::SQLStatement> stmt);

		/**
		 * @brief Archive data of entity to DB
		 * @details
		 * <p> Archives data of the entity to Database by a SQLStatement. </p>
		 *
		 * <p> If an entity implementing the ISQLEntity is a type of EntityGroup and its children also
		 * implemented the ISQLEntity, you can realize archiving chain by each child entity archives
		 * data of their own by themselves. </p>
		 *
		 * @param stmt SQLStatement would store data of the Entity
		 */
		virtual void archive(std::shared_ptr<library::SQLStatement> stmt);

		/**
		 * @brief Get a sql-statement string represents the entity.
		 *
		 * @details
		 * <p> ISQLEntity::toSL() is an abstract method returns a sql-statement string. </p>
		 *
		 * <p> However, if there's some parameter in a sql-statement string from the toString() method,
		 * it is categorized in dynamic sql. In that case, do not access table directly but indirect from
		 * sql-procedure. Indirecting procedure is good for performance and security. </p>
		 *
		 * @return SQL storing record(s) to temporary table of a Procedure
		 */
		virtual auto toSQL() const->std::string;
	};
};
};