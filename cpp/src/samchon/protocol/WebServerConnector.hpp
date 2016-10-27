#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ServerConnector.hpp>
#include <samchon/protocol/WebCommunicator.hpp>

#include <map>
#include <samchon/library/RWMutex.hpp>
#include <samchon/library/StringUtil.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * A web-server connector.
	 * 
	 * The {@link WebServerConnector} is a type of {@link WebCommunicator}, specified for connecting to remote web-server
	 * who folows web-socket protocol and taking full charge of network communication with the remot web-server.
	 * 
	 * Create an {@link WebServerConnector} instance from a {@IProtocol listener} and call the {@link connect connect()} 
	 * method. Then whenever a replied message comes from the remote system, the message will be converted to an 
	 * {@link Invoke} object and the {@link Invoke} object will be shifted to the {@link IProtocol listener}'s 
	 * {@link IProtocol.replyData IProtocol.replyData()} method. Below code is an example connecting to remote server and 
	 * interacting with it.
	 * 
	 * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-application.ts
	 * 
	 * Note that, {@link WebServerConnector} connects to a remote web-server who follows the web-socket protocol. If the 
	 * remote server is following not web-socket protocol, but protocol of Samchon Framework's own, then use 
	 * {@link ServerConnector} instead.
	 *
	 * Protocol                | Derived Type               | Connect to
	 * ------------------------|----------------------------|-------------------
	 * Samchon Framework's own | {@link ServerConnector}    | {@link Server}
	 * Web-socket protocol     | {@link WebServerConnector} | {@link WebServer}
	 *
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 *
	 * @see {@link WebServer}, {@link IProtocol}
	 * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#serverconnector)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WebServerConnector
		: public ServerConnector,
		public WebCommunicator
	{
	private:
		typedef ServerConnector super;

		static std::map<std::pair<std::string, int>, std::string> s_cookies;
		static library::RWMutex s_mtx;

	public:
		WebServerConnector(IProtocol *listener)
			: super(listener),
			WebCommunicator(false)
		{
		};
		virtual ~WebServerConnector() = default;

		/**
		 * Connect to a web server.
		 * 
		 * Connects to a server with specified *host* address, *port* number and empty *path*. After the connection has
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
		virtual void connect(const std::string &ip, int port) override
		{
			connect(ip, port, "");
		};
		
		/**
		 * Connect to a web server.
		 * 
		 * Connects to a server with specified *host* address, *port* number and *path*. After the connection has
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
		 * @param path Path of service which you want.
		 */
		virtual void connect(const std::string &ip, int port, const std::string &path)
		{
			_Connect(ip, port);

			handshake(ip, port, path);

			listen_message();
		};

	private:
		void handshake(const std::string &ip, int port, const std::string &path)
		{
			///////
			// SEND HEADER
			///////
			// CERTIFICATION KEY
			std::string &base64_key = WebSocketUtil::generate_base64_certification_key();
			std::string &sha1_key = WebSocketUtil::encode_certification_key(base64_key);

			// COOKIE
			std::string cookie;
			{
				library::UniqueReadLock uk(s_mtx);
				auto it = s_cookies.find({ ip, port });

				if (it != s_cookies.end())
					cookie = "Cookie: " + it->second + "\r\n";
			}

			// SEND
			std::string &query = library::StringUtil::substitute
			(
				std::string("") +
				"GET {1} HTTP/1.1\r\n" + //path
				"Host: {2}\r\n" + //ip:port
				"Upgrade: websocket\r\n" +
				"Connection: Upgrade\r\n" +
				"{3}" + // cookie
				"Sec-WebSocket-Key: {4}\r\n" + // hashed certification key
				"Sec-WebSocket-Version: 13\r\n" +
				"\r\n",

				path.empty() ? "/" : "/" + path,
				ip + ":" + std::to_string(port),
				cookie,
				base64_key
			);
			socket->write_some(boost::asio::buffer(query.data(), query.size()));

			///////
			// LISTEN HEADER
			///////
			std::array<unsigned char, 1000> byte_array;
			size_t size = socket->read_some(boost::asio::buffer(byte_array));

			WeakString wstr((const char*)byte_array.data(), size);
			std::string server_sha1 = wstr.between("Sec-WebSocket-Accept: ", "\r\n").str();

			// INSPECT VALIDITY
			if (sha1_key != server_sha1)
				throw std::domain_error("WebSocket handshaking has failed.");

			// SET-COOKIE
			if (wstr.find("Set-Cookie: ") != std::string::npos)
			{
				WeakString set_cookie = wstr.between("Set-Cookie: ", "\r\n");
				library::UniqueWriteLock uk(s_mtx);

				s_cookies[{ip, port}] = set_cookie.str();
			}
		};
	};
};
};