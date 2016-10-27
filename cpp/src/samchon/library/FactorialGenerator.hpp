#pragma once
#include <samchon/library/PermutationGenerator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Factorical case generator
	 * @details 
	 * <p> n! = nPn </p>
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_math.png)
	 *
	 * @handbook [Library - Genetic Algorithm](https://github.com/samchon/framework/wiki/CPP-Library-Case_Generator)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class FactorialGenerator
		: public PermutationGenerator
	{
	private:
		typedef PermutationGenerator super;

	public:
		/**
		 * @brief Construct from size N
		 *
		 * @param n Size of elements of each case to be arranged.
		 */
		FactorialGenerator(size_t size)
			: super(size, size)
		{
		};
		virtual ~FactorialGenerator() = default;
	};
};
};