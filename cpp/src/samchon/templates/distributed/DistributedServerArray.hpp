#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/distributed/DistributedSystemArray.hpp>
#include <samchon/templates/external/ExternalServerArray.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	/**
	 * Master of Distributed Processing System, a client connecting to slave servers.
	 *
	 * The {@link DistributedServerArray} is an abstract class, derived from the {@link DistributedSystemArray} class,
	 * connecting to {@link IDistributedServer distributed servers}.
	 *
	 * Extends this {@link DistributedServerArray} and overrides {@link createChild createChild()} method creating child
	 * {@link IDistributedServer} object. After the extending and overriding, construct children {@link IDistributedServer}
	 * objects and call the {@link connect connect()} method.
	 * 
	 * #### [Inherited] {@link DistributedSystemArray}
	 * @copydetails distributed::DistributedSystemArray
	 */
	template <class System = DistributedServer>
	class DistributedServerArray
		: public DistributedSystemArray<System>,
		public external::ExternalServerArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		DistributedServerArray()
			: DistributedSystemArray<System>(),
			external::ExternalServerArray<System>()
		{
		};
		virtual ~DistributedServerArray() = default;
	};
};
};
};