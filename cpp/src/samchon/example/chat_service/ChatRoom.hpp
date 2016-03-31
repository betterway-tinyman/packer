#pragma once
#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <string>
#include <samchon/library/CriticalSet.hpp>


namespace samchon
{
	namespace example
	{
		namespace chat_service
		{
			class ChatUser;
			class ChatService;

			class ChatRoomArray;

			/**
			 * @brief A chat room
			 *
			 * @details
			 * <p> An entity class representing a chat room containing participants. 
			 * The ChatRoom class takes a responsibility of Invoke message chain between
			 * clients (participating ChatUser(s)) objects. </p>
			 *
			 * <p> @image html cpp/example_chat_service.png
			 * @image latex cpp/example_chat_service.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class ChatRoom
				: public protocol::Entity,
				public protocol::IProtocol
			{
			protected:
				typedef protocol::Entity super;

				virtual auto TAG() const -> std::string { return "room"; };

			private:
				/**
				 * @brief All rooms that the room is belonged to.
				 */
				ChatRoomArray *roomArray;

				/**
				 * @brief Name of the room.
				 * @details The name is the identifier of the ChatRoom.
				 */
				std::string name;

				/**
				 * @brief A host, who established the room.
				 */
				ChatUser *host;

				/**
				 * @brief Participants, ChatUser(s) in the room
				 */
				library::CriticalSet<ChatService*> participants;

			public:
				/* -----------------------------------------------------------
					CONSTRUCTORS
				----------------------------------------------------------- */
				/**
				 * @brief Construct from rooms, name and host
				 *
				 * @param rooms Rooms managing ChatRoom(s).
				 * @param name Name of the room, the identifier.
				 * @param host A host who established the room.
				 */
				ChatRoom(ChatRoomArray*, const std::string &, ChatUser*);
				virtual ~ChatRoom() = default;
				
				/**
				 * @brief Register a participant client.
				 * @details Registers a participant and notify the participation to another clients.
				 */
				void registerClient(ChatService*);

				/**
				 * @brief Erase a participant client.
				 * @details Erases a participant and notify others about the exiting.
				 */
				void eraseClient(ChatService*);

				/* -----------------------------------------------------------
					CHAIN OF RESPONSIBILITY
				----------------------------------------------------------- */
				virtual void replyData(std::shared_ptr<protocol::Invoke>) override;
				virtual void sendData(std::shared_ptr<protocol::Invoke>) override;	

			public:
				/* -----------------------------------------------------------
					GETTERS
				----------------------------------------------------------- */
				virtual auto toXML() const -> std::shared_ptr<library::XML> override;
			};
		};
	};
};