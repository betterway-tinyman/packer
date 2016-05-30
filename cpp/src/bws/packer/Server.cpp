#pragma once
#include <bws/packer/Server.hpp>

#include <bws/packer/Client.hpp>
#include <samchon/protocol/FlashPolicyServer.hpp>

#include <iostream>
#include <thread>

using namespace std;
using namespace bws::packer;
using namespace samchon::protocol;

auto Server::PORT() const -> int
{
	return 37896;
}

Server::Server()
	: super()
{
	flashPolicyServer.reset(new FlashPolicyServer());
}
void Server::open()
{
	thread(&FlashPolicyServer::open, flashPolicyServer.get()).detach();
	super::open();
}

void Server::addClient(Socket *socket)
{
	cout << "addClient" << endl;

	thread([this, socket]()
	{
		unique_ptr<Client> client(new Client(socket));
		client->listen();
	}).detach();
}
