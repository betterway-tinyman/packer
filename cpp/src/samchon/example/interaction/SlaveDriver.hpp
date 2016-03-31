#pragma once
#include <samchon/protocol/master/ParallelClient.hpp>
#include <samchon/protocol/master/ParallelSystemArray.hpp>

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
	 * @brief A slave system's driver for optimization.
	 * 
	 * @details
	 * <p> SlaveDriver is a boundary class interacting with a slave system by inheriting 
	 * master::ParallelClient. The SlaveDriver is built for providing a guidance of utilizing slave
	 * driver in master module of protocol. </p>
	 * 		   
	 * <p> @image html  cpp/example_interaction.png
	 *	   @image latex cpp/example_interaction.png </p>
	 * 
	 * <p> @image html  conception/example_interaction_network_diagram.png
	 *	   @image latex conception/example_interaction_network_diagram.png </p>
	 *	   		  
	 * \par [Inherited]
	 *		@copydoc master::ParallelClient
	 * 
	 * @author Jeongho Nam
	 */
	class SlaveDriver
		: public virtual master::ParallelClient
	{
	private:
		typedef master::ParallelClient super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		SlaveDriver()
			: super()
		{
		};
		virtual ~SlaveDriver() = default;

		virtual void replyData(std::shared_ptr<Invoke> invoke) override
		{
			systemArray->replyData(invoke);
		};
	};			
};
};
};