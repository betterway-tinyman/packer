#pragma once
#include <samchon/protocol/service/Service.hpp>

namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			class ChatRoom;

			/**
			 * @brief A chat service.
			 * @details A chat service about a chat room.
			 *
			 * @details
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatService
				: public protocol::service::Service
			{
			private:
				typedef protocol::service::Service super;

				/**
				 * @brief A chat room
				 * @details A chat room that related client is participating in.
				 */
				ChatRoom *room;

			public:
				/**
				 * @brief Construct from a client.
				 */
				ChatService(protocol::service::Client*);
				virtual ~ChatService();

				virtual void replyData(std::shared_ptr<protocol::Invoke>) override;
			};
		};
	};
};