#pragma once
#include <samchon/protocol/service/Service.hpp>

namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			class ListService;

			/**
			 * @brief A list service of chat rooms.
			 *
			 * @details
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ListService
				: public protocol::service::Service
			{
			private:
				typedef protocol::service::Service super;

			public:
				/**
				 * @brief Construct from a client.
				 */
				ListService(protocol::service::Client*);
				virtual ~ListService() = default;

				virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override;

			private:
				/**
				 * @brief Create a chat room.
				 *
				 * @param name Name of the candidate room to create.
				 */
				void createRoom(const std::string &);

				void handleRoomArray(std::shared_ptr<library::XML>);
				void handleMakeRoom(bool);
				void handleRoomArray(bool);
			};
		};
	};
};