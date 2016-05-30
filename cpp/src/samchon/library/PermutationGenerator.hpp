#pragma once
#include <samchon/library/CaseGenerator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A permutation case generator
	 * @details 
	 * <p> nPr </p>
	 *
	 * @image html cpp/subset/library_case_generator.png
	 * @image latex cpp/subset/library_case_generator.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PermutationGenerator
		: public CaseGenerator
	{
	private:
		typedef CaseGenerator super;

		std::vector<size_t> atoms;

	public:
		/**
		 * @brief Construct from size of N and R
		 *
		 * @copydetails CaseGenerator::CaseGenerator()
		 */
		PermutationGenerator(size_t n, size_t r)
			: super(n, r)
		{
			size_ = n;
			for (size_t i = n - 1; i > n - r; i--)
				size_ *= i;

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