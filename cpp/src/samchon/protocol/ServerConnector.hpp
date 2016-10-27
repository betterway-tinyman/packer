#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Communicator.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * A server connector.
	 * 
	 * The {@link ServerConnector} class is a type of {@link Communicator}, specified for connecting to remote server who 
	 * follows Samchon Framework's own protocol and taking full charge of network communication with the remote server. 
	 * 
	 * Create a {@link ServerConnector} instance from a {@IProtocol listener} and call the {@link connect connect()} method.
	 * Then whenever a replied message comes from the remote system, the message will be converted to an {@link Invoke} 
	 * object and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s {@link IProtocol.replyData} 
	 * method. Below code is an example connecting to remote server and interacting with it.
	 * 
	 * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-application.ts
	 * 
	 * Note that, {@link ServerConnector} connects to a remote server who follows the protocol of Samchon Framework's own. 
	 * If the remote server is following web-socket protocol, then use {@link WebServerConnector} instead.
	 * 
	 * Protocol                | Derived Type               | Connect to
	 * ------------------------|----------------------------|-------------------
	 * Samchon Framework's own | {@link ServerConnector}    | {@link Server}
	 * Web-socket protocol     | {@link WebServerConnector} | {@link WebServer}
	 * 
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 * 
	 * @see {@link Server}, {@link IProtocol}
	 * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#serverconnector)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class ServerConnector
		: public virtual Communicator
	{
	protected:
		std::unique_ptr<boost::asio::io_service> io_service;

		std::unique_ptr<boost::asio::ip::tcp::endpoint> endpoint;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Construct from *listener*.
		 * 
		 * @param listener A listener object to listen replied message from newly connected client in
		 *				   {@link IProtocol.replyData replyData()} as an {@link Invoke} object.
		 */
		ServerConnector(IProtocol *listener)
		{
			this->listener = listener;
		};
		
		/**
		 * Default Destructor.
		 */
		virtual ~ServerConnector() = default;

		/* -----------------------------------------------------------
			CONNECTOR
		----------------------------------------------------------- */
		/**
		 * Connect to a server.
		 * 
		 * Connects to a server with specified *host* address and *port* number. After the connection has
		 * succeeded, callback function {@link onConnect} is called. Listening data from the connected server also begins.
		 * Replied messages from the connected server will be converted to {@link Invoke} classes and will be shifted to
		 * the {@link WebCommunicator.listener listener}'s {@link IProtocol.replyData replyData()} method.
		 * 
		 * If the connection fails immediately, either an event is dispatched or an exception is thrown: an error 
		 * event is dispatched if a host was specified, and an exception is thrown if no host was specified. Otherwise, 
		 * the status of the connection is reported by an event. If the socket is already connected, the existing 
		 * connection is closed first.
		 * 
		 * @param ip The name or IP address of the host to connect to. 
		 *			 If no host is specified, the host that is contacted is the host where the calling file resides. 
		 *			 If you do not specify a host, use an event listener to determine whether the connection was 
		 *			 successful.
		 * @param port The port number to connect to.
		 */
		virtual void connect(const std::string &ip, int port)
		{
			_Connect(ip, port);

			listen_message();
		};

	protected:
		void _Connect(const std::string &ip, int port)
		{
			if (socket != nullptr && socket->is_open() == true)
				throw std::logic_error("Already connected");

			io_service.reset(new boost::asio::io_service());
			endpoint.reset(new boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));

			socket.reset(new boost::asio::ip::tcp::socket(*io_service, boost::asio::ip::tcp::v4()));
			socket->connect(*endpoint);
		};
	};
};
};