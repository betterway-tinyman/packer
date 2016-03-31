#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IClient.hpp>

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
	 * type of network system in Samchon Framework. The basic 3 components are IProtocol, IServer and
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
		: public virtual IClient
	{
	protected:
		typedef IClient super;

		/**
		 * @brief An io_service of Boost.Asio's own.
		 */
		boost::asio::io_service *ioService;

		/**
		 * @brief An endpoint directing a server.
		 */
		EndPoint *endPoint;

		/**
		 * @brief (Optional) An local endpoint of the client (my system)
		 *
		 * @details
		 * <p> An local endpoint fof my system. It binds my ip to custom. It acts when my_ip is
		 * configured. If the my_ip is not configured, does not act. </p>
		 */
		EndPoint *localEndPoint;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		ServerConnector();
		virtual ~ServerConnector();

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Destinatio IP
		 */
		virtual auto getIP() const->std::string = 0;

		/**
		 * @brief Destination port
		 */
		virtual auto getPort() const -> int = 0;

		/**
		 * @brief (optional) My IP, if you want to bind
		 */
		virtual auto getMyIP() const->std::string;

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
		virtual void connect();
	};
};
};