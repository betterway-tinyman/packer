#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Server.hpp>
#include <samchon/protocol/WebClientDriver.hpp>

#include <sstream>
#include <samchon/WeakString.hpp>
#include <samchon/library/StringUtil.hpp>
#include <samchon/protocol/WebSocketUtil.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * A web server.
	 *
	 * The {@link WebServer} is an abstract class providing methods for {@link open opening a web-server} and
	 * {@link WebClientDriver accepting web-clients}. The web-server opened by this {@link WebServer} class follows the 
	 * protocol of web-socket.
	 * 
	 * To open a web-server, extends the {@link WebServer} class and overrides {@link addClient addClient()} methods to 
	 * define what to do with a newly conneted {@link WebClientDriver remote web-clietns}. At last, call {@link open open()}
	 * method with the specified port number.
	 *
	 * Below codes and classes will be good examples for comprehending how to open a server and handle remote clients.
	 *
	 * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
	 * - https://github.com/samchon/framework/blob/master/ts/examples/chat-server/server.ts
	 * - {@link service::Server}
	 * - {@link external::ExternalClientArray}
	 * - {@link slave::SlaveServer}
	 *
	 * Note that, this {@link WebServer} class follows the web-socket protocol, not Samchon Framework's own protocol. If you
	 * want to open a server following the protocol of Samchon Framework's own, then extends {@link Server} instead.
	 *
	 * Protocol                | Derived Type      | Related {@link ClientDriver}
	 * ------------------------|-------------------|------------------------------
	 * Samchon Framework's own | {@link Server}    | {@link ClientDriver}
	 * Web-socket protocol     | {@link WebServer} | {@link WebClientDriver}
	 *
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 *
	 * @see {@link WebClientDriver}
	 * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#server)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WebServer 
		: public virtual Server
	{
	private:
		typedef Server super;

		size_t sequence;

	public:
		/**
		 * Default Constructor.
		 */
		WebServer()
			: super()
		{
			sequence = 0;
		};
		virtual ~WebServer() = default;

	private:
		virtual void handle_connection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) override
		{
			std::array<char, 1000> byte_array;
			boost::system::error_code error;

			///////
			// LISTEN HEADER
			///////
			size_t size = socket->read_some(boost::asio::buffer(byte_array), error);
			if (error)
				return;

			// HEADER FROM CLIENT
			WeakString header(byte_array.data(), size);

			// KEY VALUES
			WeakString path = header.between("", "\r\n").between(" /", " HTTP");
			std::string session_id;
			std::string cookie;

			WeakString encrypted_cert_key;

			// DECODE ENCRYPTED CERTIFICATION KEY
			encrypted_cert_key = header.between("Sec-WebSocket-Key:", "\n").trim();

			if (encrypted_cert_key.find("\r") != std::string::npos)
				encrypted_cert_key = encrypted_cert_key.between("", "\r");

			if (header.find("Set-Cookie: ") != std::string::npos)
			{
				cookie = header.between("Set-Cookie: ", "\r\n");

				size_t session_id_idx = header.find("SESSION_ID=");
				if (session_id_idx == std::string::npos)
				{
					// ISSUE A NEW SESSION_ID AND ADD IT TO ORDINARY COOKIES
					session_id = issue_session_id();
					cookie += "; SESSION_ID=" + session_id;
				}
				else
				{
					// FETCH ORDINARY SESSION_ID
					session_id = header.substr
					(
						session_id_idx + 11,
						std::min
						(
							header.find(";", session_id_idx),
							header.find("\r\n", session_id_idx)
						)
					);
				}
			}
			else
			{
				// NO COOKIE EXISTS
				session_id = issue_session_id();
				cookie = "SESSION_ID=" + session_id;
			}

			///////
			// SEND HEADER
			///////
			// CONSTRUCT REPLY MESSAGE
			std::string &reply_header = library::StringUtil::substitute
			(
				std::string("") +
				"HTTP/1.1 101 Switching Protocols\r\n" +
				"Upgrade: websocket\r\n" +
				"Connection: Upgrade\r\n" +
				"Set-Cookie: {1}\r\n" +
				"Sec-WebSocket-Accept: {2}\r\n" +
				"\r\n",

				cookie,
				WebSocketUtil::encode_certification_key(encrypted_cert_key)
			);

			// SEND
			socket->write_some(boost::asio::buffer(reply_header), error);
			if (error)
				return;

			///////
			// ADD CLIENT
			///////
			// CREATE DRIVER
			std::shared_ptr<WebClientDriver> driver(new WebClientDriver(socket));
			driver->session_id = session_id;
			driver->path = path.str();

			// ADD CLIENT
			addClient(driver);
		};
		
		auto issue_session_id() -> std::string
		{
			static std::uniform_int_distribution<unsigned int> distribution(0, UINT32_MAX);
			static std::random_device device;

			unsigned int port = _Acceptor->local_endpoint().port();
			size_t uid = ++sequence;
			long long linux_time = library::Date().toLinuxTime();
			unsigned int rand = distribution(device);

			std::stringstream ss;
			ss << std::hex << port;
			ss << std::hex << uid;
			ss << std::hex << linux_time;
			ss << std::hex << rand;

			return ss.str();
		};
	};
};
};