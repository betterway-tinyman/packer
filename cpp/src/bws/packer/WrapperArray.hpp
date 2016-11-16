#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#	include <bws/packer/Wrapper.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief An array of Wrapper objects.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WrapperArray
		: public protocol::SharedEntityArray<Wrapper>
	{
	private:
		typedef protocol::SharedEntityArray<Wrapper> super;
	
	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		using super::super;
		virtual ~WrapperArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>) -> Wrapper* override
		{
			return new Wrapper();
		};

	public:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get (calculate) price.
		 */
		virtual auto getPrice() const -> double
		{
			double price = 0.0;
			for (size_t i = 0; i < size(); i++)
				price += at(i)->getPrice();

			return price;
		};

		/**
		 * @brief Get (calculate) utilization rate.
		 */
		virtual auto getUtilization() const -> double
		{
			if (this->empty() == true)
				return 0.0;

			double numerator = 0.0;
			double denominator = 0.0;

			for (size_t i = 0; i < size(); i++)
			{
				const auto &wrapper = at(i);

				for (size_t j = 0; j < wrapper->size(); j++)
					numerator += wrapper->at(j)->getVolume();

				denominator += wrapper->getContainableVolume();
			}

			return numerator / denominator;
		};

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "wrapperArray";
		};
		virtual auto CHILD_TAG() const -> std::string override
		{
			return "instance";
		};
	};
};
};