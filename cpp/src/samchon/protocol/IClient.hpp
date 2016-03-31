#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#include <samchon/protocol/Socket.hpp>
#include <string>

namespace std
{
	class mutex;
};
namespace samchon
{
	class ByteArray;
};

namespace samchon
{
namespace protocol
{
	class Invoke;

	/**
	 * @brief An interface for a client
	 *
	 * @details
	 * <p> IClient is an interface not only for a physical client, but also a driver for a client in 
	 * a physical server. </p>
	 *
	 * <ul>
	 *	<li> IServer:addClient() </li>
	 *	<ul>
	 *		<li> service::Server -> service::User -> service::Client </li>
	 *		<li> ExternalClientArray (A physical server) -> ExternalClient (A driver of a client) </li>
	 *	</ul>
	 * </ul>
	 *
	 * <p> IClient is one of the basic 3 + 1 components that can make any type of network system in
	 * Samchon Framework with IProtocol and IServer. Looking around classes in Samchon Framework, 
	 * you can see all classes related with client are implemented from the IClient. </p>
	 *
	 * <p> IClient takes a role of real network I/O. IClient sends data to otherside network system
	 * from IProtocol::sendData(). IClient also gets data from otherside network system, generates
	 * standard Invoke message from parsing the replied data and shift the Invoke message to 
	 * IProtocol::replyData(). </p>
	 *
	 * @image html  cpp/protocol_interface.png
	 * @image latex cpp/protocol_interface.png
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API IClient
		: public virtual IProtocol
	{
	protected:
		/**
		 * @brief Socket for network I/O
		 */
		Socket *socket;

		/**
		 * @brief A mutex for sending message
		 */
		std::mutex *sendMtx;

		/**
		 * @brief Buffer size of network I/O
		 */
		virtual auto BUFFER_SIZE() const->size_t;

	private:


	public:
		/**
		 * @brief Default Constructor
		 */
		IClient();
		virtual ~IClient();

		/**
		 * @brief Listens message from a related system
		 *
		 * @note It holds (monopolies) a thread.
		 */
		virtual void listen();

		/**
		 * @brief Sends message to a related system
		 *
		 * @param invoke Invoke message to send
		 */
		virtual void sendData(std::shared_ptr<Invoke>);

	protected:
		/**
		 * @brief A method for pre-processing replied Invoke message
		 *
		 * @details
		 * <p> You can realize a pre-processing about the replied Invoke message by overriding the
		 * IClient::_replyData(). </p>

		 * <p> Archiving a history log, filtering and truncating, let handling the message with a new
		 * thread, calculating performance by estimating elapsed time about the Invoke message, and so on.
		 * Anything you can define pre-processing works before IProtocol::replyData().
		 *	\li Archive history log -> cloud service
		 *	\li Estimating performance -> distributed and parallel processing system
		 *
		 * @param invoke An invoke message to be pre-processed and shifted to IProtocol::replyData().
		 */
		virtual void _replyData(std::shared_ptr<Invoke>);

	private:
		void handleString(ByteArray &, std::string &, std::shared_ptr<Invoke> &, size_t);
		void handleBinary(ByteArray &, std::string &, std::shared_ptr<Invoke> &, size_t);
	};
};
};