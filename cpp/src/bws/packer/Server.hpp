#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/IServer.hpp>

#include <memory>

namespace samchon
{
	namespace protocol
	{
		class FlashPolicyServer;
	};
};

namespace bws
{
namespace packer
{
	class Server
		: public protocol::IServer
	{
	private:
		typedef protocol::IServer super;

	protected:
		virtual auto PORT() const -> int override;

		std::unique_ptr<protocol::FlashPolicyServer> flashPolicyServer;

	public:
		Server();
		virtual ~Server() = default;

		virtual void open() override;

	protected:
		virtual void addClient(protocol::Socket*) override;
	};
};
};