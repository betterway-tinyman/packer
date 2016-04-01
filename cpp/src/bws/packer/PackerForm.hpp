#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace samchon
{
	namespace library
	{
		class GAParameters;
	};
};

namespace bws
{
namespace packer
{
	class InstanceFormArray;
	class WrapperArray;
	class Packer;

	/**
	 * @brief Bridge of Packer for repeated instances.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class PackerForm
		: public protocol::Entity
	{
	private:
		typedef protocol::Entity super;

		std::shared_ptr<InstanceFormArray> instanceFormArray;
		std::shared_ptr<WrapperArray> wrapperArray;
		std::shared_ptr<library::GAParameters> gaParameters;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		PackerForm();
		virtual ~PackerForm() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* -----------------------------------------------------------
			ACCESSORS
		----------------------------------------------------------- */
		auto optimize() const -> std::shared_ptr<WrapperArray>;

		auto getGAParameters() const -> std::shared_ptr<library::GAParameters>;

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;

		virtual auto toXML() const -> std::shared_ptr<library::XML>;
		virtual auto toPacker() const -> std::shared_ptr<Packer>;
	};
};
};