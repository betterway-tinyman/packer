#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystem.hpp>
#include <samchon/protocol/ServerConnector.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief A network driver for an external server.
	 *
	 * @details 
	 * <p> ExternalServer is an ExternalSystem specialized in server driver. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::ExternalSystem
	 */
	class SAMCHON_FRAMEWORK_API ExternalServer
		: public virtual ExternalSystem,
		public virtual ServerConnector
	{
	private:
		typedef ExternalSystem super;

	protected:
		/**
		 * @brief A custom ip address of my system to bind
		 */
		std::string myIP;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		ExternalServer();
		virtual ~ExternalServer() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void start() override;

		/* ------------------------------------------------------------------
			GETTERS
		------------------------------------------------------------------ */
		virtual auto getIP() const->std::string override;
		virtual auto getPort() const -> int override;
		virtual auto getMyIP() const->std::string override;

		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};