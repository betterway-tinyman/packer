#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <samchon/protocol/ExternalSystemRole.hpp>
#include <samchon/protocol/IClient.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief A network driver for an external system.
	 *
	 * @details
	 * <p> ExternalSystem is a boundary class interacting with an external system by network communication.
	 * Also, ExternalSystem is an abstract class that a network role, which one is server and which one is 
	 * client, is not determined yet. </p>
	 *
	 * <p> The ExternalSystem has ExternalSystemRole(s) groupped methods, handling Invoke message
	 * interacting with the external system, by subject or unit of a moudle. The ExternalSystemRole is 
	 * categorized in a 'control'. </p>
	 *
	 * <p> @image html  cpp/protocol_external_system.png
	 *	   @image latex cpp/protocol_external_system.png </p>
	 *	   		  
	 * \par Example Sources
	 *	\li example::interaction
	 *
	 * @note
	 * <p> The ExternalSystem class takes a role of interaction with external system in network level.
	 * However, within a framework of Samchon Framework, a boundary class like the ExternalSystem is
	 * not such important. You can find some evidence in a relationship between ExternalSystemArray,
	 * ExternalSystem and ExternalSystemRole. </p>
	 *
	 * <p> Of course, the ExternalSystemRole is belonged to an ExternalSystem. However, if you 
	 * access an ExternalSystemRole from an ExternalSystemArray directly, not passing by a belonged
	 * ExternalSystem, and send an Invoke message even you're not knowing which ExternalSystem is
	 * related in, it's called "Proxy pattern".
	 *
	 * <p> Like the explanation of "Proxy pattern", you can utilize an ExternalSystemRole as a proxy
	 * of an ExternalSystem. With the pattern, you can only concentrate on ExternalSystemRole itself, 
	 * what to do with Invoke message, irrespective of the ExternalSystemRole is belonged to which 
	 * ExternalSystem. </p>
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API ExternalSystem
		: public SharedEntityArray<ExternalSystemRole>,
		public virtual IClient
	{
	protected:
		typedef SharedEntityArray<ExternalSystemRole> super;

		/**
		 * @brief A name can identify an external system.
		 *
		 * @details The name must be unique in ExternalSystemArray.
		 */
		std::string name;

		/**
		 * @brief An ip address of an external system.
		 */
		std::string ip;

		/**
		 * @brief A port number of an external system.
		 */
		int port;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		* @brief Default Constructor
		*/
		ExternalSystem();
		virtual ~ExternalSystem() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/**
		 * @brief Start interaction
		 *
		 * @details
		 * <p> An abstract method starting interaction with an external system. </p>
		 *
		 * <p> If an external systems are a server, starts connection and listening Inovoke message,
		 * else clients, just starts listening only. You also can addict your own procudures of starting
		 * the driver, but if you directly override method of abstract ExternalSystem, be careful about
		 * virtual inheritance. </p>
		 */
		virtual void start() = 0;

		/* ------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------ */
		virtual auto key() const->std::string override;

		/* ------------------------------------------------------------------
			CHAIN OF INVOKE MESSAGE
		------------------------------------------------------------------ */
		/**
		 * @brief Handling replied message from an external system
		 * @details If there's a related ExternalSystemRole, shifts responsibility to the ExternalSystemRole.
		 *
		 * @param invoke Replied Invoke message
		 */
		virtual void replyData(std::shared_ptr<Invoke>) override;

	public:
		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto TAG() const->std::string override;
		virtual auto CHILD_TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};