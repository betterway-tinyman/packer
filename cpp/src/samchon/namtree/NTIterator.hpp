#pragma once
#include <samchon/API.hpp>

#include <samchon/library/IOperator.hpp>

#include <memory>

namespace samchon
{
namespace namtree
{
	class NTEntityGroup;

		/**
		 * @brief Iterator of historical data
		 *
		 * @details
		 * <p> NTIterator is an abstract iterator of NTEntityGroup. </p>
		 *
		 * <p> @image html  cpp/namtree_criteria.png
		 *	   @image latex cpp/namtree_criteria.png </p>
		 *
		 * @see samchon::namtree
		 * @author Jeongho Nam
		 */
	class SAMCHON_FRAMEWORK_API NTIterator
		: public library::IOperator<NTIterator>
	{
	private:
		typedef library::IOperator<NTIterator> super;

	protected:
		/**
		 * @brief A historical data, source of the iterator.
		 */
		const NTEntityGroup *data;

	public:
		/**
		 * @brief Construt from historical data.
		 */
		NTIterator(const NTEntityGroup*);
		virtual ~NTIterator() = default;

		/**
		 * @brief Get iterator to previous element
		 * @details If current iterator is the first item(equal with <i>begin()</i>), returns end().
		 *
		 * @return An iterator of the previous item.
		 */
		virtual auto operator--() const->std::shared_ptr<NTIterator> = 0;

		/**
		 * @brief Get iterator to next element
		 * @details If current iterator is the last item, returns end().
		 *
		 * @return An iterator of the next item.
		 */
		virtual auto operator++() const->std::shared_ptr<NTIterator> = 0;

		/**
		 * @brief Whether an iterator is equal with the iterator.
		 * @details Compare two iterators and returns whether they are equal or not.
		 *
		 * @param obj An iterator to compare
		 * @return Indicates whether equal or not.
		 */
		virtual auto operator==(const NTIterator&) const -> bool = 0;

		/**
		 * @brief Whether the iterator is less than an iterator.
		 * @details Compare two iterators and returns whether this iterator is less.
		 *
		 * @param obj An iterator to compare
		 * @return Indicates whether less or not.
		 */
		virtual auto operator<(const NTIterator&) const -> bool = 0;
	};
};
};