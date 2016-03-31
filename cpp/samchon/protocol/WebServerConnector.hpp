#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ServerConnector.hpp>
#include <samchon/protocol/IWebClient.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief A web-socket server connector
	 *
	 * @details  
	 * <p> WebServerConnector is a ServerConnector following web-socket protocol. </p>
	 *
	 * \par [Inherited]
	 * @copydetails protocol::ServerConnector
	 */
	class SAMCHON_FRAMEWORK_API WebServerConnector
		: public ServerConnector,
		public virtual IWebClient<CLIENT>
	{
	private:
		typedef ServerConnector super;
		typedef IWebClient<CLIENT> web_super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		WebServerConnector();
		virtual ~WebServerConnector() = default;

		/**
		 * @brief Connect to a server following web-socket protoocol.
		 */
		virtual void connect() override;
	};
};
};