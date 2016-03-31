#pragma once
#include <bws/packer/API.hpp>

#include <memory>
#include <bws/packer/Wrapper.hpp>

namespace bws
{
namespace packer
{
	class WrapperVolume
	{
	private:
		/**
		 * @brief Related wrapper.
		 */
		std::shared_ptr<Wrapper> wrapper;
		
		/**
		 * Left, un-filled volume.
		 */
		double left;

		/**
		 * @brief Cost (price) per a unit of volume.
		 *
		 * @details Wrapper's (price) / (containable volume).
		 */
		double cost;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from a Wrapper.
		 */
		WrapperVolume(std::shared_ptr<Wrapper> wrapper)
		{
			this->wrapper = wrapper;

			left = wrapper->getContainableVolume();
			cost = wrapper->getPrice() / wrapper->getContainableVolume();
		};

		/**
		 * @brief Copy Constructor.
		 */
		WrapperVolume(const WrapperVolume &obj)
			: WrapperVolume(obj.wrapper)
		{
		};

		/**
		 * @brief Allocate an instance.
		 */
		auto allocate(std::shared_ptr<Instance> instance) -> bool
		{
			if (instance->getVolume() > left)
				return false;

			left -= instance->getVolume();
			return true;
		};

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get wrapper.
		 */
		auto getWrapper() const -> std::shared_ptr<Wrapper>
		{
			return wrapper;
		};

		/**
		 * @brief Get (calculate) priority.
		 *
		 * @return Calculated priority, lower is better.
		 */
		auto getPriority(std::shared_ptr<Instance> instance) const -> double
		{
			return cost;

			if (left >= instance->getVolume())
				return cost * ((left - instance->getVolume()) / wrapper->getContainableVolume());
			else
				return cost * ((wrapper->getContainableVolume() - instance->getVolume()) / wrapper->getContainableVolume());
		};
	};
};
};