#pragma once

#include <samchon/library/GAPopulation.hpp>

#include <set>
#include <random>
#include <samchon/library/Math.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A genetic algorithm class
	 * 
	 * @details
	 * <p> In the field of artificial intelligence, a genetic algorithm (GA) is a search heuristic 
	 * that mimics the process of natural selection. This heuristic (also sometimes called a metaheuristic) 
	 * is routinely used to generate useful solutions to optimization and search problems. </p>
	 * 
	 * <p> Genetic algorithms belong to the larger class of evolutionary algorithms (EA), which generate 
	 * solutions to optimization problems using techniques inspired by natural evolution, such as 
	 * inheritance, mutation, selection, and crossover. </p>
	 *
	 *	\li Referenced Wikipedia: https://en.wikipedia.org/wiki/Genetic_algorithm
	 *
	 * <p> @image html cpp/subset/library_genetic_algorithm.png
	 * @image latex cpp/subset/library_genetic_algorithm.png </p>
	 *
	 * \par Example Sources
	 * 	\li example::tsp
	 *
	 * @warning
	 * <p> Be careful for the mistakes of direction or position of Compare. </p>
	 * <p> Most of logical errors failed to access optimal solution are occured by those mistakens. </p>
	 *
	 * @tparam GeneArray An array(std::vector) containing genes as elments; sequnce listing.
	 *					 The GeneArray must be a type of <i>std::vector</i>.
	 *
	 * @tparam Compare
	 *	<p> A comparison class (or struct) returns whether left gene is more optimal. </p>
	 *
	 *	<p> Default template parameter of Compare is <i>std::less<GeneArray></i>. It means to compare
	 *	two std::vector (GeneArray must be a std::vector). Thus, you've to keep follwing rules. </p>
	 *
	 *	<p> If you don't want to follow the rules or want a custom comparison class, you have to
	 *	realize a comparison class. The following code is an example realizing the comparison class. </p>
	 *
	 *		- GeneArray is inherited from <i>std::vector</i>
	 *		- GeneArray has custom <i>auto operator<(const GeneArray &) const -> bool</i>
	 *
	 * @code
	 template <typename _Ty>
	 struct MyCompare
	 {
	 auto operator()(const _Ty &newObj, const _Ty &prevObj) const -> bool;
	 };
	 * @endcode
	 *
	 * @see library::GAPopulation
	 * @see samchon::library
	 * @see example::tsp
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename GeneArray, typename Compare = std::less<GeneArray>>
	class GeneticAlgorithm
	{
	public:
		typedef GAPopulation<GeneArray, Compare> Population;

	protected:
		/**
		 * @brief Whether each element (Gene) is unique in their GeneArray
		 */
		bool unique;

		/**
		 * @brief Rate of mutate ocurrence
		 *
		 * @details
		 * The mutationRate determines the percentage of occurence of mutation in a GeneArray.
		 *
		 * @note
		 *	\li When mutationRate is too high, it is hard to ancitipate studying on genetic algorithm.
		 *	\li When mutationRate is too low and initial set of genes(GeneArray) is far away from optimal,
		 *	the evolution tends to wandering outside of he optimal.
		 */
		double mutationRate;

		/**
		 * @brief Size of tournament in selection
		 */
		size_t tournament;

	public:
		/**
		 * @brief Construct from parameters of Genetic Algorithm
		 *
		 * @param unique Whether each Gene is unique in their GeneArray
		 * @param mutationRate Rate of mutation
		 * @param tournament Size of tournament in selection
		 */
		GeneticAlgorithm(bool unique, double mutationRate = 0.015, size_t tournament = 10)
		{
			this->unique = unique;
			this->mutationRate = mutationRate;
			this->tournament = tournament;
		};

		/**
		 * @brief Evolve a GeneArray 
		 * @details Convenient method accessing to evolvePopulation().
		 *
		 * @param individual An initial set of genes; sequence listing
		 * @param population Size of population in a generation
		 * @param generation Size of generation in evolution
		 *
		 * @return A evolved GeneArray optimally
		 */
		inline auto evolveGeneArray(std::shared_ptr<GeneArray> individual, size_t population, size_t generation) const -> std::shared_ptr<GeneArray>
		{
			std::shared_ptr<Population> myPopulation(new Population(individual, population));

			for (size_t i = 0; i < generation; i++)
				myPopulation = evolvePopulation(myPopulation);

			return myPopulation->fitTest();
		};

		/**
		 * @brief Evolve population, a mass of GeneArray(es)
		 *
		 * @param population An initial population
		 */
		auto evolvePopulation(std::shared_ptr<Population> population) const -> std::shared_ptr<Population>
		{
			size_t size = population->children.size();
			std::shared_ptr<Population> evolved(new Population(size));

			//ELITICISM
			evolved->children[0] = population->fitTest();

			#pragma omp parallel for
			for (int i = 1; i < size; i++)
			{
				std::shared_ptr<GeneArray> &gene1 = selection(population);
				std::shared_ptr<GeneArray> &gene2 = selection(population);

				std::shared_ptr<GeneArray> &child = crossover(gene1, gene2);
				mutate(child);

				evolved->children[i] = child;
			}

			/*#pragma omp parallel for
			for (int i = 1; i < size; i++)
				mutate(evolved->children[i]);*/

			return evolved;
		};

	private:
		/**
		 * @brief Select the best GeneArray in population from tournament
		 *
		 * @details
		 * <p> Selection is the stage of a genetic algorithm in which individual genomes are chosen from
		 * a population for later breeding (using crossover operator). A generic selection procedure may be
		 * implemented as follows: </p>
		 *
		 * <ol>
		 *	<li>
		 *		The fitness function is evaluated for each individual, providing fitness values, which are
		 *		then normalized. Normalization means dividing the fitness value of each individual by
		 *		the sum of all fitness values, so that the sum of all resulting fitness values equals 1.
		 *	</li>
		 *	<li> The population is sorted by descending fitness values. </li>
		 *	<li>
		 *		Accumulated normalized fitness values are computed (the accumulated fitness value of an individual
		 *		is the sum of its own fitness value plus the fitness values of all the previous individuals).
		 *		The accumulated fitness of the last individual should be 1 (otherwise something went wrong in
		 *		the normalization step).
		 *	</li>
		 *	<li> A random number R between 0 and 1 is chosen. </li>
		 *	<li> The selected individual is the first one whose accumulated normalized value is greater than R. </li>
		 * </ol>
		 *
		 *	\li Referenced Wekipedia: https://en.wikipedia.org/wiki/Selection_(genetic_algorithm)
		 *
		 * @param population The target of tournament
		 * @return The best genes derived by the tournament
		 */
		auto selection(std::shared_ptr<Population> population) const -> std::shared_ptr<GeneArray>
		{
			size_t size = population->children.size();
			Population tornament(size);

			for (size_t i = 0; i < size; i++)
			{
				size_t randomIndex = (size_t)(Math::random() * size);
				if (randomIndex == size)
					randomIndex--;

				tornament.children[i] = population->children[randomIndex];
			}
			return tornament.fitTest();
		};

		/**
		 * @brief Create a new GeneArray by crossing over two GeneArray(s)
		 *
		 * @details
		 * <p> crossover is a genetic operator used to vary the programming of a chromosome or
		 * chromosomes from one generation to the next. It is analogous to reproduction and
		 * biological crossover, upon which genetic algorithms are based. </p>
		 *
		 * <p> Cross over is a process of taking more than one parent solutions and producing a
		 * child solution from them. There are methods for selection of the chromosomes. </p>
		 *
		 *	\li Referenced Wikipedia: https://en.wikipedia.org/wiki/Crossover_(genetic_algorithm)
		 *
		 * @param parent1 A parent sequence listing
		 * @param parent2 A parent sequence listing
		 */
		auto crossover(std::shared_ptr<GeneArray> &parent1, std::shared_ptr<GeneArray> &parent2) const -> std::shared_ptr<GeneArray>
		{
			std::shared_ptr<GeneArray> individual(new GeneArray(*parent1));
			size_t size = parent1->size();

			if (unique == false)
			{
				for (size_t i = 0; i < size; i++)
					if (Math::random() > .5)
						individual->at(i) = parent2->at(i);
			}
			else
			{
				std::set<GeneArray::value_type> ptrSet;
				std::set<size_t> indexSet;

				// RANGES
				size_t start = (size_t)(Math::random() * size);
				size_t end = (size_t)(Math::random() * size);

				if (start > end)
					std::swap(start, end);

				//INDEXING
				for (size_t i = 0; i < size; i++)
					if (start <= i && i < end)
						ptrSet.insert(parent1->at(i));
					else
						indexSet.insert(i);

				//INSERT PARENT_2
				for (size_t i = 0; i < size; i++)
				{
					GeneArray::value_type &ptr = parent2->at(i);
					if (ptrSet.find(ptr) != ptrSet.end())
						continue;

					individual->at(*indexSet.begin()) = ptr;
					indexSet.erase(indexSet.begin());
				}
			}
			return individual;
		};

	protected:
		/**
		 * @brief Cause a mutation on the GeneArray
		 *
		 * @details
		 * <p> Mutation is a genetic operator used to maintain genetic diversity from one generation
		 * of a population of genetic algorithm chromosomes to the next. It is analogous to biological
		 * mutation. </p>
		 *
		 * <p> Mutation alters one or more gene values in a chromosome from its initial state.
		 * In mutation, the solution may change entirely from the previous solution. Hence GA can come to
		 * better solution by using mutation. </p>
		 *
		 * <p> Mutation occurs during evolution according to a user-definable mutation probability.
		 * This probability should be set low. If it is set too high, the search will turn into a primitive
		 * random search. </p>
		 *
		 *	\li Referenced Wikipedia: https://en.wikipedia.org/wiki/Mutation_(genetic_algorithm)
		 *
		 * @note
		 * <p> Muttion is pursuing diversity. Mutation is useful for avoiding the following problem. </p>
		 *
		 * <p> When initial set of genes(GeneArray) is far away from optimail, without mutation (only
		 * with selection and crossover), the genetic algorithm has a tend to wandering outside of
		 * the optimal. </p>
		 *
		 * <p> Genes in the GeneArray will be swapped following percentage of the mutationRate. </p>
		 *
		 * @param individual A container of genes to mutate
		 * @see mutationRate;
		 */
		virtual void mutate(std::shared_ptr<GeneArray> individual) const
		{
			for (size_t i = 0; i < individual->size(); i++)
			{
				if (Math::random() > mutationRate)
					continue;

				// JUST SHUFFLE SEQUENCE OF GENES
				size_t j = (size_t)(Math::random() * individual->size());
				std::swap(individual->at(i), individual->at(j));
			}
		};
	};
};
};