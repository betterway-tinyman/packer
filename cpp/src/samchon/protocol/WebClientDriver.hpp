#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ClientDriver.hpp>
#include <samchon/protocol/WebCommunicator.hpp>

namespace samchon
{
namespace protocol
{
	class WebServer;

	/**
	 * A communicator with remote web-client.
	 *
	 * The {@link WebClientDriver} class is a type of {@link WebCommunicator}, specified for communication with remote 
	 * web-client who has connected in a {@link WebServer} object who follows the protocol of web-socket. The 
	 * {@link WebClientDriver} takes full charge of network communication with the remote web-client.
	 * 
	 * The {@link WebClientDriver} object is always created by {@link WebServer} class. When you got this 
	 * {@link WebClientDriver} object from the {@link WebServer.addClient WebServer.addClient()}, then specify
	 * {@link IProtocol listener} with the {@link WebClientDriver.listen WebClientDriver.listen()} method. Below code is an 
	 * example specifying and managing the {@link IProtocol listener} objects.
	 *
	 * - https://github.com/samchon/framework/blob/master/ts/examples/calculator/calculator-server.ts
	 *
	 * Protocol                | Derived Type            | Created By
	 * ------------------------|-------------------------|-------------------
	 * Samchon Framework's own | {@link ClientDriver}    | {@link Server}
	 * Web-socket protocol     | {@link WebClientDriver} | {@link WebServer}
	 * 
	 * Unlike other protocol, Web-socket protocol's clients notify two parameters on their connection; 
	 * {@link getSessionID session-id} and {@link getPath path}. The {@link getSessionID session-id} can be used to 
	 * identify *user* of each client, and the {@link getPath path} can be used which type of *service* that client wants.
	 * In {@link service} module, you can see the best utilization case of them.
	 * 
	 * - {@link service.User}: utlization of the {@link getSessionID session-id}.
	 * - {@link service.Service}: utilization of the {@link getPath path}.
	 *
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_basic_components.png)
	 * 
	 * @see {@link WebServer}, {@link IProtocol}
	 * @handbook [Protocol - Basic Components](https://github.com/samchon/framework/wiki/CPP-Protocol-Basic_Components#clientdriver)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WebClientDriver
		: public ClientDriver,
		public WebCommunicator
	{
		friend class WebServer;

	private:
		typedef ClientDriver super;

		std::string session_id;
		std::string path;

	public:
		WebClientDriver(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
			: super(socket),
			WebCommunicator(true)
		{
		};

		virtual ~WebClientDriver() = default;

		/**
		 * Get session ID, an identifier of the remote client.
		 */
		auto getSessionID() const -> std::string
		{
			return session_id;
		};

		/**
		 * Get requested path.
		 */
		auto getPath() const -> std::string
		{
			return path;
		};
	};
};
};