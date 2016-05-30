#pragma once
#include <samchon/library/CaseGenerator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A combined-permutation case generator
	 * @details
	 * <p> <sub>n</sub>TT<sub>r</sub> </p>
	 *
	 * @image html cpp/subset/library_case_generator.png
	 * @image latex cpp/subset/library_case_generator.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class CombinedPermutationGenerator
		: public CaseGenerator
	{
	private:
		typedef CaseGenerator super;

		std::vector<size_t> dividers;

	public:
		/**
		 * @brief Construct from size of N and R
		 *
		 * @copydetails CaseGenerator::CaseGenerator()
		 */
		CombinedPermutationGenerator(size_t n, size_t r)
			: super(n, r)
		{
			this->size_ = (size_t)pow(n, r);

			dividers.assign(r, 0);
			for (size_t i = 0; i < r; i++)
			{
				size_t x = r - (i + 1);
				dividers[i] = (size_t)pow(n, x);
			}
		};
		virtual ~CombinedPermutationGenerator() = default;

		virtual auto operator[](size_t index) const->std::vector<size_t> override
		{
			std::vector<size_t> row(r_, 0);
			
			for (size_t i = 0; i < row.size(); i++)
				row[i] = (index / dividers[i]) % n_;

			return row;
		};
	};
};
};