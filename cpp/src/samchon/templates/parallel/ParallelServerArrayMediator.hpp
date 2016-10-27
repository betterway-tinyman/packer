#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/parallel/ParallelSystemArrayMediator.hpp>
#include <samchon/templates/external/ExternalServerArray.hpp>

#include <samchon/templates/parallel/ParallelServer.hpp>

namespace samchon
{
namespace templates
{
namespace parallel
{
	/**
	 * Mediator of Parallel Processing System, a client connecting to slave servers.
	 *
	 * The {@link ParallelServerArrayMediator} is an abstract class, derived from the {@link ParallelSystemArrayMediator} 
	 * class, connecting to {@link IParallelServer parallel servers}.
	 *
	 * Extends this {@link ParallelServerArrayMediator} and overrides {@link createChild createChild()} method creating 
	 * child {@link IParallelServer} object. After the extending and overriding, construct children
	 * {@link IParallelServer} objects and call the {@link connect connect()} method.
	 * 
	 * #### [Inherited] {@link ParallelSystemArrayMediator}
	 * @copydetails parallel::ParallelSystemArrayMediator
	 */
	template <class System = ParallelServer>
	class ParallelServerArrayMediator
		: public ParallelSystemArrayMediator<System>,
		public external::ExternalServerArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		ParallelServerArrayMediator()
			: ParallelSystemArrayMediator<System>(),
			external::ExternalServerArray<System>()
		{
		};
		virtual ~ParallelServerArrayMediator() = default;
	};
};
};
};