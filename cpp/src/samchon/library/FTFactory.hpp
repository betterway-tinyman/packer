#pragma once
#include <samchon/API.hpp>

#include <samchon/Map.hpp>
#include <samchon/library/XML.hpp>

namespace samchon
{
namespace library
{
	class FTInstance;
	class FTFolder;
	class FTFile;

	/**
	 * @brief A Factory and manager for file-tree
	 *
	 * @details
	 * <p> FTFactory is a class taking a role of creating FTFile instances and managing
	 * them by having their direct pointers. FTFactory also has a role of archiving and
	 * loading from DB. </p>
	 *
	 * @image html cpp/library_file_tree.png
	 * @image latex cpp/library_file_tree.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API FTFactory
	{
	protected:
		/**
		 * @brief An application id
		 *
		 * @details An enumeration code of an application that managed FTInstance(s) are belonged to.
		 */
		int application;

		/**
		 * @brief A categori id
		 *
		 * @details An enumeration code of a category that managed FTInstance(s) are belonged to.
		 */
		int category;

		/**
		 * @brief Account id of a member
		 *
		 * @details
		 */
		std::string member;

		/**
		 * @brief Map of files
		 *
		 * @details
		 *	\li key: uid
		 *	\li value: pointer of file
		 */
		Map<int, FTInstance*> instanceMap;

	public:
		/**
		 * @brief Default Constructor
		 */
		FTFactory();
		virtual ~FTFactory() = default;

		/**
		 * @brief Factory method of a file
		 */
		virtual auto createFile(FTFolder*, std::shared_ptr<XML>)->FTFile* = 0;

		/**
		 * @brief Register file instance to map
		 */
		void registerInstance(FTInstance*);
	};
};
};