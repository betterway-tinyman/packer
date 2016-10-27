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
	 * @brief Packer, a solver of 3d bin packing with multiple wrappers.
	 *
	 * @details 
	 * <p> Packer is a facade class supporting packing operations in user side. You can solve a packing problem
	 * by constructing Packer class with {@link WrapperArray wrappers} and {@link InstanceArray instances} to 
	 * pack and executing Packer::optimize() method. </p>
	 *
	 * <p> During the optimization process by Packer::optimize(), ProgressEvent will be thrown. If you want
	 * to listen the ProgressEvent, register a listener function by #addEventListener() method. </p>
	 *
	 * <p> In background side, deducting packing solution, those algorithms are used. </p>
	 * <ul>
	 *	<li> <a href="http://betterwaysystems.github.io/packer/reference/AirForceBinPacking.pdf" target="_blank">
	 *		Airforce Bin Packing; 3D pallet packing problem: A human intelligence-based heuristic approach </a>
	 *	</li>
	 *	<li> Genetic Algorithm </li>
	 *	<li> Greedy and Back-tracking algorithm </li>
	 * </ul>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Packer
		: public protocol::Entity<>,
		public library::EventDispatcher
	{
	private:
		typedef protocol::Entity<> super;
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
		 * @brief Compute an optimized packing solution.
		 *
		 * @details
		 * 
		 *
		 * @param gaParams Parameters of genetic algorithm.
		 */
		auto optimize(const library::GAParameters & = {500, 50, 20, .2}) const -> std::shared_ptr<WrapperArray>;

	protected:
		/**
		 * @brief Initialize sequence list (gene_array).
		 *
		 * @details
		 * <p> Deducts initial sequence list by such assumption: </p>
		 * 
		 * <ul>
		 *	<li> Cost of larger wrapper is less than smaller one, within framework of price per volume unit. </li>
		 *	<ul>
		 *		<li> Wrapper Larger: (price: $1,000, volume: 100cm^3 -> price per volume unit: $10 / cm^3) </li>
		 *		<li> Wrapper Smaller: (price: $700, volume: 50cm^3 -> price per volume unit: $14 / cm^3) </li>
		 *		<li> Larger's <u>cost</u> is less than Smaller, within framework of price per volume unit </li>
		 *	</ul>
		 * </ul>
		 *
		 * <p> Method #initGenes() construct {@link WrapperGroup WrapperGroups} corresponding with #wrapperArray
		 * and allocates {@link #instanceArray instances} to a WrapperGroup, has the smallest <u>cost</u> between 
		 * containbles. </p>
		 *
		 * <p> After executing packing solution by WrapperGroup::optimize(), trying to re-pack each WrapperGroup
		 * to another type of Wrapper, deducts the best solution between them. It's the initial sequence list
		 * of genetic algorithm. </p>
		 *
		 * @return Initial sequence list.
		 */
		virtual auto initGenes() const -> std::shared_ptr<GAWrapperArray>;

		/**
		 * @brief Try to repack each wrappers to another type.
		 *
		 * @details Re-packs instances who are packed in the $wrappers to another type of Wrappers. 
		 *			A type of Wrapper with the smallest cost will be returned, containing the instances.
		 *
		 * @param $wrappers Wrappers to repack.
		 * @return Re-packed wrappers.
		 */
		virtual auto repack(const std::shared_ptr<WrapperArray>) const -> std::shared_ptr<WrapperArray>;

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const-> std::string override;
		
		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};