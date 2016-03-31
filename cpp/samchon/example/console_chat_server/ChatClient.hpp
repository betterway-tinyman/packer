#pragma once
#include <samchon/protocol/IClient.hpp>

namespace samchon
{
	namespace example
	{
		namespace console_chat_server
		{
			class ChatServer;

			/**
			 * @brief A chat client
			 * @details A simple chat client belonged to a chat server
			 *
			 * @image html cpp/example_console_chat.png
			 * @image latex cpp/example_console_chat.png
			 *
			 * @author Jeongho Nam
			 */
			class ChatClient
				: public protocol::IClient
			{
			private:
				typedef protocol::IClient super;

				/**
				* @brief A ChatServer that the ChatClient is belonged to
				*/
				ChatServer *server;

			public:
				/**
				 * @brief Construct from a server and socket
				 *
				 * @param server A server the client is belonged to.
				 * @param socket A socket interacting with physical client.
				 */
				ChatClient(ChatServer*, protocol::Socket*);

				/**
				 * @brief Default Destructor
				 * @details Erase from ChatServer::clientSet.
				 */
				virtual ~ChatClient();

				/**
				 * @brief Shift responsibility to related chain
				 * @details Call ChatServer::replyData().
				 */
				virtual void replyData(std::shared_ptr<protocol::Invoke>) override;
			};
		};
	};
};