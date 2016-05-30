#pragma once
#include <samchon/API.hpp>

#include <set>
#include <string>

#include <samchon/protocol/Socket.hpp>

namespace samchon
{
namespace protocol
{
namespace service
{
	class Server;
	class User;

	/**
	 * @brief Relationship between IP address and User for issuing session
	 *
	 * @details
	 * <p> IPUserPair is a class representing 1:N relationship between IP address and User. </p>
	 * <p> The key role of IPUserPair is to issuing session ID, that is published to User(s),
	 * within a critical section of an IP address. </p>
	 *
	 * @image html  cpp/protocol_service.png
	 * @image latex cpp/protocol_service.png
	 *
	 * @note
	 * <p> Samchon Framework is for Cloud Server basically, so I've adopted Session ID to
	 * discriminate unique User to follow standard. But I think there can be another good strategies
	 * something like a using MAC address instead of publishing Session-ID to discriminate
	 * uniqueness of User. It's okay to adapt those strategies and rebuild my framework's source to
	 * be suitable for those. </p>
	 *
	 * @warning
	 * <p> When changing the strategy, you've to careful about policy_ of each system or libraries. </p>
	 * <p> Followings are something to consider when adjusting MAC address for uniqueness. </p>
	 *
	 *	\li In server side and view of socket, boost::asio or candidate network i/o libary of stl
	 *		does not allow accessing otherside system's MAC address.
	 *		But basic socket of window or linux are allowed.
	 *	\li In client side and view of browser, IE allows to get MAC address by javascript with ActiveX,
	 *		but other browsers do not allow.
	 *
	 * @see samchon::protocol
	 * @see samchon::protocol::service
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API IPUserPair
	{
		friend class Server;
		friend class User;

	private:
		/**
		 * @brief The Server containing IP addresses and User(s)
		 */
		Server *server;

		/**
		 * @brief An IP address
		 */
		std::string ip;

		/**
		 * @brief User(s) belongs to the IP address
		 */
		std::set<const User*> userSet;

	private:
		/**
		 * @brief Construct from Server and IP address
		 */
		IPUserPair(Server *server, const std::string &);

		/**
		 * @brief Get a session ID.
		 *
		 * @details
		 * <p> Asks Whether client to have a session id stored in cookie. </p>
		 *
		 * <p> If client has, get the session id from client, but if not, issues a
		 * new session ID and notify it(enrolled new session ID) to client. Additionally,
		 * if the session id was gotten from a client, inspect validate of the session ID. </p>
		 *
		 * <p> Those tasks will hold a critical section of a global IP address </p>
		 *
		 * @warning The session id must be unique
		 * @return A new session ID that can ensure uniqueness
		 */
		auto getSessionID(Socket*, size_t)->std::string;

		/**
		 * @brief Issue a session ID
		 *
		 * @details
		 * <p>Issues a session ID when a target client haven't issued seesion ID yet</p>
		 *
		 * \par Default organization of the Session ID
		 * <ul>
		 *	<li> {$name}::{$port}::{$sequence}::{$linux_time} </li>
		 *	<ol>
		 *		<li> $name: Name of the Server; Server::NAME() </li>
		 *		<li> $port: Port number of the Server; Server::PORT() </li>
		 *		<li> $sequence: Sequence number of creation of User in Server </li>
		 *		<li> $linux_time: Linux time of current time. </li>
		 *			<p> If the object Server is re-constructed, $sequence can be duplicated.
		 *			$linux_time is to avoid the duplication so that can ensure uniqueness on session id</p>
		 *	</ol>
		 * </ul>
		 */
		auto issueSessionID(size_t) const->std::string;
	};
};
};
};