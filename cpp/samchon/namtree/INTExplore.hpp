#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace samchon
{
namespace namtree
{
	/**
	 * @brief An interface for exploration
	 *
	 * @details
	 * <p> @image html  cpp/result/namtree_i_nt_explore.png
	 *	   @image latex cpp/result/namtree_i_nt_explore.png </p>
	 *
	 * <p> INTExplore is an interface containing some parameters for grid optimization method. </p>
	 *
	 * <p> @image html  cpp/namtree_file.png
	 *	   @image latex cpp/namtree_file.png </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API INTExplore
		: public virtual protocol::Entity
	{
	private:
		typedef protocol::Entity super;

	protected:
		/**
		 * @brief Minimum value
		 */
		double minimum;

		/**
		 * @brief Maximum value
		 */
		double maximum;

		/**
		 * @brief Section number of exploration
		 */
		unsigned int section;

		/**
		 * @brief Target precision: 10<sup>precision</sup>
		 */
		int precision;

	public:
		/* -------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		INTExplore();
		virtual ~INTExplore() = default;

		virtual void construct(std::shared_ptr<library::XML>);

		/* -------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------- */
		/**
		 * @brief Get minimum
		 */
		auto getMinimum() const -> double;

		/**
		 * @brief Get maximum
		 */
		auto getMaximum() const -> double;

		/**
		 * @brief Get section
		 */
		auto getSection() const -> unsigned int;

		/**
		 * @brief Get precision
		 */
		auto getPrecision() const -> int;

		/* -------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------- */
		virtual auto toXML() const->std::shared_ptr<library::XML>;
	};
};
};