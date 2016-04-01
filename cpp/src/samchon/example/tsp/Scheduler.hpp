#pragma once
#include <samchon/protocol/Entity.hpp>
#include <samchon/example/tsp/Travel.hpp>

#include <samchon/library/GeneticAlgorithm.hpp>

#include <iostream>
#include <Windows.h>

namespace samchon
{
namespace example
{
namespace tsp
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief Parameters for Genetic-Algorithm
	 *
	 * @details
	 * <p> @image html cpp/example_tsp.png
	 * @image latex cpp/example_tsp.png </p>
	 *
	 * @see samchon::GeneticAlgorithm
	 * @author Jeongho Nam <http://samchon.org>
	 */
	struct GAParameters
	{
		double mutationRate;
		size_t tournament;

		size_t population;
		size_t generation;
	};

	/**
	 * @brief A scheduler
	 * @details Scheduler deducts an optimal schdule(Travel) by genetic algorithm. 
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Scheduler
		: public Entity
	{
	private:
		typedef Entity super;

	protected:
		/**
		 * @brief A travel to optimize or optimized
		 */
		shared_ptr<Travel> travel;

		/**
		 * @brief Parameters for Genetic-Algorithm
		 */
		struct GAParameters gaParameters;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		Scheduler()
			: super()
		{
			this->travel = make_shared<Travel>();
			this->gaParameters = {.03, 50, 100, 300};
		};

		/**
		 * @brief Construct from points and parameter of genetic algorithm.
		 */
		Scheduler(shared_ptr<Travel> travel, const struct GAParameters &gaParameteres)
			: super()
		{
			this->travel = travel;
			this->gaParameters = gaParameteres;
		};

		virtual ~Scheduler() = default;

		virtual void construct(shared_ptr<XML> xml) override
		{
			super::construct(xml);
			gaParameters.mutationRate = xml->getProperty<double>("mutationRate");
			gaParameters.tournament = xml->getProperty<size_t>("tournament");
			gaParameters.population = xml->getProperty<size_t>("population");
			gaParameters.generation = xml->getProperty<size_t>("generation");

			travel->construct(xml->get(travel->TAG())->at(0));
		};

		/* -----------------------------------------------------------
			CALCULATORS
		----------------------------------------------------------- */
		/**
		 * @brief Derive optimized schedule
		 * 
		 * @return An optimized travel schedule.
		 */
		auto optimize() -> shared_ptr<Travel>
		{
			GeneticAlgorithm<Travel> geneticAlgorithm
				(
					true,
					gaParameters.mutationRate,
					gaParameters.tournament
				);

			travel = 
				geneticAlgorithm.evolveGeneArray
				(
					travel, 
					gaParameters.population, 
					gaParameters.generation
				);

			return travel;
		};

		auto calcDistance() const -> double
		{
			return travel->calcDistance();
		};

		/* -----------------------------------------------------------
			EXPORTER
		----------------------------------------------------------- */
		virtual auto TAG() const -> string override
		{
			return "scheduler";
		};

		virtual auto toXML() const -> shared_ptr<XML> override
		{
			shared_ptr<XML> &xml = super::toXML();
			xml->setProperty("mutationRate", gaParameters.mutationRate);
			xml->setProperty("tournament", gaParameters.tournament);
			xml->setProperty("population", gaParameters.population);
			xml->setProperty("generation", gaParameters.generation);

			xml->push_back(travel->toXML());
			return xml;
		};

		auto toString() const -> string
		{
			return travel->toString();
		};

		/* -----------------------------------------------------------
			MAIN
		----------------------------------------------------------- */
		static void main()
		{
			shared_ptr<Travel> travel = make_shared<Travel>();
			for(int i = 0; i < 30; i++)
				travel->emplace_back(new GeometryPoint(i + 1));

			// OPTIMIZING
			struct GAParameters gaParameters = {.03, 30, 400, 400};

			Scheduler scheduler(travel, gaParameters);
			travel = scheduler.optimize();

			// PRINTING
			string &str = travel->toString();

			toClipboard(str);
			cout << str << endl;
		};

	private:
		static void toClipboard(const string &str)
		{
		#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
			OpenClipboard(0);
			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, str.size());

			if (!hg)
			{
				CloseClipboard();
				return;
			}
			memcpy(GlobalLock(hg), str.c_str(), str.size());

			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
			CloseClipboard();
			GlobalFree(hg);
		#endif
		};
	};			
};
};
};