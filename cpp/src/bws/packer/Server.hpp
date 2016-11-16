#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/WebServer.hpp>

#include <bws/packer/Client.hpp>

#include <iostream>

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
		Server()
			: super()
		{
		};
		virtual ~Server() = default;

	protected:
		virtual void addClient(std::shared_ptr<protocol::ClientDriver> driver) override
		{
			std::cout << "A client has connected." << std::endl;

			std::unique_ptr<Client> client(new Client(driver));
		};
	};
};
};