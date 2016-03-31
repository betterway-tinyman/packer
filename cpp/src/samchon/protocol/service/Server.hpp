#pragma once
#include <samchon/API.hpp>
#include <samchon/protocol/IServer.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <samchon/Dictionary.hpp>
#include <samchon/SmartPointer.hpp>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace library
{
	class SQLi;
	class SQLStatement;
};
namespace protocol
{
namespace service
{
	class IPUserPair;
	class User;
	class Client;

	/**
	 * @brief A server for (cloud) service
	 *
	 * @details
	 * <p> A server class representing the real physical server of a cloud. </p>
	 *
	 * <p> The Server is very good for development of a cloud server. You can use web or flex as
	 * (physical) client. The usage is very simple. In the class Server, what you need to do are
	 * overriding getter of port number and factory method creating an User. </p>
	 *
	 * @image html  cpp/protocol_service.png
	 * @image latex cpp/protocol_service.png
	 *
	 * @note Override those methods.
	 *  \li Server::NAME()
	 * 	\li Server::PORT()
	 * 	\li Server::createUser()
	 *
	 * @see samchon::protocol
	 * @see samchon::protocol::service
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API Server
		: private Dictionary<SmartPointer<User>>,
		public IServer
	{
		friend class IPUserPair;
		friend class User;

	private:
		typedef Dictionary<SmartPointer<User>> super;

	protected:
		/**
		 * @brief A name can be an identifier of a cloud server.
		 */
		virtual auto NAME() const->std::string = 0;

		/**
		 * @brief SQLi for archiving log
		 */
		library::SQLi *sqli;

	private:
		library::RWMutex mtx;

		/**
		 * @brief Dictionary of issuer of session ID of each ip
		 */
		Dictionary<std::shared_ptr<IPUserPair>> ipMap;

		/**
		 * @brief Sequence for issuing session ID
		 */
		size_t sequence;

	public:
		/**
		 * @brief Default Constructor
		 */
		Server();
		virtual ~Server();

		/* =========================================================
			GETTERS
		========================================================= */
		auto getSQLi() const->library::SQLi*;

		/* =========================================================
			ACCESSORS OF MAP
		========================================================= */
		/**
		 * @brief Size of User(s).
		 */
		auto size() const->size_t;

		/**
		 * @brief A const iterator of begin
		 *
		 * @note Be careful about concurrency
		 */
		auto begin() const->const_iterator;

		/**
		 * @brief A const iterator of end
		 *
		 * @note Be careful about concurrency
		 */
		auto end() const->const_iterator;

	protected:
		/* =========================================================
			ABSTRACT METHODS
		========================================================= */
		/**
		 * @brief Factory method of User
		 */
		virtual auto createUser()->User* = 0;

		/**
		* @brief Handling connection of a client
		*/
		virtual void addClient(Socket*) override;

	private:
		void eraseUser(const std::string &);
	};
};
};
};