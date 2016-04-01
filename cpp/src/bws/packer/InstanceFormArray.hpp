#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <bws/packer/InstanceForm.hpp>

namespace bws
{
namespace packer
{
	class InstanceArray;

	/**
	 * @brief An array of InstanceForm objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class InstanceFormArray
		: public protocol::SharedEntityArray<InstanceForm>
	{
	private:
		typedef protocol::SharedEntityArray<InstanceForm> super;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		InstanceFormArray();
		virtual ~InstanceFormArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> InstanceForm*;

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;
		virtual auto CHILD_TAG() const -> std::string override;

		/**
		 * @brief Convert InstanceForm objects to InstanceArray
		 *
		 * @return An array of instance containing repeated instances in InstanceForm objects.
		 */
		auto toInstanceArray() const -> std::shared_ptr<InstanceArray>;
	};
};
};