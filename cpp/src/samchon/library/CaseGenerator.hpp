#pragma once

#include <vector>

namespace samchon
{
namespace library
{
	/**
	 * @brief Case generator
	 *
	 * @details
	 * <p> An abstract case generator using like a matrix </p>
	 *	\li nTTr(n^r) -> CombinedPermutationGenerator
	 *	\li nPr -> PermutationGenerator
	 *	\li n! -> FactorialGenerator
	 *
	 * \par Example Sources
	 *	\li example::packer
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class CaseGenerator
	{
	protected:
		/**
		 * @brief N, size of the candidates
		 */
		size_t n_;

		/**
		 * @brief R, size of elements of each case
		 */
		size_t r_;

		/**
		 * @brief Size, the number of all cases
		 */
		size_t size_;

	public:
		/* ----------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------- */
		/**
		 * @brief Construct from size of N and R
		 *
		 * @param n Size of candidates
		 * @param r Size of elements of each case
		 */
		CaseGenerator(size_t n, size_t r)
		{
			this->n_ = n;
			this->r_ = r;
		};
		virtual ~CaseGenerator() = default;

		/* ----------------------------------------------------
			ELEMENT ACCESSORS
		---------------------------------------------------- */
		/**
		 * @brief Get size of all cases
		 *
		 * @return Get a number of the all cases
		 */
		auto size() const -> size_t
		{
			return size_;
		};

		/**
		 * @brief Get x'th case
		 *
		 * @return The row of the x'th in combined permuation case
		 */
		virtual auto operator[](size_t) const->std::vector<size_t> = 0;

		/**
		 * @copy CaseGenerator::operator[]()
		 */
		auto at(size_t index) const -> std::vector<size_t>
		{
			if (index > this->size())
				throw std::out_of_range("index number overs number of cases.");

			return operator[](index);
		};

		/**
		 * @brief Get size of the N
		 */
		auto n() const -> size_t
		{
			return n_;
		};

		/**
		 * @brief Get size of the R
		 */
		auto r() const -> size_t
		{
			return r_;
		};

	public:
		/* ----------------------------------------------------
			TO_MATRIX
		---------------------------------------------------- */
		/**
		 * @brief Create a matrix containing all cases.
		 * @warning Size of n or r is enourmouse, then stack overflow can be occured.
		 *
		 * @return A matrix containing all cases.
		 */
		auto toMatrix() const -> std::vector<std::vector<size_t>>
		{
			std::vector<std::vector<size_t>> matrix(size_, std::vector<std::size_t>(r_, 0));

			for (size_t i = 0; i < size_; i++)
				matrix[i] = operator[](i);

			return matrix;
		};
	};
};
};