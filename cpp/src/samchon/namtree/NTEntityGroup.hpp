#pragma once
#include <samchon/API.hpp>

#include <memory>

namespace samchon
{
namespace namtree
{
	class NTIterator;

	/** 
	 * @brief A historical, studying data
	 *
	 * @details
	 * <p> NTEntityGroup is an abstract class representing historical, regressive data </p>
	 * <p> The NTEntitGroup is used to studying data evolving and training NTCriteria, representing 
	 * Artificial neural network. </p>
	 *
	 * <p> @image html  cpp/namtree_criteria.png
	 *	   @image latex cpp/namtree_criteria.png </p>
	 *
	 * @see samchon::namtree
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API NTEntityGroup
	{
	public:
		/**
		 * @brief Default Constructor.
		 */
		NTEntityGroup();
		virtual ~NTEntityGroup() = default;

		/**
		 * @brief Return iterator to beginning
		 *
		 * @details Returns an iterator referring the first element in the historical data container.
		 * @note If the container is empty, the returned iterator is same with end().
		 *
		 * @return
		 * <p> An iterator to the first element in the container. </p>
		 * <p> The iterator containes the first element's pair; key and value. </p>
		 */
		virtual auto begin() const->std::shared_ptr<NTIterator> = 0;

		/**
		 * @brief Return iterator to end
		 *
		 * @details
		 * <p> Returns an iterator referring to the past-the-end element in the historical data container. </p>
		 *
		 * <p> The past-the-end element is the theoretical element that would follow the last element in
		 * the NTEntityGroup container. It does not point to any element, and thus shall not be dereferenced. </p>
		 *
		* <p> Because the ranges used by functions of the NTEntityGroup do not include the element reference
		* by their closing iterator, this function is often used in combination with NTEntityGroup::begin()
		* to specify a range including all the elements in the container. </p>
		*
		* @note
		* <p> Returned iterator from NTEntityGroup::end() does not refer any element. Trying to accessing
		* element by the iterator will cause throwing exception (out of range). </p>
		* <p> If the container is empty, this function returns the same as NTEntityGroup::begin(). </p>
		*/
		virtual auto end() const->std::shared_ptr<NTIterator> = 0;
	};
};
};
