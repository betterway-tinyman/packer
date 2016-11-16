#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <bws/packer/InstanceArray.hpp>

namespace bws
{
namespace packer
{
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
		InstanceForm() 
			: super()
		{
		};
		virtual ~InstanceForm() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			if (xml->hasProperty("type"))
			{
				instance.reset(createInstance(xml));
				instance->construct(xml);
			}
			else if (xml->has("instance"))
			{
				auto instanceXML = xml->get("instance")->at(0);

				instance.reset(createInstance(instanceXML));
				instance->construct(instanceXML);
			}

			count = xml->getProperty<size_t>("count");
		};

	private:
		/**
		 * @brief Factory method of #instance.
		 */
		auto createInstance(std::shared_ptr<library::XML> xml) -> Instance*
		{
			if (xml->getProperty("type") == "product")
				return new Product();
			else
				return new Wrapper();
		};

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "instanceForm";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = super::toXML();
			if (instance != nullptr)
				xml->push_back(instance->toXML());

			xml->setProperty("count", count);

			return xml;
		};

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
		auto toInstanceArray() const -> std::shared_ptr<InstanceArray>
		{
			std::shared_ptr<InstanceArray> instanceArray(new InstanceArray());
			instanceArray->assign(count, instance);

			return instanceArray;
		};
	};
};
};