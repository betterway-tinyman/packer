#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystemArray.hpp>
#include <samchon/templates/external/ExternalClientArray.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * Master of Parallel Processing System, a server accepting slave clients.
	 * 
	 * The {@link ParallelClientArray} is an abstract class, derived from the {@link ParallelSystemArray} class, opening 
	 * a server accepting {@link ParallelSystem parallel clients}.
	 * 
	 * Extends this {@link ParallelClientArray}, overrides {@link createExternalClient createExternalClient()} creating 
	 * child {@link ParallelSystem} object. After the extending and overridings, open this server using the 
	 * {@link open open()} method. If you want this server to follow web-socket protocol, then overrides {@link WebServer} 
	 * virtually.
	 * 
	 * #### [Inherited] {@link ParallelSystemArray}
	 * @copydetails parallel::ParallelSystemArray
	 */
	template <class System = ParallelSystem>
	class ParallelClientArray
		: public ParallelSystemArray<System>,
		public external::ExternalClientArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		ParallelClientArray()
			: ParallelSystemArray<System>(),
			external::ExternalClientArray<System>()
		{
		};
		virtual ~ParallelClientArray() = default;
	};
};
};
};