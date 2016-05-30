#pragma once
#include <samchon/API.hpp>

#include <string>
#include <samchon/protocol/Socket.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An interface of a physical server
	 * 
	 * @details 
	 * <p> IServer provides methods for opening a server. </p>
	 *
	 * <p> IServer is one of the basic 3 + 1 components that can make any type of network system in
	 * Samchon Framework with IProtocol and IClient. Looking around classes in Samchon Framework, 
	 * you can see all servers are implemented from the IServer. </p>
	 *
	 * <p> When a client connects to the server, the abstract method IServer::addClient() is called
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
	class SAMCHON_FRAMEWORK_API IServer
	{
	protected:
		/**
		 * @brief An acceptor for clients
		 */
		Acceptor *acceptor;

		/**
		 * @brief (optional) Server's IP
		 */
		virtual auto MY_IP() const -> std::string;

		/**
		 * @brief Port number of the server
		 */
		virtual auto PORT() const -> int = 0;

	public:
		/**
		 * @brief Default Constructor
		 */
		IServer();

		virtual ~IServer();

		/**
		 * @brief Open the server
		 *
		 * @note It holds (monopolies) a thread.
		 */
		virtual void open();

		/**
		 * @brief Close the server
		 */
		virtual void close();

	protected:
		/**
		 * @brief Handling connection of a physical client
		 */
		virtual void addClient(Socket*) = 0; //ADD_CLIENT
	};
};
};