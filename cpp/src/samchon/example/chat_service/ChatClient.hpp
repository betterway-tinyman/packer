#pragma once
#include <samchon/protocol/service/Client.hpp>

namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			/**
			 * @brief A client 
			 *
			 * @details
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatClient
				: public protocol::service::Client
			{
			public:
				/**
				 * @brief Construct from an user.
				 */
				ChatClient(protocol::service::User*);
				virtual ~ChatClient() = default;

			protected:
				auto createService(const std::string &) -> protocol::service::Service* override;
			};
		};
	};
};