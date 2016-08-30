#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/WebServer.hpp>

namespace bws
{
namespace packer
{
	class Server 
		: public protocol::WebServer
	{
	private:
		typedef protocol::WebServer super;

	public:
		Server();
		virtual ~Server();

	protected:
		virtual void addClient(std::shared_ptr<protocol::ClientDriver>) override;
	};
};
};