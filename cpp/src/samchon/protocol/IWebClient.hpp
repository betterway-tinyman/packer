#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/IClient.hpp>

namespace boost
{
namespace system
{
	class error_code;
};
};
namespace samchon
{
namespace protocol
{
		/**
		 * @brief An interface for a web-client.
		 *
		 * @details
		 * <p> IWebClient is a IClient following web-socket protocol. </p>
		 *
		 * \par [Inherited]
		 *		@copydetails protocol::IClient
		 */
	class SAMCHON_FRAMEWORK_API IWebClient
		: public virtual IClient
	{
	protected:
		typedef IClient super;

	public:
		/* -----------------------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		IWebClient();
		virtual ~IWebClient() = default;

	protected:
		virtual auto isServer() const -> bool;

	public:
		/* -----------------------------------------------------------------------
			LISTEN MESSAGE
		----------------------------------------------------------------------- */
		virtual void listen() override;

	private:
		auto listen_string(size_t) -> std::shared_ptr<Invoke>;
		void listen_binary(size_t, std::shared_ptr<Invoke> &);

		/* -----------------------------------------------------------------------
			SEND MESSAGE
		----------------------------------------------------------------------- */
	public:
		virtual void sendData(std::shared_ptr<Invoke>) override;

	private:
		void send_header(unsigned char, size_t);

		void send_string(const std::string &);
		void send_binary(const ByteArray &);
	};
};
};