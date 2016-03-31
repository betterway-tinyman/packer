#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace bws
{
namespace packer
{
	class Instance;
	class InstanceArray;

	class InstanceForm
		: public protocol::Entity
	{
	private:
		typedef protocol::Entity super;

		std::shared_ptr<Instance> instance;
		size_t count;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		InstanceForm();
		virtual ~InstanceForm() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

	private:
		auto createInstance(std::shared_ptr<library::XML>) -> Instance*;

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;

		virtual auto toXML() const -> std::shared_ptr<library::XML> override;

		auto toInstanceArray() const -> std::shared_ptr<InstanceArray>;
	};
};
};