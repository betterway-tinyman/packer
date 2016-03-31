#pragma once
#include <samchon/example/interaction/Master.hpp>

#include <samchon/example/interaction/ChiefDriver.hpp>
#include <samchon/example/interaction/SlaveDriver.hpp>
#include <samchon/protocol/Invoke.hpp>

#include <samchon/example/tsp/Scheduler.hpp>

namespace samchon
{
namespace example
{
namespace interaction
{
	using namespace std;

	using namespace library;
	using namespace protocol;

	/**
	 * @brief A master for parallel system solving TSP problem.
	 * 
	 * @details
	 * \par [Inherited]
	 *		@copydetails interaction::Master
	 * 		  
	 * @author Jeongho Nam
	 */
	class TSPMaster
		: public Master
	{
	private:
		typedef Master super;

	protected:
		shared_ptr<tsp::Scheduler> scheduler;

	public:
		/* ---------------------------------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		TSPMaster()
			: super(37100)
		{
			scheduler = make_shared<tsp::Scheduler>();
		};
		virtual ~TSPMaster() = default;

	protected:
		/* ---------------------------------------------------------------------------------
			INVOKE MESSATE CHAIN
		--------------------------------------------------------------------------------- */
		virtual void optimize(shared_ptr<XML> xml) override
		{
			super::optimize(nullptr);

			scheduler->construct(xml);

			sendSegmentData
			(
				make_shared<Invoke>
				(
					"optimize", 
					scheduler->toXML()
				), 
				this->size()
			);
		};

		virtual void replyOptimization(shared_ptr<XML> xml) override
		{
			unique_lock<mutex> uk(mtx);

			shared_ptr<tsp::Scheduler> scheduler(new tsp::Scheduler());
			scheduler->construct(xml);

			cout << "An optimization process from a slave system has completed" << endl;
			cout << "\tOrdinary minimum distance: " << this->scheduler->calcDistance()
				<< ", New Price from the slave: " << scheduler->calcDistance() << endl;

			if (scheduler->calcDistance() < this->scheduler->calcDistance())
				this->scheduler = scheduler;

			if (++optimized < this->size())
				return;

			cout << endl;
			cout << "Parallel optimization has completed." << endl;
			cout << scheduler->toString() << endl << endl;

			chiefDriver->sendData
			(
				make_shared<Invoke>
				(
					"replyOptimization", 
					scheduler->toXML()
				)
			);
		};

	public:
		/* ---------------------------------------------------------------------------------
			MAIN
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Main function
		 */
		static void main()
		{
			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	TSP SOLVER MASTER" << endl;
			cout << "----------------------------------------------------------------------------" << endl;

			TSPMaster master;
			master.start();
		};
	};			
};
};
};