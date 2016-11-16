#pragma once
#include <bws/packer/API.hpp>

#include <samchon/library/GeneticAlgorithm.hpp>

#include <bws/packer/GAWrapperArray.hpp>
#include <bws/packer/WrapperArray.hpp>

#include <samchon/library/Math.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief A genetic algorithm class for Packer.
	 *
	 * @details 
	 * Rereferences
	 *	\li http://samchon.github.io/framework/api/cpp/d5/d28/classsamchon_1_1library_1_1GeneticAlgorithm.html
	 *	\li https://en.wikipedia.org/wiki/Genetic_algorithm
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PCKGeneticAlgorithm
		: public library::GeneticAlgorithm<GAWrapperArray>
	{
	private:
		typedef library::GeneticAlgorithm<GAWrapperArray> super;

		/**
		 * @brief Candidate wrappers of mutation.
		 */
		std::shared_ptr<WrapperArray> candidates;

	public:
		/**
		 * @brief Construct from members
		 *
		 * @param mutationRate Rate of mutation
		 * @param tournament Size of tournament in selection
		 * @param candidates Candidate wrappers of mutation
		 */
		PCKGeneticAlgorithm(double mutate, size_t tournament, std::shared_ptr<WrapperArray> candidates)
			: super(false, mutate, tournament)
		{
			this->candidates = candidates;
		};

	protected:
		/**
		 * @brief Cause a mutation on a sequence list.
		 *
		 * @details
		 * References
		 *	\li http://samchon.github.io/framework/api/cpp/d5/d28/classsamchon_1_1library_1_1GeneticAlgorithm.html#ac5c1dcb9ca7ecd8980dca7aac2316ff0
		 *	\li https://en.wikipedia.org/wiki/Mutation_(genetic_algorithm)
		 *
		 * @param individual A sequcen list.
		 */
		virtual void mutate(std::shared_ptr<GAWrapperArray> individual) const override
		{
			for (size_t i = 0; i < individual->size(); i++)
			{
				if (library::Math::random() > mutationRate)
					continue;

				size_t index = (size_t)(library::Math::random() * candidates->size());
				individual->at(i) = candidates->at(index);
			}
		};
	};
};
};