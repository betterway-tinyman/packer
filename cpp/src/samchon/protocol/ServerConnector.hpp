#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Communicator.hpp>

#include <string>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief A server connector for a physical client.
	 *
	 * @details
	 * <p> ServerConnector is a class for a physical client connecting a server. If you want to connect 
	 * to a server,  then implements this ServerConnector and just override some methods like 
	 * getIP(), getPort() and replyData(). That's all. </p>
	 *
	 * <p> In Samchon Framework, package protocol, There are basic 3 + 1 components that can make any 
	 * type of network system in Samchon Framework. The basic 3 components are IProtocol, Server and
	 * IClient. The last, surplus one is the ServerConnector. Looking around classes in 
	 * Samchon Framework, especially module master and slave which are designed for realizing 
	 * distributed processing systems and parallel processing systems, physical client classes are all 
	 * derived from this ServerConnector. </p>
	 *
	 * @image html cpp/protocol_interface.png
	 * @image latex cpp/protocol_interface.png
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API ServerConnector
		: public virtual Communicator
	{
	protected:
		/**
		 * @brief An io_service of Boost.Asio's own.
		 */
		std::unique_ptr<boost::asio::io_service> io_service;

		/**
		 * @brief An endpoint directing a server.
		 */
		std::unique_ptr<EndPoint> endpoint;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		ServerConnector(IProtocol *listener);
		virtual ~ServerConnector();

		/* -----------------------------------------------------------
			CONNECTOR
		----------------------------------------------------------- */
		/**
		 * @brief Connect to a server
		 *
		 * @details
		 * <p> Connects to a server with configured ip address and port number. After connection,
		 * listens data replied from the server (IClient::listen()). </p>
		 *
		 * @note It monopolies a thread
		 */
		virtual void connect(const std::string &ip, int port);
	};
};
};