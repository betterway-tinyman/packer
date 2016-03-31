#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/slave/ParallelSystem.hpp>

namespace samchon
{
	namespace protocol
	{
		namespace master
		{
			class ParallelSystemArrayMediator;

			/**
			 * @brief A parallel slave system for mediation.
			 *
			 * @details
			 * <p> ParallelSlaveSystemMediator is an abstract class derived from slave::ParallelSystem
			 * for realizing tree-structured distriuted processins system. ParallelSlaveSystemMediator
			 * acts a slave system's role for its master, on the above level. </p>
			 *
			 * <p> ParallelSystemArrayMediator and ParallelSlaveSystemMediator classes are paired to
			 * act a mediator system role by acting their own role, master system role for below slaves 
			 * and slave system role for above master system. </p>
			 *
			 * <p> @image html  conception/parallel_system_array_mediator.png
			 *	   @image latex conception/parallel_system_array_mediator.png </p>
			 *
			 * <p> @image html  cpp/protocol_master_parallel_system.png
			 *	   @image latex cpp/protocol_master_parallel_system.png </p>
			 *
			 * \par [Inherited]
			 *		@copydetails slave::SlaveSystem
			 */
			class SAMCHON_FRAMEWORK_API ParallelSlaveSystemMediator
				: public virtual slave::ParallelSystem
			{
				friend class ParallelSystemArrayMediator;

			private:
				typedef slave::ParallelSystem super;

				/**
				 * @brief A master system for mediation
				 */
				ParallelSystemArrayMediator *master;

			public:
				/**
				 * @brief Default Constructor.
				 */
				ParallelSlaveSystemMediator();
				virtual ~ParallelSlaveSystemMediator() = default;

				virtual void replyPieceData(std::shared_ptr<Invoke>, size_t, size_t) override;

			public:
				virtual auto TAG() const -> std::string override;
			};
		};
	};
};