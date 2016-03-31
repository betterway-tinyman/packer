#pragma once
#include <samchon/API.hpp>

#include <samchon/library/FTInstance.hpp>

namespace samchon
{
namespace library
{
	/** 
	 * @brief A file
	 * @details An abstract class represents a file instance with extension.
	 *
	 * @image html cpp/library_file_tree.png
	 * @image latex cpp/library_file_tree.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API FTFile
		: public FTInstance
	{
	private:
		typedef FTInstance super;

	protected:
		/**
		 * @brief An extension of a file.
		 * @details FTFile's identifier is composited by name and extension.
		 */
		std::string extension;

	public:
		/**
		 * @brief Construct from parent folder
		 *
		 * @param folder Belonged folder
		 */
		FTFile(FTFolder*);
		virtual ~FTFile() = default;

		virtual void construct(std::shared_ptr<library::XML> xml);

		/**
		 * @brief Get extension.
		 */
		auto getExtension() const->std::string;

		virtual auto toXML() const->std::shared_ptr<library::XML>;
	};
};
};