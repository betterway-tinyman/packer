#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityDeque.hpp>
#	include <samchon/protocol/external/ExternalSystem.hpp>
#include <samchon/protocol/IProtocol.hpp>

namespace samchon
{
namespace protocol
{
namespace external
{
	class SAMCHON_FRAMEWORK_API ExternalSystemArray
		: public SharedEntityDeque<ExternalSystem>,
		public virtual IProtocol
	{
		friend class ExternalSystem;

	private:
		typedef SharedEntityDeque<ExternalSystem> super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		ExternalSystemArray();

		virtual ~ExternalSystemArray();
		
	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto hasRole(const std::string &key) const -> bool
		{
			for (size_t i = 0; i < size(); i++)
				for (size_t j = 0; j < at(i)->size(); j++)
					if (at(i)->at(j)->key() == key)
						return true;

			return false;
		};

		auto getRole(const std::string &key) const -> std::shared_ptr<ExternalSystemRole>
		{
			for (size_t i = 0; i < size(); i++)
				for (size_t j = 0; j < at(i)->size(); j++)
					if (at(i)->at(j)->key() == key)
						return at(i)->at(j);

			throw std::out_of_range("No such key.");
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		virtual void sendData(std::shared_ptr<Invoke> invoke);

		virtual void replyData(std::shared_ptr<Invoke> invoke) = 0;

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "systemArray";
		};

		virtual auto CHILD_TAG() const -> std::string override
		{
			return "system";
		};
	};
};
};
};