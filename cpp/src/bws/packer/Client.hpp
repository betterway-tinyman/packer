#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/IProtocol.hpp>
#include <samchon/protocol/ClientDriver.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief Driver for a connected client.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Client : public virtual protocol::IProtocol
	{
	private:
		std::shared_ptr<protocol::ClientDriver> driver;

	public:
		/**
		 * Construct from a socket.
		 *
		 * @param socket A socket connected with the client.
		 */
		Client(std::shared_ptr<protocol::ClientDriver>);
		virtual ~Client();

		virtual void sendData(std::shared_ptr<protocol::Invoke>) override;

		virtual void replyData(std::shared_ptr<protocol::Invoke>) override;

	private:
		/**
		 * @brief Pack instances to wrappers.
		 *
		 * @details 
		 * <p> Deducts the best optimization solution of packing and send the optimization 
		 * result data to the client. #pack needs lots of execution time and during the optimization
		 * process, progress data of the optimization will bed sent to the client. </p>
		 * 
		 * @param xml XML represents instances and wrappers to pack.
		 */
		void pack(std::shared_ptr<library::XML>);
	};
};
};