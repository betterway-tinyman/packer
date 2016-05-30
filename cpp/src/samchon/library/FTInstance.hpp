#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace samchon
{
namespace library
{
	class FTFolder;

		/**
		 * @brief An interface of file-tree.
		 * @details An abstract class for folder and file instances.
		 *
		 * @image html cpp/library_file_tree.png
		 * @image latex cpp/library_file_tree.png
		 *
		 * @see samchon::library
		 * @author Jeongho Nam <http://samchon.org>
		 */
	class SAMCHON_FRAMEWORK_API FTInstance
		: public virtual protocol::Entity
	{
	private:
		typedef protocol::Entity super;

	protected:
		virtual auto TAG() const -> std::string;

		/**
		 * @brief Parent folder containing the instance
		 */
		FTFolder *parent;

		/**
		 * @brief Key, an unique id of file
		 */
		int uid;

		/**
		 * @brief Name of the file
		 */
		std::string name;

		/**
		 * @brief Comment of the file
		 */
		std::string comment;

	public:
		/* ========================================================
			CONSTRUCTORS
		======================================================== */
		/**
		 * @brief Construct from parent folder
		 *
		 * @param parent Parent folder that this instance is belonged to
		 */
		FTInstance(FTFolder*);
		virtual ~FTInstance() = default;

		virtual void construct(std::shared_ptr<library::XML> xml);

		/* ========================================================
			GETTERS
		======================================================== */
		virtual auto key() const -> std::string;

		/**
		 * @brief Get uid
		 */
		auto getUID() const -> int;

		/**
		 * @brief Get parent folder
		 */
		auto getParent() const -> FTFolder*;

		/**
		 * @brief Get name
		 */
		auto get_name() const -> std::string;

		/**
		 * @brief Get comment
		 */
		auto getComment() const -> std::string;


		/* ========================================================
			EXPORTER
		======================================================== */
		virtual auto toXML() const -> std::shared_ptr<library::XML>;
	};
};
};