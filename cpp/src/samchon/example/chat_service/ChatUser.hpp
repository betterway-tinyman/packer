#pragma once
#include <samchon/protocol/service/User.hpp>

namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			/**
			 * @brief An user in chat server.
			 *
			 * @details
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatUser
				: public protocol::service::User
			{
			public:
				/**
				 * @brief Construct from server
				 */
				ChatUser(protocol::service::Server*);
				virtual ~ChatUser() = default;

			protected:
				virtual auto createClient() -> protocol::service::Client* override;
			};
		};
	};
};