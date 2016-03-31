#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>
#include <samchon/library/EventDispatcher.hpp>

#include <samchon/library/GAParameters.hpp>

namespace bws
{
namespace packer
{
	class GAWrapperArray;
	class WrapperArray;
	class InstanceArray;

	/**
	 * @brief Packer, packing optimization solver.
	 *
	 * @details Facade class
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Packer
		: public protocol::Entity,
		public library::EventDispatcher
	{
	private:
		typedef protocol::Entity super;
		typedef library::EventDispatcher event_super;

	protected:
		/**
		 * @brief Type of Wrapper(s).
		 */
		std::shared_ptr<WrapperArray> wrapperArray;

		/**
		 * @brief Instance(s) to be packed (wrapped).
		 */
		std::shared_ptr<InstanceArray> instanceArray;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		Packer();

		/**
		 * @brief Construct from members. 
		 *
		 * @param wrapperArray Candidate wrappers who can contain instances.
		 * @param instanceArray Instances to be packed into some wrappers.
		 */
		Packer(std::shared_ptr<WrapperArray>, std::shared_ptr<InstanceArray>);
		virtual ~Packer() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

	public:
		/* -----------------------------------------------------------
			GETTERSE
		----------------------------------------------------------- */
		/**
		 * @brief Get Wrappers.
		 */
		auto getWrapperArray() const -> std::shared_ptr<WrapperArray>;

		/**
		 * @brief Get Instances.
		 */
		auto getInstanceArray() const -> std::shared_ptr<InstanceArray>;

		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		/**
		 * @brief Get an optimized packing solution.
		 *
		 * @param gaParams Parameters of genetic algorithm.
		 */
		auto optimize(const library::GAParameters & = {500, 50, 20, .2}) const -> std::shared_ptr<WrapperArray>;

	protected:
		/**
		 * @brief Initialize sequence list (gene_array).
		 *
		 * @return Initial sequence list.
		 */
		virtual auto initGenes() const -> std::shared_ptr<GAWrapperArray>;

		/**
		 * @briefTry to repack each wrappers to another type.
		 *
		 * @param $wrappers Wrappers to repack.
		 * @return Re-packed wrappers.
		 */
		virtual auto repack(const std::shared_ptr<WrapperArray>) const -> std::shared_ptr<WrapperArray>;

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const->std::string override;
		
		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};