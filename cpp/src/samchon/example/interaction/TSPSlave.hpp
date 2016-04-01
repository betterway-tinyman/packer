#pragma once
#include <samchon/example/interaction/Slave.hpp>

#include <samchon/example/tsp/Scheduler.hpp>

#include <iostream>

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
	 * @brief A slave system solving TSP.
	 * 
	 * @details
	 * \par [Inherited]
	 *		@copydetails slave::ParallelClient 
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class TSPSlave
		: public Slave
	{
	private:
		typedef Slave super;

	public:
		/* ---------------------------------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Construct from ip address of the master.
		 * 		  
		 * @param ip IP address of the master.
		 */
		TSPSlave(const string &ip)
			: super(ip, 37100)
		{
		};
		virtual ~TSPSlave() = default;

	protected:
		/* ---------------------------------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------------------------------- */
		virtual void optimize(shared_ptr<XML> xml, size_t index, size_t size) override
		{
			super::optimize(xml, index, size);

			tsp::Scheduler scheduler;
			scheduler.construct(xml);
			scheduler.optimize();

			cout << scheduler.toString() << endl << endl;
			sendData( make_shared<Invoke>("replyOptimization", scheduler.toXML()) );
		};

	public:
		/* ---------------------------------------------------------------------------------
			MAIN
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Main function.
		 */
		static void main()
		{
			string ip;
			int port;

			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	TSP SOLVER SLAVE" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	ip: ";		cin >> ip;
			cout << endl;

			TSPSlave slave(ip);
			slave.start();
		};
	};
};
};
};