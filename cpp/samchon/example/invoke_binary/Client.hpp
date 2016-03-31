#pragma once
#include <samchon/protocol/ServerConnector.hpp>

#include <iostream>
#include <fstream>
#include <thread>
#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
	namespace example
	{
		namespace invoke_binary
		{

class Client
	: public protocol::ServerConnector
{
private:
	typedef protocol::ServerConnector super;

public:
	Client()
		: super()
	{
	};
	virtual void listen()
	{
		std::thread
		(
			&Client::sendData, this, 
			std::make_shared<protocol::Invoke>("getFile")
		).detach();
		
		super::listen();
	}
	
	virtual auto getIP() const -> std::string override
	{
		return "127.0.0.1";
	};
	virtual auto getPort() const -> int override
	{
		return 11711;
	};

	virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
	{
		if (invoke->getListener() != "saveFile")
			return;

		const std::string &name = invoke->at(0)->referValue<std::string>();
		const std::string &extension = invoke->at(1)->referValue<std::string>();

		const ByteArray &data = invoke->at(2)->referValue<ByteArray>();

		std::ofstream file("E:/" + name + "." + extension, std::ios::out | std::ios::binary);
		file.write((const char*)&data[0], data.size());

		file.close();
	};
};

		};
	};
};