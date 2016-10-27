#include <bws/packer/Server.hpp>

#include <memory>

using namespace std;
using namespace bws::packer;

void main()
{
	unique_ptr<Server> server(new Server());
	server->open(37896);
}