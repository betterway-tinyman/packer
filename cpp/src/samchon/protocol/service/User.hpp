#pragma once
#include <samchon/API.hpp>

#include <samchon/Map.hpp>
#include <samchon/SmartPointer.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <string>
#include <memory>
#include <samchon/library/RWMutex.hpp>
#include <samchon/protocol/service/ServiceKeeper.hpp>
#include <samchon/protocol/Socket.hpp>

/**
 * @brief Keep User object alive
 *
 * @param client
 */
#define KEEP_USER_ALIVE auto &keeper = SmartPointer<User>(this);

namespace samchon
{
namespace library
{
	class Semaphore;
};
namespace protocol
{
	class Invoke;

namespace service
{
	class Server;
	class Client;

	class IPUserPair;

	/**
	 * @brief User containing Client(s) with session-id.
	 *
	 * @details
	 * <p> The User does not have any network I/O and its own special work something to do. It's a
	 * container for groupping clients by their session id. Thus, the User corresponds with an
	 * user (Computer) and Client corresponds with a client (A browser window) </p>
	 *
	 * @image html  cpp/protocol_service.png
	 * @image latex cpp/protocol_service.png
	 *
	 * @notice Override those methods
	 *	\li createClient()
	 *	\li doLogin()
	 *	\li doJoin()
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API User
		: private Map<size_t, SmartPointer<Client>>,
		public IProtocol
	{
		friend class Server;
		friend class Client;

	private:
		typedef Map<size_t, SmartPointer<Client>> super;

	private:
		/* =========================================================
			BASIC VARIABLES
		========================================================= */
		/**
		 * @brief Server containing the user
		 */
		Server *server;

		/**
		 * @brief Session ID of the user
		 *
		 * @details
		 *
		 *
		 * \par Organization of the Session ID
		 * <ul>
		 *	<li> {$name}::{$port}::{$sequence}::{$linux_time} </li>
		 *	<ol>
		 *		<li> $name: Name of the Server; Server::NAME() </li>
		 *		<li> $port: Port number of the Server; Server::PORT() </li>
		 *		<li> $sequence: Sequence number of creation of User in Server </li>
		 *		<li> $linux_time: Linux time of current time. </li>
		 *			<p> If the object Server is re-constructed, $sequence can be duplicated.
		 *			$linux_time is to avoid the duplication so that ensures exclusiveness on session id</p>
		 *	</ol>
		 * </ul>
		 */
		std::string sessionID;

		IPUserPair *ipPair;

		/* =========================================================
			VARIABLES FOR CRITICAL SECTION
		========================================================= */
		/**
		 * @brief Mutex for container
		 */
		library::RWMutex mtx;

		/**
		 * @brief Semaphore to limit number of thread
		 *
		 * @details
		 * <p> Semaphore in User is designed to constraint number of thread. </p>
		 * <p> It's recommended to acquire the semaphore when a task from Client or Service
		 * occurs a new thread and you want to avoid exhausted allocation of new threads. </p>
		 *
		 * @note
		 * <p> If a task influences to all clients in a user globally,
		 * use lock instead of acquire </p>
		 */
		library::Semaphore *semaphore;

		/**
		 * @brief
		 */
		size_t sequence;

		/* =========================================================
			ACCOUNT VARIABLES
		========================================================= */
		/**
		 * @brief Account id
		 *
		 * @details Default is \"guest\"
		 */
		std::string id;

		/**
		 * @brief Authority allocated to user
		 *
		 * @details
		 * <p> The authority means authority(right, power) level allocated to the user. </p>
		 * <p> The default value is zero, which means not logged-in user. </p>
		 *
		 * @note
		 * <p> Each number of level is not defined clearly. Follwing table is just an example. </p>
		 *
		 * <table>
		 *	<tr>
		 *		<th>Level</th>
		 *		<th>Definition</th>
		 *	</tr>
		 *	<tr>
		 *		<td>-1</td>
		 *		<td>
		 *			<p> Banned member </p>
		 *			<p> The -1 level member is banished by manager or policy </p>
		 *		</td>
		 *	<tr>
		 *		<td>0</td>
		 *		<td>
		 *			<p> Guest, Not logged in </p>
		 *			<p>  </p>
		 *		</td>
		 *	</tr>
		 *	<tr>
		 *		<td>1</td>
		 *		<td>
		 *			<p> Unconfired member </p>
		 *			<p> The member have applied to the join,
		 *			but the join was not confirmed by manager or policy yet. </p>
		 *		</td>
		 *	</tr>
		 *	<tr>
		 *		<td>2</td>
		 *		<td>
		 *			<p> Confirmed member </p>
		 *			<p> The member have joined and have already confirmed by manager or policy </p>
		 *		</td>
		 *	</tr>
		 *	<tr>
		 *		<td>3</td>
		 *		<td>
		 *			<p> Manager </p>
		 *			<p> A manager of the application(cloud service). The manager can</p>
		 *			<ul>
		 *				<li> Access to the monitoring service. (MonitorService) </li>
		 *				<li> Access historical log of members whose level are below Manager. </li>
		 *				<li> Banish or modify level of members whose level are below Manager. </li>
		 *			</u>
		 *		</td>
		 *	</tr>
		 *	<tr>
		 *		<td>4</td>
		 *		<td>
		 *			<p> Administrator or super Manager </p>
		 *			<p> Administrator is a Manager of Managers. An administrator can do same thing
		 *			with Managers, but adminstrator even possible to target Managers as below. </p>
		 *		</td>
		 *	</tr>
		 * </table>
		 */
		int authority;

	public:
		/**
		 * @brief Construct from server
		 *
		 * @param server A server who containing users and clients
		 * @param sessionID A session ID for discriminating User; A logical identifier
		 */
		User(Server*);
		virtual ~User();

		/* =========================================================
			ACCESSORS OF MAP
		========================================================= */
		auto size() const->size_t;
		auto begin() const->const_iterator;
		auto end() const->const_iterator;

		/* =========================================================
			GETTERS
		========================================================= */
		/**
		 * @brief Get server
		 */
		auto getServer() const->Server*;

		/**
		 * @brief Get semaphore
		 *
		 * @see User::semaphore
		 */
		auto getSemaphore() const->library::Semaphore*;

		/**
		 * @brief Get user's account id
		 */
		auto getID() const->std::string;

		/**
		 * @brief Get authority of user
		 *
		 * @see User::authority
		 */
		auto getAuthority() const -> int;

	protected:
		/* =========================================================
			CLIENT FACTORY
		========================================================= */
		/**
		 * @brief Factory method of a Client
		 */
		virtual auto createClient()->Client* = 0;
		//void setMember(const std::string &id, int authority);

	private:
		/**
		 * @brief Add a client in user
		 *
		 * @param socket Socket for interacting with (physical) client
		 */
		void addClient(Socket*);

		/**
		 * @brief Erase a Client
		 *
		 * @details
		 * <p> Erase a Client from User by its key(no:int). </p>
		 * <p> When a network connection between server and user(person) was disconnected,
		 * matched Client is erased from User and the Client object will be destructed. </p>
		 *
		 * <p> Some reasons of disconnection </p>
		 * <ul>
		 *	<li> An user(not object but person using UI) closed the Window </li>
		 *	<li> A \"closeWindow\" command has sent to the Window </li>
		 *	<li> A network failure has occured </li>
		 * </ul>
		 *
		 * <p>If the Client was the last object remained in the User, the User will be
		 * destructed too. However, the User is not destructed directly but waits for 15
		 * seconds. In the 15 seconds, another Client object was created in User, the User
		 * doesn't be destructed.</p>
		 *
		 * <p> Some reasons of postponing destruction for 15 seconds </p>
		 * <ul>
		 *	<li> Page direction(href) or redirection in a browser. </li>
		 *	<li> Temporary network failure. </li>
		 * </ul>
		 *
		 * @note
		 * <p> There're some tasks that have to completed irrespectively of network connection.</p>
		 * <p> In that case, although user closed the window or logged-out, matched User, Client and
		 * Service objects haven't be destructed. They must stay alive until the tasks have completed.</p>
		 *
		 * <p> To stay those objects User, Client and Service, use follwing macros.
		 * Those macros assure the objects to stay alive until the termination matched
		 * each stack. </p>
		 * <ul>
		 *	<li> KEEP_USER_ALIVE(Client*) </li>
		 *	<li> KEEP_CLIENT_ALIVE </li>
		 *	<li> KEEP_SERVICE_ALIVE </li>
		 * </ul>
		 *
		 * @param no Sequence number of the client in user
		 */
		void eraseClient(size_t);


		/* =========================================================
			LOGIN AND JOIN
		========================================================= */
		/**
		 * @brief Log-in
		 *
		 * @param client
		 * @param invoke
		 * @see User::doLogin()
		 */
		void goLogin(std::shared_ptr<Invoke>);

		/**
		 * @brief Join as a member
		 *
		 * @param client An application; who requested the join command
		 * @param invoke AN invoke message containing parameters of joinning
		 * @see User::doJoin()
		 */
		void goJoin(std::shared_ptr<Invoke>);

		/**
		 * @brief Log-out
		 *
		 * @details
		 * <p> Log-out account; change id and authority to \"guest\" and 0. </p>
		 * <p> Default is to send \"closeWindow\" message to all clients except the applicant. </p>
		 *
		 * <ul>
		 *	<li> Perspective on UI: </li>
		 *		<p> All windows of the user will be closed by getting an invoke message
		 *		\"closeWindow\" except who requested the command log-out. </p>
		 *		<p> The applicant window will moved to default log-in page. </p>
		 * </ul>
		 *
		 * @param client A window of browser; client who requested the log-out command
		 */
		virtual void goLogout();

	protected:
		/**
		 * @brief Login and return whether success
		 *
		 * @param invoke Invoke message containing parameters for logging-in
		 * @return Whether the log-in is succeded or not
		 */
		virtual auto doLogin(std::shared_ptr<Invoke>) -> bool = 0;

		/**
		 * @brief Join and returns whether success
		 *
		 * @param invoke Invoke message containing paramters for joinning
		 * @return Whether the join is succeded or not
		 */
		virtual auto doJoin(std::shared_ptr<Invoke>) -> bool = 0;

	public:
		/* =========================================================
			MESSAGE CHAIN
		========================================================= */
		virtual void sendData(std::shared_ptr<Invoke>) override;
		virtual void replyData(std::shared_ptr<Invoke>) override;
	};
};
};
};