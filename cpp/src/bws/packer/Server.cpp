#pragma once
#include <bws/packer/Server.hpp>
#	include <bws/packer/Client.hpp>

#include <iostream>

using namespace std;
using namespace bws::packer;

Server::Server()
	: super()
{
}
Server::~Server()
{
}

void Server::addClient(shared_ptr<protocol::ClientDriver> driver)
{
	cout << "A client has connected." << endl;

	unique_ptr<Client> client(new Client(driver));
}