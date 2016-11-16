#pragma once
#include <samchon/library/CaseGenerator.hpp>

#define TEMPLATE_FACTORIAL_CASE_MACRO(N) case N: size_ = factorial<N>::value; break;

namespace samchon
{
namespace library
{
	/**
	 * @brief A permutation case generator
	 * @details 
	 * <p> nPr </p>
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_math.png)
	 *
	 * @handbook [Library - Genetic Algorithm](https://github.com/samchon/framework/wiki/CPP-Library-Case_Generator)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PermutationGenerator
		: public CaseGenerator
	{
	private:
		typedef CaseGenerator super;

		std::vector<size_t> atoms;

		template<const size_t N> struct factorial
		{
			enum { value = N * factorial<N - 1>::value };
		};
		template<> struct factorial<1>
		{
			enum {value = 1};
		};

	public:
		/**
		 * @brief Construct from size of N and R
		 *
		 * @copydetails CaseGenerator::CaseGenerator()
		 */
		PermutationGenerator(size_t n, size_t r)
			: super(n, r)
		{
			if (n == r)
				if (n <= 15)
					switch (n)
					{
						TEMPLATE_FACTORIAL_CASE_MACRO(1)
						TEMPLATE_FACTORIAL_CASE_MACRO(2)
						TEMPLATE_FACTORIAL_CASE_MACRO(3)
						TEMPLATE_FACTORIAL_CASE_MACRO(4)
						TEMPLATE_FACTORIAL_CASE_MACRO(5)
						TEMPLATE_FACTORIAL_CASE_MACRO(6)
						TEMPLATE_FACTORIAL_CASE_MACRO(7)
						TEMPLATE_FACTORIAL_CASE_MACRO(8)
						TEMPLATE_FACTORIAL_CASE_MACRO(9)
						TEMPLATE_FACTORIAL_CASE_MACRO(10)
						TEMPLATE_FACTORIAL_CASE_MACRO(11)
						TEMPLATE_FACTORIAL_CASE_MACRO(12)
						TEMPLATE_FACTORIAL_CASE_MACRO(13)
						TEMPLATE_FACTORIAL_CASE_MACRO(14)
						TEMPLATE_FACTORIAL_CASE_MACRO(15)
					}
				else
				{
					size_ = factorial<15>::value;
					for (size_t i = 16; i < n; i++)
						size_ *= i;
				}
			else
			{
				size_ = 1;
				for (size_t i = n - r + 1; i <= n; i++)
					size_ *= i;
			}

			atoms.assign(n, 0);
			for (size_t i = 0; i < n; i++)
				atoms[i] = i;
		};
		virtual ~PermutationGenerator() = default;

		virtual auto operator[](size_t index) const->std::vector<size_t> override
		{
			std::vector<size_t> atoms = this->atoms;
			std::vector<size_t> row(r_, NULL);

			for (size_t i = 0; i < row.size(); i++)
			{
				size_t item = index % atoms.size();
				index = index / atoms.size();

				row[i] = atoms[item];
				atoms.erase(atoms.begin() + item);
			}
			return row;
		};
	};
};
};