#pragma once
#include <samchon/API.hpp>

#include <boost/asio.hpp>
#include <thread>
#include <samchon/library/XML.hpp>

namespace samchon
{
namespace library
{
	class XML;
};
namespace protocol
{
	/**
	 * @brief A flash policy_ server
	 * @details FlashPolicyServer is a policy_ server replying cross-domain-policy_ to flash clients.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class FlashPolicyServer
	{
	private:
		/**
		 * @brief cross-domain-policy_
		 */
		std::shared_ptr<library::XML> policy;

	public:
		/**
		 * @brief Default constructor
		 *
		 * @details The cross-domain-policy_ is to accept all, any client.
		 */
		FlashPolicyServer()
			: FlashPolicyServer
				(
					std::make_shared<library::XML>
					(
						std::string() +
							"<cross-domain-policy>\n" +
							"	<allow-access-from domain='*' to-ports='*' />\n" +
							"</cross-domain-policy>"
						)
				)
		{
		};

		/**
		 * @brief Construct from custom policy_
		 *
		 * @param policy_ Custom cross-domain-policy_ to adjust
		 */
		FlashPolicyServer(std::shared_ptr<library::XML> policy)
		{
			this->policy = policy;
		};

		/**
		 * @brief Open server
		 */
		void open(int port = 843)
		{
			boost::asio::io_service ioService;
			boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), port);
			boost::asio::ip::tcp::acceptor acceptor(ioService, endPoint);

			while (true)
			{
				boost::asio::ip::tcp::socket *socket = new boost::asio::ip::tcp::socket(ioService);
				acceptor.accept(*socket);

				std::thread(&FlashPolicyServer::accept, this, socket).detach();
			}
		};

		/**
		 * @brief Get policy.
		 */
		auto getPolicy() const -> std::shared_ptr<library::XML>
		{
			return policy;
		};

		void setPolicy(std::shared_ptr<library::XML> xml)
		{
			this->policy = xml;
		};

	private:
		/**
		 * @brief Accept client
		 *
		 * @details Accepts flash client and replies cross-domain-policy_
		 */
		void accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
		{
			std::string data = "<?xml version=\"1.0\"?>\n" + policy->toString();
			std::vector<unsigned char> piece;
			boost::system::error_code error;

			while (true)
			{
				piece.assign(1000, NULL);
				socket->read_some(boost::asio::buffer(piece), error);

				boost::system::error_code error;
				socket->write_some(boost::asio::buffer(data), error);

				break;
			}
			socket->close();
		};
	};
};
};