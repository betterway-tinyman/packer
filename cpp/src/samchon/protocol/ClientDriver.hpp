#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Communicator.hpp>

namespace samchon
{
namespace protocol
{
	class Server;

	class SAMCHON_FRAMEWORK_API ClientDriver
		: public virtual Communicator
	{
	protected:
		Server *server;

	public:
		ClientDriver(Server *server, std::shared_ptr<Socket> socket);
		virtual ~ClientDriver();

		void listen(IProtocol *listener);
	};
};
};