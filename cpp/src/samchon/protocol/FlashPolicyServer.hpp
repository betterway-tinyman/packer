#pragma once
#include <samchon/API.hpp>

#include <memory>
#include <samchon/protocol/Socket.hpp>

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
	class SAMCHON_FRAMEWORK_API FlashPolicyServer
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
		FlashPolicyServer();

		/**
		 * @brief Construct from custom policy_
		 *
		 * @param policy_ Custom cross-domain-policy_ to adjust
		 */
		FlashPolicyServer(std::shared_ptr<library::XML>);

		/**
		 * @brief Open server
		 */
		void open(int port = 843);

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
		void accept(Socket *);
	};
};
};