#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/DistributedSystemArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class DistributedSlaveSystemMediator;

	/**
	 * @brief A mediator of distributed system between master and slaves.
	 *
	 * @details
	 * <p> DistributedSystemArrayMediator class is used to realizing a mediator system of
	 * tree-structured distributed processing system. DistributedSystemArrayMediator intermediates
	 * between origin-master(DistributedSystemArray) and its slaves(slave::DistributedSystem). </p>
	 *
	 * <p> DistributedSystemArrayMediator is a master(DistributedSystemArray) within the framework
	 * of slaves systems (slave::DistributedSystem objects) and also can be a slave system
	 * (slave::DistributedSystem) within framework of its master(DistributedSystemArray) by having
	 * DistributedSlaveSystemMediator. </p>
	 *
	 * <p> @image html  conception/distributed_system_array_mediator.png
	 *	   @image latex conception/distributed_system_array_mediator.png </p>
	 *
	 * \par [Inherited]
	 *		@copydetails master::DistributedSystemArray
	 */
	class SAMCHON_FRAMEWORK_API DistributedSystemArrayMediator
		: public virtual DistributedSystemArray
	{
	private:
		typedef DistributedSystemArray super;

	protected:
		/**
		 * @brief A slave system for mediation.
		 */
		DistributedSlaveSystemMediator *slave;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		DistributedSystemArrayMediator();
		virtual ~DistributedSystemArrayMediator();

		virtual void construct(std::shared_ptr<library::XML>) override;

	protected:
		/**
		 * @brief Factory method of slave system for mediation.
		 *
		 * @throw invalid_parameter If DistributedServerArrayMediator, slave must be a type of
		 *							DistributedSlaveServerMediator
		 */
		virtual auto createSlave() const->DistributedSlaveSystemMediator* = 0;

		/* ------------------------------------------------------------------
			PROCESS
		------------------------------------------------------------------ */
	public:
		virtual void start() override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};