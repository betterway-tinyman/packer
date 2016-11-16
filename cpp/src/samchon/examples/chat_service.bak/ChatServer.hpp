#pragma once
#include <samchon/protocol/service/Server.hpp>

namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			class ChatRoomArray;
			class ChatRoom;

			/**
			 * @brief A chat server
			 *
			 * @details
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatServer
				: public protocol::service::Server
			{
			protected:
				typedef protocol::service::Server super;

				virtual auto PORT() const -> int override
				{
					return 37749;
				};

			private:
				/**
				 * @brief A manager of chat rooms
				 */
				ChatRoomArray *roomArray;

			public:
				/**
				 * @brief Default Constructor.
				 */
				ChatServer();
				virtual ~ChatServer();

			protected:
				virtual auto createUser() -> protocol::service::User* override;

			public:
				/**
				 * @brief Get chat room list.
				 */
				auto getChatRoomArray() const -> ChatRoomArray*;
			};
		};
	};
};