#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#include <bws/packer/Product.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief An array containing Instance objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class InstanceArray
		: public protocol::SharedEntityArray<Instance>
	{
	private:
		typedef protocol::SharedEntityArray<Instance> super;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		using super::super;
		virtual ~InstanceArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> Instance* override;

	public:
		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;
		virtual auto CHILD_TAG() const -> std::string override;
	};
};
};