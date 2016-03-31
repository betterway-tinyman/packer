#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IClient.hpp>
#include <samchon/protocol/service/ServiceKeeper.hpp>

#include <mutex>
#include <string>

/**
 * @brief A macro keeps the Client object until termination of a stack.
 */
#define KEEP_CLIENT_ALIVE auto &ucPair = __keepAlive();

namespace samchon
{
namespace protocol
{
namespace service
{
	class Server;
	class User;
	class Service;

	/**
	 * @brief A network boundary with the client in an User
	 *
	 * @details
	 * <p> Client is an object interacting with physical client by socket. </p>
	 *
	 * <p> The Client is correspond with a Window in JS (UI) 1:1 </p>
	 *	\li A Window, the object in UI
	 *	\li A Window, matching with window of a internet browser
	 *
	 * @image html  cpp/protocol_service.png
	 * @image latex cpp/protocol_service.png
	 *
	 * @note Method to override
	 *	\li createUser()
	 *
	 * @see samchon::protocol
	 * @see samchon::protocol::service
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API Client
		: public IClient
	{
		friend class Server;
		friend class User;

	private:
		typedef IClient super;

	protected:
		/**
		 * @brief An User of the Client
		 */
		User *user;

		/**
		 * @brief A sequence number of the Client in an User
		 */
		size_t no;

		/**
		 * @brief A Service belongs to the Client
		 */
		Service *service;

	public:
		/**
		 * @brief Construct from User, No and socket
		 *
		 * @param user An User containing the Client
		 */
		Client(User*);
		virtual ~Client();

	protected:
		/**
		 * @brief Factory method of Service
		 *
		 * @param name Requested name representing a Service from the (physical) client
		 * @return A new Service belongs the the Client
		 */
		virtual auto createService(const std::string &)->Service* = 0;

		auto __keepAlive()->ServiceKeeper;

	public:
		/**
		 * @brief Get User
		 */
		auto getUser() const->User*;

		/**
		 * @brief Get Service
		 */
		auto getService() const->Service*;

		/**
		 * @brief Get no
		 */
		auto getNo() const->size_t;

	public:
		/**
		 * @brief Send Invoke message to (physical) client
		 *
		 * @details
		 * <p> Method sendData of Client does not only send Invoke message to client system,
		 * but also archives the Invoke message to database system as a historical-log. </p>
		 *
		 * @note
		 * <p> Method sendData monopolies a critical section. </p>
		 * <p> How many threads have called that sendData, the Invoke messages are delivered sequentially.</p>
		 *	\li Using sendData with a new thread is not recommended
		 *
		 * @warning You can't override this method, sendData.
		 * @param invoke Invoke message to send
		 */
		void sendData(std::shared_ptr<Invoke>);

		/**
		 * @brief Reply Invoke message from (physical) client
		 *
		 * @details
		 * <p> Handles replied Invoke message from client system. </p>
		 *
		 * <ol>
		 *	<li> Constructs Service </li>
		 *	<ul>
		 *		<li> Constructs Service by requested service name. </li>
		 *		<li> Notifies client system whether the user satisfies the authority. </li>
		 *	</ul>
		 *	<li> Shifts responsibility to related chain. </li>
		 *	<ul>
		 *		<li> Invoke message is about member (join, login, etc.), shift to User. </li>
		 *		<li> Except that all, shifts to Service. </li>
		 *	</ul>
		 *	<li> Archives the Invoke message to Database system as a historical-log. </li>
		 *	<ul>
		 *		<li> Invoke to HISTORY_INVOKE table, with User's session and Client's sequence id. </li>
		 *		<li> InvokeParameters(s) to HISTORY_INVOKE_PARAMETER with key of HISTORY_INVOKE as foriegn key. </li>
		 *	</ul>
		 * </ol>
		 *
		 * @note
		 * \par Historical-log
		 * <p> HISTORY_INVOKE and HISTORY_INVOKE_PARAMETER has a 1:N relationship </p>
		 * <p> If you want to modify archiving method, override method of Server::archiveReplyData() </p>
		 *
		 * \par Shifting responsibility to Service
		 * <p> Service::replyData will be called by a new thread. The thread will acquire an admission
		 * from semaphore in User to avoid exhausted allocation of threads.</p>
		 *
		 * \par Exception handling
		 * <p> If an std::exception has thrown from Service::replyData, the exception will
		 * archived in Database (HISTORY_INVOKE_EXCEPTION) </p>
		 * <p> The most famous thrown parameters like std::exception, std::invalid_argument and
		 * std::runtime_error, those are all derived from std::exception. <br>
		 * &nbsp;&nbsp;&nbsp;&nbsp; -> http://www.cplusplus.com/reference/stdexcept/ </p>
		 *
		 * <ul>
		 *  <li> std::exception </li>
		 *	<ul>
		 *		<li> std::logic_error </li>
		 *		<ul>
		 *			<li> std::domain_error </li>
		 *			<li> std::invalid_argument </li>
		 *			<li> std::length_error </li>
		 *			<li> std::out_of_range </li>
		 *			<li> std::future_error </li>
		 *		</ul>
		 *		<li> std::runtime_error </li>
		 *		<ul>
		 *			<li> std::range_error </li>
		 *			<li> std::overflow_error </li>
		 *			<li> std::underflow_error </li>
		 *			<li> std::system_error </li>
		 *		</ul>
		 *	</ol>
		 * </ul>
		 *
		 * @warning
		 * <p> You can't override this method, replyData. </p>
		 * <p> If you want to modify replyData's rule, override related objects' replyData </p>
		 *
		 * @param invoke Invoke message replied from client system
		 */
		void replyData(std::shared_ptr<Invoke>);

	private:
		/**
		 * @brief Construct Service
		 *
		 * @details
		 * <p> Constructs Service by requested name from client and notify whether the
		 * User is satisfying authority. </p>
		 *
		 * @param name A name representing type of Service
		 */
		void constructService(const std::string &);
	};
};
};
};