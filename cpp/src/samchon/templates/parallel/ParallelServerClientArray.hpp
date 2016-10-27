#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystemArray.hpp>
#include <samchon/templates/external/ExternalServerClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * Master of Parallel Processing System, be a server and client at the same time.
	 *
	 * The {@link ParallelServerClientArray} is an abstract class, derived from the {@link ParallelSystemArray} class, 
	 * opening a server accepting {@link ParallelSystem parallel clients} and being a client connecting to 
	 * {@link IParallelServer parallel servers} at the same time.
	 *
	 * Extends this {@link ParallelServerClientArray} and overrides below methods creating child {@link ParallelSystem}
	 * object. After the overridings, open server with {@link open open()} method and connect to 
	 * {@link IParallelServer parallel servers} through the {@link connect connect()} method. If you want this server to 
	 * follow web-socket protocol, then overrides {@link WebServer} virtually.
	 * 
	 * - {@link createExternalClient createExternalClient()}
	 * - {@link createExternalServer createExternalServer()}
	 * 
	 * #### [Inherited] {@link ParallelSystemArray}
	 * @copydetails parallel::ParallelSystemArray
	 */
	template <class System = ParallelSystem>
	class ParallelServerClientArray
		: public ParallelSystemArray<System>,
		public external::ExternalServerClientArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		ParallelServerClientArray()
			: ParallelSystemArray<System>(),
			external::ExternalServerClientArray<System>()
		{
		};
		virtual ~ParallelServerClientArray() = default;
	};
};
};
};