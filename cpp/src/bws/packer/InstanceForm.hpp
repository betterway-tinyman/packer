#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace bws
{
namespace packer
{
	class Instance;
	class InstanceArray;

	/**
	 * @brief A repeated Instance.
	 *
	 * @details InstanceForm is an utility class for repeated Instance. It shrinks volume of
	 *			network message I/O by storing {@link #count repeated count}.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class InstanceForm
		: public protocol::Entity<>
	{
	private:
		typedef protocol::Entity<> super;

		/**
		 * @brief A duplicated Instance.
		 */
		std::shared_ptr<Instance> instance;
		
		/**
		 * @brief Repeated count of the #instance.
		 */
		size_t count;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		InstanceForm();
		virtual ~InstanceForm() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

	private:
		/**
		 * @brief Factory method of #instance.
		 */
		auto createInstance(std::shared_ptr<library::XML>) -> Instance*;

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;

		virtual auto toXML() const -> std::shared_ptr<library::XML> override;

		/**
		 * @brief Repeated #instance to InstanceArray.
		 *
		 * @details 
		 * <p> Contains the {@link #instance repeated instance} to an InstanceArray to make #instance
		 * to participate in the packing process. The returned InstanceArray will be registered on 
		 * Packer::instanceArray.
		 *
		 * @return An array of instance containing repeated #instance.
		 */
		auto toInstanceArray() const -> std::shared_ptr<InstanceArray>;
	};
};
};