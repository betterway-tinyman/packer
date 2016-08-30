#pragma once
#include <samchon/API.hpp>

#include <memory>
#include <samchon/protocol/Socket.hpp>

namespace samchon
{
namespace protocol
{
	class ClientDriver;

	/**
	 * @brief An interface of a physical server
	 * 
	 * @details 
	 * <p> Server provides methods for opening a server. </p>
	 *
	 * <p> Server is one of the basic 3 + 1 components that can make any type of network system in
	 * Samchon Framework with IProtocol and IClient. Looking around classes in Samchon Framework, 
	 * you can see all servers are implemented from the Server. </p>
	 *
	 * <p> When a client connects to the server, the abstract method Server::addClient() is called
	 * with a new thread. If you want to accept only a client at a time, use OneToOneServer instead. </p>
	 *
	 * @image html  cpp/protocol_interface.png
	 * @image latex cpp/protocol_interface.png
	 *
	 * <h3>Example source</h3>
	 * <p> A simple chat server running on console </p>
	 *
	 *	\par example/console_chat_server/ChatServer.hpp
	 *		@includelineno example/console_chat_server/ChatServer.hpp
	 *	\par example/console_chat_server/ChatServer.cpp
	 *		@includelineno example/console_chat_server/ChatServer.cpp
	 *	\par example/console_chat_server/ChatClient.hpp
	 *		@includelineno example/console_chat_server/ChatClient.hpp
	 *	\par example/console_chat_server/ChatClient.cpp
	 *		@includelineno example/console_chat_server/ChatClient.cpp
	 *	\par example/console_chat_server/main.cpp
	 *		@includelineno example/console_chat_server/main.cpp
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API Server
	{
	protected:
		std::unique_ptr<Acceptor> acceptor;

	public:
		Server();
		virtual ~Server();

		/**
		 * @brief Open the server
		 *
		 * @note It holds (monopolies) a thread.
		 */
		virtual void open(int port);

		/**
		 * @brief Close the server
		 */
		virtual void close();

	protected:
		virtual void handleConnection(std::shared_ptr<Socket> socket);

		/**
		 * @brief Handling connection of a physical client
		 */
		virtual void addClient(std::shared_ptr<ClientDriver>) = 0; //ADD_CLIENT
	};
};
};