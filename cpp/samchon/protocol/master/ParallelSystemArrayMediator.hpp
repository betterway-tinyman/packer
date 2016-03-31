#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/ParallelSystemArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSlaveSystemMediator;

	/**
	 * @brief A mediator of parallel system between master and slaves.
	 *
	 * @details
	 * <p> ParallelSystemArrayMediator class is used to realizing a mediator system of
	 * tree-structured parallel processing system. ParallelSystemArrayMediator intermediates
	 * between origin-master(ParallelSystemArray) and its slaves(slave::ParallelSystem). </p>
	 *
	 * <p> ParallelSystemArrayMediator is a master(ParallelSystemArray) within the framework
	 * of slaves systems (slave::ParallelSystem objects) and also can be a slave system
	 * (slave::ParallelSystem) within framework of its master(ParallelSystemArray) by having
	 * ParallelSlaveSystemMediator. </p>
	 *
	 * @todo
	 * Sends PRMasterHistory to its origin master.
	 *
	 * <p> @image html  conception/distributed_system_array_mediator.png
	 * 	   @image latex conception/distributed_system_array_mediator.png </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::ParallelSystemArray
	 */
	class SAMCHON_FRAMEWORK_API ParallelSystemArrayMediator
		: public virtual ParallelSystemArray
	{
	private:
		typedef ParallelSystemArray super;

	protected:
		/**
		 * @brief A slave system for mediation.
		 */
		ParallelSlaveSystemMediator *slave;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		ParallelSystemArrayMediator();
		virtual ~ParallelSystemArrayMediator();

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* ------------------------------------------------------------------
			PROCESS
		------------------------------------------------------------------ */
		virtual void start() override;

		/* ------------------------------------------------------------------
			CHAIN OF INVOKE MESSAGE
		------------------------------------------------------------------ */
		//virtual void sendPieceData(std::shared_ptr<Invoke>, size_t, size_t);

	protected:
		virtual void notifyEnd(PRMasterHistory*);

	public:
		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};