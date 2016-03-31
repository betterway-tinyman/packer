#pragma once
#include <samchon/protocol/ExternalServer.hpp>

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
	* @brief A driver for each system, master.
	* 		  
	* @details
	* <p> MasterDriver is a boundary class interacting with a master system which is one of them; 
	* PackerMaster, TSPMaster and Reporter. The MasterDriver classes are belonged to a Chief 
	* logically and real systems associated with the MasterDriver classes are belonged to 
	* a Cheif physically. </p>
	* 
	* <p> @image html  cpp/example_interaction.png
	*	   @image latex cpp/example_interaction.png </p>
	* 
	* <p> @image html  conception/example_interaction_network_diagram.png
	*	   @image latex conception/example_interaction_network_diagram.png </p>  
	* 
	* \par [Inherited]
	*		@copydoc ExternalServer 
	*
	* @author Jeongho Nam
	*/
	class MasterDriver
		: public protocol::ExternalServer
	{
	private:
		typedef protocol::ExternalServer super;

		/**
		* @brief A chief, containing the driver object.
		* 		  
		* @details A Chief instance belonging the MasterDriver object.
		*/
		IProtocol *chief;

	public:
		/**
		 * @brief Construct from parent(master), name, ip and port.
		 * 
		 * @param chief A Chief object that the MasterDriver is belonged to.
		 * @param name Name of the system the driver is connected to.
		 * @param ip IP address of the slave system.
		 * @param port Port number of the slave system.
		 */
		MasterDriver(IProtocol *chief, const string &name, const string &ip, int port)
			: super()
		{
			this->chief = chief;
			this->name = name;

			this->ip = ip;
			this->port = port;
		};
		virtual ~MasterDriver() = default;

	protected:
		virtual auto createChild(shared_ptr<XML>) -> ExternalSystemRole* override
		{
			return nullptr;
		};

	public:
		virtual void replyData(shared_ptr<Invoke> invoke) override
		{
			chief->replyData(invoke);
		};
	};			
};
};
};