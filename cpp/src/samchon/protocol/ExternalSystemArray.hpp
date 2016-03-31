#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <samchon/protocol/ExternalSystem.hpp>
#include <samchon/protocol/IProtocol.hpp>

namespace samchon
{
namespace protocol
{
	class ExternalSystemRole;
		
	/**
	 * @brief An array of ExternalSystem(s).
	 *
	 * @details
	 * <p> ExternalSystemArray is an abstract class containing and managing external system drivers. </p>
	 *
	 * <p> Also, ExternalSystemArray can access to ExternalSystemRole(s) directly. With the method, you
	 * can use an ExternalSystemRole as "logical proxy" of an ExternalSystem. Of course, the 
	 * ExternalSystemRole is belonged to an ExternalSystem. However, if you access an ExternalSystemRole 
	 * from an ExternalSystemArray directly, not passing by a belonged ExternalSystem, and send an Invoke 
	 * message even you're not knowing which ExternalSystem is related in, the ExternalSystemRole acted 
	 * a role of proxy. </p>
	 *
	 * <p> It's called as "Proxy pattern". With the pattern, you can only concentrate on 
	 * ExternalSystemRole itself, what to do with Invoke message, irrespective of the ExternalSystemRole 
	 * is belonged to which ExternalSystem. </p>
	 *
	 * @code
	 * ExternalSystemArray::getRole("something")->sendData(invoke);
	 * @endcode
	 * 
	 * <p> @image html  cpp/protocol_external_system.png
	 *	   @image latex cpp/protocol_external_system.png </p>
	 *
	 * \par Example Sources
	 *	\li example::interaction
	 * 
	 * @see samchon::protocol
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API ExternalSystemArray
		: public SharedEntityArray<ExternalSystem>,
		public virtual IProtocol
	{
	protected:
		typedef SharedEntityArray<ExternalSystem> super;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		* @brief Default Constructor
		*/
		ExternalSystemArray();
		virtual ~ExternalSystemArray() = default;

		/**
		 * @brief Start interaction
		 *
		 * @details
		 * <p> An abstract method starting interaction with external systems. </p>
		 *
		 * <p> If external systems are servers, starts connection to them, else clients, opens a server
		 * and accepts the external systems. You can addict your own procudures of starting drivers, but
		 * if you directly override method of abstract ExternalSystemArray, be careful about virtual
		 * inheritance. </p>
		 *
		 *	\li DistributedClientArray::start() calls DistributedSystemArray::start() and ExternalClientArray:start()
		 */
		virtual void start() = 0;

		/* ------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------ */
		/**
		 * @brief Test whether has a role
		 *
		 * @param name Name of an ExternalSystemRole
		 * @return Whether has or not
		 */
		auto hasRole(const std::string&) const -> bool;

		/**
		 * @brief Get a role
		 *
		 * @param name Name of an ExternalSystemRole
		 * @return A shared pointer of specialized role
		 */
		auto getRole(const std::string&) const->std::shared_ptr<ExternalSystemRole>;

		/* ------------------------------------------------------------------
			CHAIN OF INVOKE MESSAGE
		------------------------------------------------------------------ */
		/**
		 * @brief Sends an Invoke message to related ExternalSystem(s).
		 *
		 * @details
		 * <p> If listener of the Invoke message is registered on some ExternalSystemRole(s), sends the
		 * Invoke message to related ExternalSystem(s) (containing the ExternalSystemRole(s)). If cannot
		 * find any ExternalSystemRole, sends the Invoke message to all ExternalSystem(s) belongs to
		 * the ExternalSystemArray. </p>
		 *
		 * @param invoke Invoke message to send
		 */
		virtual void sendData(std::shared_ptr<Invoke>) override;

		/**
		 * @brief Handle replied Invoke message
		 *
		 * @details
		 * <p> Nothing is defined in the ExternalSystemArray::replyData(). It's only for avoid the
		 * method to have a status "not overriden, abstract". </p>
		 *
		 * @param invoke Invoke message to send
		 */
		virtual void replyData(std::shared_ptr<Invoke>) override;

		/* ------------------------------------------------------------------
			XML TAG
		------------------------------------------------------------------ */
		virtual auto TAG() const->std::string override;
		virtual auto CHILD_TAG() const->std::string override;
	};
};
};