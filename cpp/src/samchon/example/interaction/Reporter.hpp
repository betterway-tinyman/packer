#pragma once

#include <samchon/example/interaction/ChiefDriver.hpp>

#include <samchon/example/tsp/Scheduler.hpp>
#include <samchon/example/packer/Packer.hpp>

#include <samchon/library/XML.hpp>
#include <samchon/protocol/Invoke.hpp>

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
	 * @brief A reporter printing optimization result on screen.
	 * 
	 * @details
	 * <p> A reporter system prints optimization results on screen which are gotten from cheif
	 * system. </p>
	 * 
	 * <p> Of course, the optimizatino results came from the Chief system are came from Master
	 * systems and even the Master systems also got those optimization results from those own
	 * Slave systems. </p>
	 *  
	 * <p> The ReportSystem class is built for be helpful for users to comprehend using chain of
	 * responsibility pattern in network level. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails ChiefDriver 
	 *
	 * @author Jeongho Nam
	 */
	class Reporter
		: public ChiefDriver
	{
	private:
		typedef ChiefDriver super;

	public:
		/* ---------------------------------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		Reporter()
			: super(nullptr, 37200)
		{
		};
		virtual ~Reporter() = default;

	protected:
		virtual void addClient(Socket *socket) override
		{
			cout << "The chief has connected." << endl;

			super::addClient(socket);
		};

	public:
		/* ---------------------------------------------------------------------------------
			INVOKE MESSAGE CHAIN
		--------------------------------------------------------------------------------- */
		virtual void replyData(shared_ptr<Invoke> invoke)
		{
			//PRINT
			shared_ptr<XML> &xml = invoke->at(0)->getValueAsXML();

			if (xml->getTag() == "scheduler")
				printTSP(xml);
			else if (xml->getTag() == "packer")
				printPacker(xml);
		};

	protected:
		/**
		 * @brief Print TSP result on screen.
		 * 		  
		 * @param xml XML object representing a Scheduler of TSP.
		 */
		void printTSP(shared_ptr<XML> xml)
		{
			tsp::Scheduler scheduler;
			scheduler.construct(xml);

			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	TSP SOLVER" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			cout << scheduler.toString() << endl << endl;
		};

		/**
		 * @brief Print Packer result on screen.
		 * 
		 * @param xml XML object representing a Packer.
		 */
		void printPacker(shared_ptr<XML> xml)
		{
			packer::Packer packer;
			packer.construct(xml);

			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	PACKER" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			cout << packer.toString() << endl << endl;
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
			string ip;

			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	REPOTER" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			cout << endl;

			Reporter reporter;
			reporter.open();
		};
	};			
};
};
};