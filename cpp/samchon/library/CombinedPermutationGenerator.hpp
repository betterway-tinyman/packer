#pragma once
#include <samchon/API.hpp>

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
	class SAMCHON_FRAMEWORK_API CombinedPermutationGenerator
		: public CaseGenerator
	{
	private:
		typedef CaseGenerator super;

	public:
		/**
		 * @brief Construct from size of N and R
		 *
		 * @copydetails CaseGenerator::CaseGenerator()
		 */
		CombinedPermutationGenerator(size_t n, size_t r);
		virtual ~CombinedPermutationGenerator() = default;

		virtual auto operator[](size_t) const->std::vector<size_t> override;
	};
};
};