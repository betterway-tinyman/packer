#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IProtocol.hpp>

#include <string>
#include <samchon/protocol/service/ServiceKeeper.hpp>

/**
 * @brief A macro keeps the Service object to alive until termination of a stack.
 */
#define KEEP_SERVICE_ALIVE auto &ucPair = __keepAlive();

namespace samchon
{
namespace library
{
	class XML;
	class SQLi;
};
namespace protocol
{
namespace service
{
	class Client;

	/**
	 * @brief A service
	 *
	 * @details
	 * <p> A service represents an unique functions of a web page </p>
	 *
	 * <p> Most of functinos are realized in here. The Service is correspondent with a 'a service
	 * of a web page'. For a cloud Server, there can be enormous Service classes. Create Services
	 * for each functions and Define the functions detail in here. </p>
	 *
	 * @image html  cpp/protocol_service.png
	 * @image latex cpp/protocol_service.png
	 *
	 * @see samchon::protocol
	 * @see samchon::protocol::service
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API Service
		: public IProtocol
	{
		friend class Client;

	private:
		typedef IProtocol super;

	protected:
		/**
		 * @brief Required authority to access the service
		 */
		virtual auto REQUIRE_AUTHORITY() const -> int = NULL;

	private:
		/**
		 * @brief Client object Service is belonged to
		 */
		Client *client;

		/**
		 * @brief A name represents a Service
		 */
		std::string name;

	public:
		/**
		 * @brief Construct from Client
		 *
		 * @param client Client object Service is belonged to
		 */
		Service(Client*);
		virtual ~Service() = default;

		/**
		 * @brief Get Client
		 */
		auto getClient() const->Client*;

	protected:
		auto __keepAlive()->ServiceKeeper;

	public:
		/**
		 * @brief Shift the responsibility of sending an Invoke message to Client
		 *
		 * @param in An Invoke message to be sent to the (physical) client
		 */
		virtual void sendData(std::shared_ptr<Invoke>);
	};
};
};
};