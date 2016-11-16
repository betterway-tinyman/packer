#pragma once
#include <samchon/library/CriticalDictionary.hpp>
#include <samchon/protocol/IEntityGroup.hpp>

namespace samchon
{
	namespace library { class XML; };
	namespace protocol { class Invoke; };

	namespace example
	{
		namespace chat_service
		{
			class ChatRoom;
			
			class ChatServer;
			class ChatUser;

			/**
			 * @brief A manager of chat rooms.
			 *
			 * @details
			 * <p> An entity list of chat rooms and a manager of chat rooms taking a responsibility 
			 * of notifying changes of children chat rooms to a related chat server. </p>
			 *
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatRoomArray
				: public CriticalDictionary<std::shared_ptr<ChatRoom>>,
				public protocol::IEntityGroup
			{
			protected:
				typedef CriticalDictionary<std::shared_ptr<ChatRoom>> super;
				
				virtual auto TAG() const -> std::string { return "roomArray"; };
				virtual auto CHILD_TAG() const -> std::string { return "room"; };

			private:
				/**
				 * @brief A chat server that rooms are belonged to.
				 */
				ChatServer *server;
				
			public:
				/* -----------------------------------------------------------------
					CONSTRUCTORS
				----------------------------------------------------------------- */
				/**
				 * @brief Construct from a chat server
				 */
				ChatRoomArray(ChatServer*);
				virtual ~ChatRoomArray() = default;
				
				/* -----------------------------------------------------------------
					NOTIFIER
				----------------------------------------------------------------- */
				/**
				 * @brief Notify changes of chat rooms.
				 * @details Notify changes of chat rooms to all clients in the chat server.
				 */
				void notify();

			private:
				/* -----------------------------------------------------------------
					EXPORTERS
				----------------------------------------------------------------- */
				/**
				 * @brief Get an XML object representing the ChatRoomArray
				 */
				auto toXML() const -> std::shared_ptr<library::XML>;

				/**
				 * @brief Get an Invoke message used to notifying change.
				 */
				auto toInvoke() const -> std::shared_ptr<protocol::Invoke>;
			};
		};
	};
};