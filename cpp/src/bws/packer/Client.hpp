#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/IClient.hpp>

namespace samchon
{
	namespace library
	{
		class XML;
	};
};

namespace bws
{
namespace packer
{
	/**
	 * @brief Driver for a connected client.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Client
		: public protocol::IClient
	{
	private:
		typedef protocol::IClient super;

	public:
		/**
		 * Construct from a socket.
		 *
		 * @param socket A socket connected with the client.
		 */
		Client(protocol::Socket*);
		virtual ~Client() = default;

		virtual void replyData(std::shared_ptr<protocol::Invoke>) override;

	protected:
		virtual void _replyData(std::shared_ptr<protocol::Invoke>) override;

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