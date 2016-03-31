#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/SlaveSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class DistributedSystemArrayMediator;

	/**
	 * @brief A distributed slave system for mediation.
	 *
	 * @details
	 * <p> DistributedSlaveSystemMediator is an abstract class derived from slave::SlaveSystem
	 * for realizing tree-structured distriuted processins system. DistributedSlaveSystemMediator
	 * acts a slave system's role for its master, on the above level. </p>
	 *
	 * <p> DistributedSystemArrayMediator and DistributedSlaveSystemMediator classes are paired to
	 * act a mediator system role by acting their own role, master system role for below slaves
	 * and slave system role for above master system. </p>
	 *
	 * <p> @image html  conception/distributed_system_array_mediator.png
	 *	   @image latex conception/distributed_system_array_mediator.png </p>
	 *
	 * <p> @image html  cpp/protocol_master_distributed_system.png
	 *	   @image latex cpp/protocol_master_distributed_system.png </p>
	 *
	 * \par [Inherited]
	 *		@copydetails slave::SlaveSystem
	 */
	class SAMCHON_FRAMEWORK_API DistributedSlaveSystemMediator
		: public virtual slave::SlaveSystem
	{
	private:
		typedef slave::SlaveSystem super;

	protected:
		/**
		 * @brief A master system for mediation
		 */
		DistributedSystemArrayMediator *master;

	public:
		/**
		 * @brief Default Constructor.
		 */
		DistributedSlaveSystemMediator();
		virtual ~DistributedSlaveSystemMediator() = default;

		virtual void replyData(std::shared_ptr<Invoke>) override;

	public:
		virtual auto TAG() const->std::string override;
	};
};
};
};