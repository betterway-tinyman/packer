#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A structure containing parameters of genetic algorithm
	 *
	 * @details
	 * <p> The number and rate parameters in the structure affects success of evolution of genetic algorithm. </p> 
	 *
	 * \par Example Sources
	 *	\li example::tsp
	 *
	 * @see library::GeneticAlgorithm
	 * @see library::GAPopulation
	 * @see example::tsp
	 * 
	 * @author Jeongho Nam
	 */
	class GAParameters
		: public protocol::Entity
	{
	private:
		typedef protocol::Entity super;

	protected:
		/**
		 * @brief A number of generation of evolution.
		 */
		size_t generation;

		/**
		 * @brief A number of population in a generation.
		 */
		size_t population;

		/**
		 * @brief A number of tournament in a selection.
		 */
		size_t tournament;

		/**
		 * @brief Rate of mutate ocurrence.
		 *
		 * @details
		 * <p> A mutation rate determines the percentage of occurence of mutation in a sequence list. </p>
		 *	\li Value from 0.0 to 1.0
		 *
		 * @note
		 *	\li When mutationRate is too high, it is hard to ancitipate studying on genetic algorithm.
		 *	\li When mutationRate is too low and initial set of genes(GeneArray) is far away from optimal,
		 *	the evolution tends to wandering outside of he optimal.
		 */
		double mutationRate;

	public:
		/* ===========================================================
			CONSTRUCTORS & SETTERS
		==============================================================
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		GAParameters()
			: GAParameters(1000, 200, 50, .05)
		{
		};

		/**
		 * Construct from members.
		 */
		GAParameters(size_t generation, size_t population, size_t tournament, double mutationRate)
			: super()
		{
			this->generation = generation;
			this->population = population;
			this->tournament = tournament;
			this->mutationRate = mutationRate;
		};

		virtual void construct(std::shared_ptr<XML> xml) override
		{
			generation = xml->getProperty<size_t>("generation");
			population = xml->getProperty<size_t>("population");
			tournament = xml->getProperty<size_t>("tournament");
			mutationRate = xml->getProperty<double>("mutationRate");
		};

		/* -----------------------------------------------------------
			SETTERS
		----------------------------------------------------------- */
		/**
		 * Set generation.
		 */
		void setGeneration(size_t val)
		{
			generation = val;
		}

		/**
		 * Set population.
		 */
		void setPopulation(size_t val)
		{
			population = val;
		}

		/**
		 * Set tournament.
		 */
		void setTournament(size_t val)
		{
			tournament = val;
		}

		/**
		 * Set mutation rate.
		 */
		void setMutationRate(double val)
		{
			mutationRate = val;
		}

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Get generation.
		 */
		auto getGeneration() const -> size_t
		{
			return generation;
		};

		/** 
		 * Get population.
		 */
		auto getPopulation() const -> size_t
		{
			return population;
		};

		/**
		 * Get tournament.
		 */
		auto getTournament() const -> size_t
		{
			return tournament;
		};

		/**
		 * Get mutation rate.
		 */
		auto getMutationRate() const -> double
		{
			return mutationRate;
		};

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string
		{
			return "gaParameters";
		};
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<XML> &xml = super::toXML();
			xml->setProperty("generation", generation);
			xml->setProperty("population", population);
			xml->setProperty("tournament", tournament);
			xml->setProperty("mutationRate", mutationRate);

			return xml;
		};
	};
};
}