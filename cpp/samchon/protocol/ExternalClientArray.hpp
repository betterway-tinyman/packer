#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystemArray.hpp>
#include <samchon/protocol/IServer.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An array of ExternalClient(s)
	 *
	 * @details
	 * <p> ExternalClientArray is an ExternalSystemArray specialized in clients' driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::ExternalSystemArray
	 */
	class SAMCHON_FRAMEWORK_API ExternalClientArray
		: public virtual ExternalSystemArray,
		private virtual IServer
	{
	protected:
		typedef ExternalSystemArray super;

		/**
		 * @brief A custom ip address of my system to bind
		 */
		std::string myIP;

		/**
		 * @brief A port number for accepting external clients.
		 */
		int port;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		ExternalClientArray();
		virtual ~ExternalClientArray() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void start() override;

	protected:
		/* ------------------------------------------------------------------
			ISERVER METHODS
		------------------------------------------------------------------ */
		virtual auto PORT() const -> int;
		virtual auto MY_IP() const->std::string;

		virtual void addClient(Socket*) override;

	public:
		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};