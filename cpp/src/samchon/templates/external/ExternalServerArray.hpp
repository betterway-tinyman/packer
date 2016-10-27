#pragma once
#include <samchon/API.hpp>

#include <samchon/templates/external/ExternalSystemArray.hpp>
#	include <samchon/templates/external/ExternalServer.hpp>

#include <thread>

namespace samchon
{
namespace templates
{
namespace external
{
	/**
	 * An array and manager of {@link IExternalServer external servers}.
	 *
	 * The {@link ExternalServerArray} is an abstract class, derived from the {@link ExternalSystemArray} class,
	 * connecting to {@link IExternalServer external servers}.
	 *
	 * Extends this {@link ExternalServerArray} and overrides {@link createChild createChild()} method creating child
	 * {@link IExternalServer} object. After the extending and overriding, construct children {@link IExternalServer}
	 * objects and call the {@link connect connect()} method.
	 * 
	 * #### [Inherited] {@link ExternalSystemArray}
	 * @copydetails external::ExternalSystemArray
	 */
	template <class System = ExternalServer>
	class ExternalServerArray
		: public virtual ExternalSystemArray<System>
	{
	public:
		/**
		 * Default Constructor.
		 */
		ExternalServerArray()
			: ExternalSystemArray<System>()
		{
		};
		virtual ~ExternalServerArray() = default;

		/**
		 * Connect to {@link ExternalServer external servers}.
		 * 
		 * This method calls children elements' method {@link ExternalServer.connect} gradually.
		 */
		virtual void connect()
		{
			std::vector<std::thread> thread_array;

			for (size_t i = 0; i < size(); i++)
			{
				auto external_server = std::dynamic_pointer_cast<ExternalServer>(this->at(i));
				if (external_server == nullptr)
					continue;

				thread_array.emplace_back(&ExternalServer::connect, external_server.get());
			}

			for (size_t i = 0; i < thread_array.size(); i++)
				thread_array[i].join();
		};
	};
};
};
};