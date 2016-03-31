#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace samchon
{
namespace library
{
	/**
	 * @brief A population of a generation in G.A.
	 *
	 * @tparam GeneArray
	 * <p> An array(std::vector) containing genes as elments; sequnce listing </p>
	 * <p> The GeneArray must be a type of <i>std::vector</i>. </p>
	 *
	 * @tparam Compare
	 *	A comparison class (or struct) returns whether left gene is more optimal.\n\n
	 *
	 *	Default template parameter of Compare is <i>std::less<GeneArray></i>. It means to compare
	 *	two std::vector (GeneArray must be a std::vector). Thus, you've to keep follwing rules.\n\n
	 *
	 *	If you don't want to follow the rules or want a custom comparison class, you have to
	 *	realize a comparison class. The following code is an example realizing the comparison class.
	 *
	 *		- GeneArray is inherited from <i>std::vector</i>
	 *		- GeneArray has custom <i>auto operator<(const GeneArray &) const -> bool</i>
	 *
	 * @code
	 * template <typename _Ty>
	 * struct MyCompare
	 * {
	 * auto operator()(const _Ty &newObj, const _Ty &prevObj) const -> bool;
	 * };
	 * @endcode
	 *
	 * @details
	 * <p> GAPopulation is a class representing population of candidate genes(sequence listing)
	 * having an array of GeneArray as a member. GAPopulation also manages initial set of genes and
	 * handles fitting test direclty by the method <i>fitTest</i>. </p>
	 *
	 * <p> The success of evolution of genetic alogirhtm is depend on the GAPopulation's initial set
	 * and fitting test. (GeneArray and Compare.) </p>
	 *
	 * <p> @image html cpp/subset/library_genetic_algorithm.png
	 * @image latex cpp/subset/library_genetic_algorithm.png </p>
	 *
	 * \par Example Sources
	 *	\li example::tsp
	 *
	 * @warning
	 * <p> Be careful for the mistakes of direction or position of Compare. </p>
	 * <p> Most of logical errors failed to access optimal solution are occured by those mistakens. </p>
	 *
	 * @see library::GeneticAlgorithm
	 * @see samchon::library
	 * @see example::tsp
	 *
	 * @author Jeonghoh Nam
	 */
	template <typename GeneArray, typename Compare = std::less<GeneArray>>
	class GAPopulation
	{
		template <typename GeneArray, typename Compare, typename Gene>
		friend class GeneticAlgorithm;

	private:
		/**
		 * @brief Genes representing the population
		 */
		std::vector<std::shared_ptr<GeneArray>> children;

		/**
		 * @brief Private Constructor with population
		 *
		 * @details
		 * <p> Private constructor of GAPopulation does not create children (candidate genes)
		 * but fills only <i>nullptr</i> repeatedly following the size. </p>
		 * <p> The private constructor is only for and handled by GeneticAlgorithm. </p>
		 *
		 * @param size Size of the population
		 */
		GAPopulation(size_t size)
		{
			children.assign(size, nullptr);
		};

	public:
		/**
		 * @brief Construct from a GeneArray and size of the population
		 *
		 * @details
		 * <p> Constructor with initial set of genes and size of the population. </p>
		 * <p> The public constructor creates GeneArray(s) as population (size) having
		 * shuffled genes which are came from the initial set of genes (geneArray). </p>
		 *
		 * @note The success of genetic algorithm is depend on the initial genes(geneArray).
		 *
		 * @param geneArray An initial sequence listing
		 * @param size The size of population to have as children
		 */
		GAPopulation(std::shared_ptr<GeneArray> geneArray, size_t size)
		{
			children.reserve(size);

			for (size_t i = 0; i < size; i++)
			{
				GeneArray *ptr = new GeneArray(*geneArray);

				if (i > 0) //FOR ELITICISM
					std::random_shuffle(ptr->begin(), ptr->end());

				children.emplace_back(ptr);
			}
		};

		/**
		 * @brief Test fitness of each GeneArray in the population.
		 *
		 * @return The best GeneArray in the population
		 */
		auto fitTest() const -> std::shared_ptr<GeneArray>
		{
			std::shared_ptr<GeneArray> best = children[0];

			for (size_t i = 1; i < children.size(); i++)
				if (Compare()(*children[i], *best) == true)
					best = children[i];

			return best;
		};
	};
};
};