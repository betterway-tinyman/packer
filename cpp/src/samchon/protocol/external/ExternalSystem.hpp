#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityDeque.hpp>
#	include <samchon/protocol/external/ExternalSystemRole.hpp>
#include <samchon/protocol/IProtocol.hpp>

namespace samchon
{
namespace protocol
{
	class Communicator;

namespace external
{
	class SAMCHON_FRAMEWORK_API ExternalSystem 
		: public SharedEntityDeque<ExternalSystemRole>,
		public virtual IProtocol
	{
		friend class ExternalClientArray;

	private:
		typedef SharedEntityDeque<ExternalSystemRole> super;

	protected:
		std::string name;

		std::shared_ptr<Communicator> communicator;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		ExternalSystem();
		virtual ~ExternalSystem();

		virtual void construct(std::shared_ptr<library::XML> xml) override;

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto key() const -> std::string
		{
			return name;
		};

		auto getName() const -> std::string
		{
			return name;
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke> invoke);

		virtual void replyData(std::shared_ptr<Invoke> invoke);

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "system";
		};
		virtual auto CHILD_TAG() const -> std::string override
		{
			return "role";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};
};