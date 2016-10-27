#pragma once

#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * An interface for {@link Invoke} message chain.
	 *
	 * {@link IProtocol} is an interface for {@link Invoke} message, which is standard message of network I/O in
	 * *Samchon Framework*, chain. The {@link IProtocol} interface is used to network drivers and some classes which are
	 * in a relationship of *Chain of Responsibility Pattern* with those network drivers.
	 *
	 * Implements {@link IProtocol} if the class sends and handles {@link Invoke} messages. Looking around source codes of
	 * the *Samchon Framework*, especially *Templates*, you can find out that all the classes and modules handling
	 * {@link Invoke} messages are always implementing this {@link IProtocol}.
	 *
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 *
	 * @see {@link Invoke}
	 * @handbook https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#iprotocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class IProtocol
	{
	public:
		virtual ~IProtocol() = default;

		/**
		 * Handling replied message.
		 * 
		 * Handles a replied {@link Invoke} message or shifts the responsibility to chain.
		 *
		 * @param invoke An {@link Invoke} message has received.
		 */
		virtual void replyData(std::shared_ptr<Invoke>) = 0;

		/**
		 * Send message.
		 * 
		 * Sends an {@link Invoke} message to related system or shifts the responsibility to chain.
		 *
		 * @param invoke Invoke message to send
		 */
		virtual void sendData(std::shared_ptr<Invoke>) = 0;
	};
};
};