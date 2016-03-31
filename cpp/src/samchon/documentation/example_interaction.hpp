#pragma once

namespace samchon
{
	namespace example
	{
		/**
		 * @brief An example of complicate network system.
		 * 
		 * @details
		 * <p> The interaction module is an example solving some problems like TSP and Packer not only in
		 * a computer in a computer but in a network level, with parallel processing system. </p>	   
		 * 		   
		 * <p> Principle purpose of protocol module in Samchon Framework is to constructing complicate 
		 * network system interacting with another external network systems and using master, slave modules 
		 * that can realize (tree-structured) parallel (or distributed) processing system. </p>
		 * 
		 * <p> The example interaction has built for providing guidance for those things. The interaction 
		 * demonstrates howto build complicate netwrok system eailsy by considering each system as a class
		 * of a S/W, within framework of Object-Oriented Design. Of course, interaction module provides a
		 * guidance for using external system and parallel processing system modules, too. </p>
		 * 
		 * <p> You can learn how to construct a network system interacting with external network system and
		 * build (tree-structured) parallel processing systems which are distributing tasks (processes) by
		 * segmentation size by following the example, interaction module. </p>
		 *
		 * <p> @image html  cpp/example_interaction.png
		 *	   @image latex cpp/example_interaction.png </p>
		 * 
		 * <p> @image html  conception/example_interaction_network_diagram.png
		 *	   @image latex conception/example_interaction_network_diagram.png </p>  
		 *					   
		 * <h3> Example Sources - Abstract & Basic classes </h3>
		 * \par Master.hpp
		 *		@includelineno example/interaction/Master.hpp
		 * \par Slave.hpp
		 *		@includelineno example/interaction/Slave.hpp
		 * \par SlaveDriver.hpp
		 *		@includelineno example/interaction/SlaveDriver.hpp
		 * \par ChiefDriver.hpp
		 *		@includelineno example/interaction/ChiefDriver.hpp
		 *					   
		 * <h3> Example Sources - Chief System </h3>
		 * \par Chief.hpp
		 *		@includelineno example/interaction/Chief.hpp
		 * \par MasterDriver.hpp
		 *		@includelineno example/interaction/MasterDriver.hpp
		 *					   
		 * <h3> Example Source - Reporter System </h3>
		 * \par Reporter.hpp
		 *		@includelineno example/interaction/Reporter.hpp
		 *					   
		 * <h3> Example Sources - Packer Systems </h3>
		 * \par PackerMaster.hpp
		 *		@includelineno example/interaction/PackerMaster.hpp
		 * \par PackerMediator.hpp
		 *		@includelineno example/interaction/PackerMediator.hpp
		 * \par PackerSlave.hpp
		 *		@includelineno example/interaction/PackerSlave.hpp
		 *					   
		 * <h3> Example Sources - TSP Systems </h3>
		 * \par TSPMaster.hpp
		 *		@includelineno example/interaction/TSPMaster.hpp
		 * \par TSPSlave.hpp
		 *		@includelineno example/interaction/TSPSlave.hpp
		 * 
		 * @see protocol
		 * @see protocol::master
		 * @see protocol::slave
		 * 
		 * @author Jeongho Nam
		 */
		namespace interaction
		{
			//THIS HEADER FILE IS ONLY FOR DOCUMENTATION
		};
	};
};