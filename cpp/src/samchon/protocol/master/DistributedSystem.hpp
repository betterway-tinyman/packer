#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/ExternalSystem.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class DistributedSystemArray;
	class DistributedSystemRole;

	class DSInvokeHistoryArray;
	class DSRoleHistoryList;

	/**
	 * @brief A network driver for a distributed system.
	 *
	 * @details
	 * <p> DistributedSystem class is an ExternalSystem having role as DistributedSystemRole objects
	 * with performance index and history log allocation of DistributedSystemRole objects and have
	 * requsted Invoke message to related external system. </p>
	 *
	 * <p> The greatest difference between DistributedSystem and ExternalSystem is, unlike the
	 * ExternalSystem class which having is ExternalSystemRole objects exclusively, DistributedSystem
	 * does not monopoly a DistributedSystemRole. The DistributedSystemRole can allocatedto multiple
	 * DistributedSystem.</p>
	 *
	 * <p> DistributedSystem and DistributedSystemRole classes have performance index and history log
	 * archiving what they've requested to the related distributed system. Allocating DistributedSystemRole
	 * objects to DistributedSyste will be determined those performance index and history log for
	 * optimize distribution of elasped time about each Invoke message processing. </p>
	 *
	 * <p> @image html  cpp/protocol_master_distributed_system.png
	 *	   @image latex cpp/protocol_master_distributed_system.png </p>
	 *
	 * @todo
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::ExternalSystem
	 */
	class SAMCHON_FRAMEWORK_API DistributedSystem
		: public virtual ExternalSystem
	{
		friend class DistributedSystemArray;

	private:
		typedef ExternalSystem super;

	protected:
		/* --------------------------------------------------------------------------------
			VARIABLES FOR ESTIMATING PERFORMANCE
		-------------------------------------------------------------------------------- */
		/**
		 * @brief A number of processes running on the distributed system.
		 *
		 * @details
		 * <p> Adds one count when sendData() is called and minus one count if a DSInvokeHistory,
		 * which means reported Invoke history, was reported by replyData(). </p>
		 *
		 * <p> If an DistributedSystemRole is allocated to multiple DistributedSystem objects, which
		 * DistributedSystem will be used is determined by the <i>process size</i> and performance
		 * index of the ExternalSystemRole. </p>
		 */
		size_t inProgress;

		/**
		 * @brief A number of processed have runned.
		 */
		size_t processed;

		/**
		 * @brief Average of elapsed time.
		 *
		 * @details
		 * <p> Average elapsed time of handling Invoke message (request) in frame work of tbe related
		 * distributed system. Not in frame work of distributed system's driver, DistributeDSystem. </p>
		 *
		 * <p> The ultimate optimization target of distributed processing system is to minimizing
		 * standard deviation of the average elapsed time of the DistributedSystem. </p>
		 */
		double avgElapsedTime;

		/* --------------------------------------------------------------------------------
			OBJECTS FOR HISTORIES
		-------------------------------------------------------------------------------- */
		/**
		 * @brief A performance index.
		 *
		 * @details
		 * <p> A performance index z is calculated by normalization calculating reverse number of
		 * whole distributed system's average elapsed times and its z value between the normal
		 * distribution. </p>
		 *
		 *	\li X = (¥ì + z¥ò)
		 *
		 * <p> If a distributed system has no history of handling Invoke message, then set the
		 * performance index to 0 as default. The performance index will be re-calcuated whenever
		 * re-allocation of roles is done (DistributedSystem::allocateRoles()). </p>
		 */
		double performance;

		/**
		 * @brief A list of history log for role allocations.
		 */
		DSRoleHistoryList *roleHistoryList;

		/**
		 * @brief A list of history log for reported Invoke messages.
		 */
		DSInvokeHistoryArray *invokeHistoryArray;

	public:
		/* ------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		DistributedSystem();
		virtual ~DistributedSystem();

		virtual void construct(std::shared_ptr<library::XML>) override;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->ExternalSystemRole* override;

	public:
		// GETTER
		SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(DistributedSystemRole)

			/* ------------------------------------------------------------------
				CHAIN OF INVOKE MESSAGE
			------------------------------------------------------------------ */
			virtual void sendData(std::shared_ptr<Invoke>) override;
		virtual void replyData(std::shared_ptr<Invoke>) override;

		/* ------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------ */
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};