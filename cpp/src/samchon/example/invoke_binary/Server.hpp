#pragma once

#include <samchon/protocol/IServer.hpp>
#include <samchon/protocol/IClient.hpp>

#include <iostream>
#include <fstream>
#include <iterator>
#include <thread>

#include <samchon/ByteArray.hpp>
#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
	namespace example
	{
		namespace invoke_binary
		{

class File
{
public:
	std::string name;
	std::string extension;
	ByteArray data;

public:
	File(WeakString path)
	{
		// LOAD FILE
		std::ifstream file(path.str(), std::ios::in | std::ios::binary);
		file.unsetf(std::ios::skipws);

		std::istream_iterator<unsigned char> begin(file), end;

		data.assign(begin, end);
		file.close();

		// SET NAME AND EXTENSION
		path = path.substr(path.rfind("/") + 1);
		name = path.between("", ".");
		extension = path.between(".");
	};

	auto toInvoke() const -> std::shared_ptr<protocol::Invoke>
	{
		//return std::make_shared<protocol::Invoke>("saveFile2", name, extension);
		return std::make_shared<protocol::Invoke>("saveFile", name, extension, data);
	};
};

class Server
	: public protocol::IServer,
	public protocol::IClient
{
private:
	typedef protocol::IServer super;

	std::vector<File> fileArray;

public:
	Server()
		: super(),
		protocol::IClient()
	{
		fileArray =
		{
			File("D:/cpp_class_diagram.pdf"),
			File("D:/entity_relationship_diagram.pdf"),
			File("D:/development_guide.pdf"),
			File("D:/js_class_diagram.pdf"),
			File("D:/sequence_diagram.pdf")
		};
	};

protected:
	virtual auto PORT() const -> int override
	{
		return 11711;
	};
	virtual void addClient(protocol::Socket *socket)
	{
		std::cout << "A client has connected." << std::endl;

		this->socket = socket;
		listen();
	};

public:
	virtual void sendData(std::shared_ptr<protocol::Invoke> invoke)
	{
		protocol::IClient::sendData(invoke);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	};
	virtual void replyData(std::shared_ptr<protocol::Invoke> invoke)
	{
		std::cout << invoke->toXML()->toString() << std::endl;
		if (invoke->getListener() != "getFile")
			return;

		std::shared_ptr<protocol::Invoke> send_invoke(new protocol::Invoke("saveFile"));
		for (size_t i = 0; i < fileArray.size(); i++)
		{
			const File &file = fileArray[i];

			send_invoke->emplace_back(new protocol::InvokeParameter(file.name + "." + file.extension, file.data));
		}

		this->sendData(send_invoke);

		/*for (size_t i = 0; i < fileArray.size(); i++)
			this->sendData(fileArray[i].toInvoke());*/
	}
};

		};
	};
};