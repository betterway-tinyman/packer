#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ClientDriver.hpp>
#include <samchon/protocol/WebCommunicator.hpp>

namespace samchon
{
namespace protocol
{
	class WebServer;

	class SAMCHON_FRAMEWORK_API WebClientDriver
		: public ClientDriver,
		public WebCommunicator
	{
		friend class WebServer;

	private:
		typedef ClientDriver super;

		std::string session_id;
		std::string path;

	public:
		WebClientDriver(WebServer *server, std::shared_ptr<Socket> socket);
		virtual ~WebClientDriver();

		auto getSessionID() const -> std::string
		{
			return session_id;
		};
		auto getPath() const -> std::string
		{
			return path;
		};
	};
};
};