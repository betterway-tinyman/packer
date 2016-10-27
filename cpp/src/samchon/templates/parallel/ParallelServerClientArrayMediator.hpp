#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystemArrayMediator.hpp>
#include <samchon/templates/external/ExternalServerClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * Mediator of Parallel Processing System, be a server and client at the same time as a **master**.
	 *
	 * The {@link ParallelServerClientArrayMediator} is an abstract class, derived from the 
	 * {@link ParallelSystemArrayMediator} class, opening a server accepting {@link ParallelSystem parallel clients} and 
	 * being a client connecting to {@link IParallelServer parallel servers} at the same time.
	 *
	 * Extends this {@link ParallelServerClientArrayMediator}, overrides {@link createExternalClient createExternalClient()} 
	 * creating child {@link ParallelSystem} object. After the extending and overridings, open this server using the 
	 * {@link open open()} method. If you want this server to follow web-socket protocol, then overrides {@link WebServer} 
	 * virtually.
	 *
	 * - {@link createServerBase createServerBase()}
	 * - {@link createExternalClient createExternalClient()}
	 * - {@link createExternalServer createExternalServer()}
	 *
	 * #### [Inherited] {@link ParallelSystemArrayMediator}
	 * @copydetails parallel::ParallelSystemArrayMediator
	 */
	template <class System = ParallelSystem>
	class ParallelServerClientArrayMediator
		: public ParallelSystemArrayMediator<System>,
		public external::ExternalServerClientArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		ParallelServerClientArrayMediator()
			: ParallelSystemArray<System>(),
			external::ExternalServerClientArray<System>()
		{
		};
		virtual ~ParallelServerClientArrayMediator() = default;
	};
};
};
};