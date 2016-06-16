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
	enum ENUM_DIRECTION
	{
		SERVER = 1,
		CLIENT = 2
	};

		/**
		 * @brief An interface for a web-client.
		 *
		 * @details
		 * <p> IWebClient is a IClient following web-socket protocol. </p>
		 *
		 * \par [Inherited]
		 *		@copydetails protocol::IClient
		 */
	class SAMCHON_FRAMEWORK_API IWebClientBase
		: public virtual IClient
	{
	protected:
		typedef IClient super;

		static const unsigned char TEXT_HEADER = 129;
		static const unsigned char BINARY_HEADER = 130;

	public:
		/* -----------------------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		IWebClientBase();
		virtual ~IWebClientBase() = default;

	protected:
		virtual auto DIRECTION() const->ENUM_DIRECTION = 0;


	public:
		/* -----------------------------------------------------------------------
			LISTEN MESSAGE
		----------------------------------------------------------------------- */
		virtual void listen() override;

	private:
		auto listenString(size_t, ByteArray&, boost::system::error_code &)->std::shared_ptr<Invoke>;
		void listenBinary(size_t, ByteArray&, std::shared_ptr<Invoke>, boost::system::error_code &);

		void listenMoreBytes(ByteArray&, boost::system::error_code &);

		/* -----------------------------------------------------------------------
			SEND MESSAGE
		----------------------------------------------------------------------- */
	public:
		virtual void sendData(std::shared_ptr<Invoke>) override;

	private:
		void sendSizeHeader(unsigned char, size_t, boost::system::error_code &);

		void sendString(const std::string &, boost::system::error_code &);
		void sendBinary(const ByteArray &, boost::system::error_code &);
	};
};
};