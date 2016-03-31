#pragma once
#include <samchon/protocol/IServer.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <set>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
	namespace example
	{
		/**
		 * @brief A chat server
		 * @details A simple chat server operates on a console.
		 *
		 * @image html cpp/example_console_chat.png
		 * @image latex cpp/example_console_chat.png
		 *
		 * @author Jeongho Nam
		 */
		namespace console_chat_server
		{
			class ChatClient;

			/**
			 * @brief A chat server
			 *
			 * @author Jeongho Nam
			 */
			class ChatServer
				: public protocol::IServer,
				public protocol::IProtocol
			{
			private:
				typedef protocol::IServer super;

				/**
				 * @brief List of clients
				 */
				std::set<ChatClient*> clientSet;

				/**
				 * @brief A rw_mutex managing clientSet
				 */
				library::RWMutex rwMutex;

			public:
				/**
				 * @brief Default Constructor
				 */
				ChatServer();
				virtual ~ChatServer() = default;

				/**
				 * @brief Erase client when disconnected
				 */
				void eraseClient(ChatClient *client);

			protected:
				/**
				 * @brief Port number: #33749
				 */
				virtual auto PORT() const -> int override
				{
					return 37749;
				};

				/**
				 * @brief Add a client
				 */
				virtual void addClient(protocol::Socket *socket) override;

			public:
				/**
				 * @brief Handling replied message
				 * @details When got a message from a client, sends the message to all clients.
				 *
				 * @param invoke An invoke message represents a chat.
				 */
				virtual void replyData(std::shared_ptr<protocol::Invoke>) override;

				/**
				 * @brief Sending a message
				 * @details Send a message to all clients. Called by replyData().
				 *
				 * @param invoke An invoke message represents a chat.
				 */
				virtual void sendData(std::shared_ptr<protocol::Invoke>) override;
			};
		};
	};
};