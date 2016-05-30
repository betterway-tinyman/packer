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
	 * @image html cpp/subset/library_case_generator.png
	 * @image latex cpp/subset/library_case_generator.png
	 *
	 * @see samchon::library
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