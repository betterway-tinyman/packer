#pragma once
#include <set>
#include <string>

#include <samchon/protocol/Entity.hpp>

namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			/**
			 * @brief A chat message
			 *
			 * @details
			 * <p> An entity class representing a chat message. </p>
			 *
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatMessage
				: public protocol::Entity
			{
			protected:
				typedef protocol::Entity super;

				virtual auto TAG() const -> std::string
				{
					return "message";
				};

			private:
				/**
				 * @brief An orator, who told.
				 */
				std::string orator;

				/**
				 * @brief A listener, target of whispering.
				 * @details If there's not a listener to whisper, the value is "ALL".
				 */
				std::string listener;

				/**
				 * @brief A message, what to say.
				 */
				std::string message;

			public:
				/* -----------------------------------------------------------
					CONSTRUCTORS
				----------------------------------------------------------- */
				/**
				 * @brief Default Constructor
				 */
				ChatMessage();
				virtual ~ChatMessage() = default;

				virtual void construct(std::shared_ptr<library::XML>);

				/* -----------------------------------------------------------
					GETTERS
				----------------------------------------------------------- */
				/**
				 * @brief Get listener
				 * @details Used to determine which (physical) client will get the message.
				 */
				auto getListener() const -> std::string;

				virtual auto toXML() const -> std::shared_ptr<library::XML>;
			};
		};
	};
};