#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystem.hpp>
#include <samchon/templates/external/ExternalServer.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * A driver for parallel slave server.
	 * 
	 * The {@link ParallelServer} is an abstract class, derived from the {@link ParallelSystem} class, connecting to 
	 * remote, parallel **slave** server. Extends this {@link ParallelServer} class and overrides the 
	 * {@link createServerConnector createServerConnector()} method following which protocol the **slave** server uses.
	 * 
	 * #### [Inherited] {@link ParallelSystem}
	 * @copydetails parallel::ParallelSystem 
	 */
	class ParallelServer
		: public ParallelSystem,
		public external::ExternalServer
	{
	public:
		/**
		 * Construct from parent {@link ParallelSystemArray}.
		 * 
		 * @param systemArray The parent {@link ParallelSystemArray} object.
		 */
		ParallelServer(external::base::ExternalSystemArrayBase *systemArray)
			: ParallelSystem(systemArray),
			external::ExternalServer(systemArray),
			external::ExternalSystem(systemArray)
		{
		};
		virtual ~ParallelServer() = default;
	};
};
};
};