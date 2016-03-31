#pragma once
#include <samchon/protocol/ExternalServerArray.hpp>
#include <samchon/example/interaction/MasterDriver.hpp>

#include <samchon/protocol/Invoke.hpp>

#include <samchon/example/packer/Packer.hpp>
#include <samchon/example/tsp/Scheduler.hpp>

#include <thread>

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
	 * @brief A chief system managing master systems.
	 * 		  
	 * @details
	 * <p> A chief system manages master systems. The chief system orders optimization processes
	 * to each master system and get reports of the optimization results from those master systems. </p>
	 * 		  
	 * <p> The chief system and Chief class is built for providing a guidance for external system
	 * module. You can study how to intergrate a system with external network system following the
	 * example, the chief system and Chief class. </p>
	 *
	 * <p> @image html  cpp/example_interaction.png
	 *	   @image latex cpp/example_interaction.png </p>
	 * 
	 * <p> @image html  conception/example_interaction_network_diagram.png
	 *	   @image latex conception/example_interaction_network_diagram.png </p>  
	 *	   		  
	 * @details
	 * \par [Inherited]
	 *		@copydoc ExternalServerArray
	 * 		  
	 * @author Jeongho Nam
	 */
	class Chief
		: public ExternalServerArray
	{
	private:
		typedef ExternalServerArray super;

	public:
		/* ---------------------------------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------------------------------- */
		/**
		 * @brief Construct from ip address.
		 * 		  
		 * @param ip IP address of master systems.
		 */
		Chief(const string &ip)
		{
			emplace_back(new MasterDriver(this, "TSP", "127.0.0.1", 37110));
			emplace_back(new MasterDriver(this, "Reporter", ip, 37200));
			emplace_back(new MasterDriver(this, "Packer", ip, 37310));
		};
		virtual ~Chief() = default;

		virtual void start()
		{
			thread([this]()
			{
				super::start();
			}).detach();

			while (true)
			{
				int no;

				cout << "1. TSP Solver, 2. Packer: ";
				cin >> no;

				if (no == 1)
					callTSP();
				else if (no == 2)
					callPacker();
				else
					continue;
			}
		};

	protected:
		virtual auto createChild(shared_ptr<XML>) -> ExternalSystem*
		{
			return new MasterDriver(this, "", "", 0);
		};

	public:
		/* ---------------------------------------------------------------------------------
			INVOKE MESSATE CHAIN
		--------------------------------------------------------------------------------- */
		virtual void replyData(shared_ptr<Invoke> invoke)
		{
			get("Reporter")->sendData(invoke);
		};

	private:
		/**
		 * @brief Call packer master system.
		 * 		  
		 * @details 
		 * <p> Sends a message to packer system for ordering find the best solution of packaging 
		 * with its basic data. The packer system who got the ordering message will find and reply 
		 * the best optimization result about the basic data. </p>
		 */
		void callPacker()
		{
			// CONSTRUCT PRODUCTS
			shared_ptr<packer::ProductArray> productArray(new packer::ProductArray());
			
			productArray->emplace_back( new packer::Product("Eraser", 500, 10, 70) );
			productArray->emplace_back( new packer::Product("Pencil", 400, 30, 35) );
			productArray->emplace_back( new packer::Product("Pencil", 400, 30, 35) );
			productArray->emplace_back( new packer::Product("Pencil", 400, 30, 35) );
			productArray->emplace_back( new packer::Product("Book", 8000, 150, 300) );
			productArray->emplace_back( new packer::Product("Book", 8000, 150, 300) );
			productArray->emplace_back( new packer::Product("Drink", 1000, 75, 250) );
			productArray->emplace_back( new packer::Product("Umbrella", 4000, 200, 1000) );
			productArray->emplace_back( new packer::Product("Notebook-PC", 800000, 150, 850) );
			productArray->emplace_back( new packer::Product("Tablet-PC", 600000, 120, 450) );

			// CONSTRUCT PACKER
			packer::Packer packer(productArray);
			
			packer.emplace_back( new packer::WrapperArray("Large", 100, 200, 1000) );
			packer.emplace_back( new packer::WrapperArray("Medium", 70, 150, 500) );
			packer.emplace_back( new packer::WrapperArray("Small", 50, 100, 250) );

			// SEND
			get("Packer")->sendData
			(
				make_shared<Invoke>
				(
					"optimize", 
					packer.toXML()
				)
			);
		};

		/**
		 * @brief Call tsp master system.
		 * 		  
		 * @details 
		 * <p> Sends a message to tsp system for ordering find the best solution of tsp 
		 * with its basic data. The tsp system who got the ordering message will find and reply 
		 * the best optimization result about the basic data. </p>
		 */
		void callTSP()
		{
			shared_ptr<tsp::Travel> travel = make_shared<tsp::Travel>();
			for(int i = 0; i < 20; i++)
				travel->emplace_back(new tsp::GeometryPoint(i + 1));

			// GENETIC ALGORITHM
			struct tsp::GAParameters gaParameters = {.03, 30, 400, 400};

			// SEND
			tsp::Scheduler scheduler(travel, gaParameters);
			get("TSP")->sendData(make_shared<Invoke>("optimize", scheduler.toXML()));
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

			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	CHIEF" << endl;
			cout << "----------------------------------------------------------------------------" << endl;
			cout << "	ip: ";	cin >> ip;

			Chief cheif(ip);
			cheif.start();
		};
	};			
};
};
};