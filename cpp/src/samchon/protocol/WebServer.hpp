#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Server.hpp>

namespace samchon
{
namespace protocol
{
	class SAMCHON_FRAMEWORK_API WebServer 
		: public virtual Server
	{
	private:
		typedef Server super;

	public:
		WebServer();
		virtual ~WebServer();

	protected:
		virtual void handleConnection(std::shared_ptr<Socket> socket);

	private:
		size_t sequence;

		auto issue_session_id() -> std::string;
	};
};
};