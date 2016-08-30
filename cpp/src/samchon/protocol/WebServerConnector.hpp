#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ServerConnector.hpp>
#include <samchon/protocol/WebCommunicator.hpp>

#include <map>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace protocol
{
	class SAMCHON_FRAMEWORK_API WebServerConnector
		: public ServerConnector,
		public WebCommunicator
	{
	private:
		typedef ServerConnector super;

		static std::map<std::pair<std::string, int>, std::string> s_cookies;
		static library::RWMutex s_mtx;

	public:
		WebServerConnector(IProtocol *listener);
		virtual ~WebServerConnector();

		virtual void connect(const std::string &ip, int port);
		void connect(const std::string &ip, int port, const std::string &path);

	private:
		void handshake(const std::string &ip, int port, const std::string &path);
	};
};
};