#pragma once

#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An interface of Invoke message chain
	 *
	 * @details 
	 * <p> IProtocol is an interface for Invoke message, which is standard message of network I/O 
	 * in Samchon Framework, chain. The IProtocol interface is used to network drivers and some classes
	 * which are in a relationship of chain of responsibility with those network drivers. </p>
	 *
	 * <p> IProtocol is one of the basic 3 + 1 components that can make any type of network system in
	 * Samchon Framework with Server and IClient. Following the "chain of responsibility" pattern, 
	 * looking around classes in Samchon Framework, you can see all related classes with network I/O 
	 * are implemented from the IProtocol. </p>
	 *
	 * @image html cpp/protocol_interface.png
	 * @image latex cpp/protocol_interface.png
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class IProtocol
	{
	public:
		virtual ~IProtocol() = default;

		/**
		 * @brief Reply a message
		 *
		 * @details Handles a replied Invoke message or shifts the responsibility to related chain.
		 * @param invoke An Invoke message gotten from a network system
		 */
		virtual void replyData(std::shared_ptr<Invoke>) = 0;

		/**
		 * @brief Send a message
		 *
		 * @details Sends Invoke message to a network system or shifts the responsibility to related chain.
		 * @param invoke An Invoke message to send to a network system
		 */
		virtual void sendData(std::shared_ptr<Invoke>) = 0;
	};
};
};