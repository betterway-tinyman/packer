#pragma once
#include <samchon/API.hpp>

#include <samchon/SmartPointer.hpp>

namespace samchon
{
namespace protocol
{
namespace service
{
	class User;
	class Client;

	/**
	 * @brief A keeper blocking destrunction of User and Client
	 *
	 * @details
	 * <p> Keeps User, Client and Service objects until termination of a stack. </p>
	 * <p> You can prevent the object to be destructed until the method is in a process.
	 * User, Client and Service provides a macro instruction for it. </p>
	 *
	 * @image html  cpp/protocol_service.png
	 * @image latex cpp/protocol_service.png
	 *
	 * @note Defined macros
	 *	\li KEEP_USER_ALIVE
	 *	\li KEEP_CLIENT_ALIVE
	 *	\li KEEP_SERVICE_ALIVE
	 *
	 * @see samchon::protocol
	 * @see samchon::protocol::service
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API ServiceKeeper
	{
	private:
		/**
		 * @brief A shared pionter of an User
		 */
		SmartPointer<User> user;

		/**
		 * @brief A shared pointer of a Client
		 */
		SmartPointer<Client> client;

	public:
		/**
		 * @brief Construct from User and Client
		 *
		 * @brief user An User to keep
		 * @brief client A Client to keep
		 */
		ServiceKeeper(const User *user, const Client *client);

		/**
		 * @brief Copy Constructor
		 */
		ServiceKeeper(const ServiceKeeper&);

		/**
		 * @brief Movie Constructor
		 */
		ServiceKeeper(ServiceKeeper&&);
	};
};
};
};