#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IServer.hpp>

namespace samchon
{
	namespace protocol
	{
		/**
		 * @brief An interface for a physical server following web-socket.
		 *
		 * @details
		 * <p> IWebServer is a IServer following web-socket protocol. </p>
		 *
		 * \par [Inherited]
		 *		@copydetails protocol::IServer
		 */
		class SAMCHON_FRAMEWORK_API IWebServer
			: public virtual IServer
		{
		protected:
			typedef IServer super;

		public:
			/* -----------------------------------------------------------------------
				CONSTRUCTORS
			----------------------------------------------------------------------- */
			/**
			 * @brief Default Constructor.
			 */
			IWebServer();
			virtual ~IWebServer() = default;

			/**
			 * @brief Open the web-socket server
			 * @details Accepts clients and handshake with them following web-socket protocol.
			 *
			 * @note It monopolies a thread.
			 */
			virtual void open() override;

		private:
			/* -----------------------------------------------------------------------
				HANDSHAKE OF WEB-SOCKET
			----------------------------------------------------------------------- */
			auto handshake(Socket *socket) const -> bool;

			auto calculateCertificationKey(const std::string &) const -> std::string;
		};
	};
};