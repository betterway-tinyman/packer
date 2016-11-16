#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <bws/packer/InstanceFormArray.hpp>
#include <bws/packer/WrapperArray.hpp>
#include <bws/packer/Packer.hpp>

#include <samchon/library/GAParameters.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief Bridge of Packer for repeated instances.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PackerForm
		: public protocol::Entity<>
	{
	private:
		typedef protocol::Entity<> super;

		std::shared_ptr<InstanceFormArray> instanceFormArray;
		std::shared_ptr<WrapperArray> wrapperArray;
		std::shared_ptr<library::GAParameters> gaParameters;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		PackerForm()
			: super()
		{
			instanceFormArray.reset(new InstanceFormArray());
			wrapperArray.reset(new WrapperArray());
			gaParameters.reset(new library::GAParameters());
		};
		virtual ~PackerForm() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			instanceFormArray->construct(xml->get(instanceFormArray->TAG())->at(0));
			wrapperArray->construct(xml->get(wrapperArray->TAG())->at(0));

			if (xml->has(gaParameters->TAG()))
				gaParameters->construct(xml->get(gaParameters->TAG())->at(0));
			else
				gaParameters.reset(new library::GAParameters());
		};

		/* -----------------------------------------------------------
			ACCESSORS
		----------------------------------------------------------- */
		auto optimize() const -> std::shared_ptr<WrapperArray>
		{
			auto &packer = this->toPacker();

			return packer->optimize(*gaParameters);
		};

		/**
		 * @brief Get parameters of genetic algorithm.
		 */
		auto getGAParameters() const -> std::shared_ptr<library::GAParameters>
		{
			return gaParameters;
		};

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "packerForm";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML>
		{
			auto xml = super::toXML();
			xml->push_back(instanceFormArray->toXML());
			xml->push_back(wrapperArray->toXML());
			xml->push_back(gaParameters->toXML());

			return xml;
		};

		virtual auto toPacker() const -> std::shared_ptr<Packer>
		{
			return std::make_shared<Packer>(wrapperArray, instanceFormArray->toInstanceArray());
		};
	};
};
};