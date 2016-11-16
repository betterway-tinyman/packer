#pragma once
#include <bws/packer/API.hpp>

#include <bws/packer/WrapperArray.hpp>

#include <bws/packer/InstanceArray.hpp>

#include <array>
#include <algorithm>
#include <boxologic/Boxologic.hpp>

namespace bws
{
namespace packer
{
	/**
	 * @brief A group of {@link Wrapper Wrappers} with same type.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class WrapperGroup
		: public WrapperArray
	{
	private:
		typedef WrapperArray super;

	protected:
		/**
		 * @brief A sample, standard of the group.
		 *
		 * @details The sample represents what type of Wrappers are grouped into the WrapperGroup.
		 */
		std::shared_ptr<Wrapper> sample;

		/**
		 * @brief Allocated instances.
		 */
		std::shared_ptr<InstanceArray> allocatedInstanceArray;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		WrapperGroup()
		{
			sample = nullptr;
			allocatedInstanceArray.reset(new InstanceArray());
		};

		/** 
		 * @brief Construct from memebers of sample.
		 *
		 * @param name Name, identifier of the sample.
		 * @param price Price, issued cost for a type of the sample.
		 * @param width Width, dimensional length on the X-axis in 3D, of the sample.
		 * @param height Height, dimensional length on the Y-axis in 3D, of the sample.
		 * @param length Length, dimensional length on the Z-axis in 3D, of the sample.
		 * @param thickness A thickness, causes shrinkness on containable volume, of the sample.
		 */
		WrapperGroup(const std::string &name, double price, double width, double height, double length, double thickness)
			: WrapperGroup()
		{
			sample.reset(new Wrapper(name, price, width, height, length, thickness));
		};
		
		/**
		 * @brief Construct from a sample Wrapper.
		 *
		 * @param sample A sample, standard of the WrapperGroup.
		 */
		WrapperGroup(std::shared_ptr<Wrapper> sample)
			: WrapperGroup()
		{
			this->sample = sample;
		};

		virtual ~WrapperGroup() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			super::construct(xml);

			if (xml->hasProperty("width"))
			{
				if (sample == nullptr)
					sample.reset(new Wrapper());

				sample->construct(xml);
			}
		};

	public:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		virtual auto key() const -> std::string override
		{
			if (sample == nullptr)
				return "";
			else
				return sample->key();
		};

		/**
		 * @brief Get sample.
		 *
		 * @return A sample, standard of the group.
		 */
		auto getSample() const -> std::shared_ptr<Wrapper>
		{
			return sample;
		};

		/**
		 * @brief Get allocated instances.
		 *
		 * @return Allocated instances.
		 */
		auto getAllocatedInstanceArray() const -> std::shared_ptr<InstanceArray>
		{
			return allocatedInstanceArray;
		};

		/**
		 * Get (calculate) price.
		 *
		 * @return (Price of the sample) x (numbers of children Wrappers)
		 */
		virtual auto getPrice() const -> double override
		{
			if (sample == nullptr)
				return 0.0;
			else
				return sample->getPrice() * size();
		};

		virtual auto getUtilization() const -> double override
		{
			double utilization = 0.0;
			for (size_t i = 0; i < size(); i++)
				utilization += at(i)->getUtilization();

			return utilization / (double)size();
		};

		/* -----------------------------------------------------------
			OPERATORS
		----------------------------------------------------------- */
		/**
		 * @brief Allocate instance(s) to the WrapperGroup.
		 *
		 * @details
		 * <p> Inspect the instance is enough small to be wrapped into an empty wrapper. 
		 * If the instance is enough small, registers the instance (or repeated instances) and returns true. 
		 * If the instance is too large to be capsuled, returns false. </p>
		 *
		 * @note
		 * <p> The word <u>the instance is enough small to be wrapped into the empty wrapper</u> means 
		 * the instance can be contained into an empty, a new wrapper contaning nothing literally. </p>
		 *
		 * <p> In the method allocate(), it doesn't consider how many instances are wrapped into ordinary 
		 * wrapper and how much volumes are consumed.  </p>
		 * 
		 * @param instance An Instance to allocate.
		 * @param n Repeating number of the <i>instance</i>.
		 *
		 * @return Whether the instance is enough small to be wrapped into a (new) wrapper 
		 *		   of same type with the sample.
		 */
		auto allocate(std::shared_ptr<Instance> instance, size_t n = 1) -> bool
		{
			// TEST WHETHER A PRODUCT IS NOT LARGER THAN BOX
			if (sample->operator>=(*instance) == false)
				return false;

			// INSERTS TO THE RESERVED ITEMS
			this->allocatedInstanceArray->insert
			(
				allocatedInstanceArray->end(),
				n, instance
			);

			return true;
		};
		
		/**
		 * @brief Run optimization in level of the group.
		 *
		 * @details 
		 * <p> The optimization routine begins by creating a Wrapper like the sample. Then try to pack 
		 * allocated instances (allocatedInstanceArray) to the Wrapper as a lot as possible. If there're some 
		 * Wrappers can't be packed by overloading, then create a new Wrapper again and try to pack instances 
		 * as a lot as possible again, too. </p>
		 *
		 * <p> Repeats those steps until all instances are packed so that there's not any instance left. </p>
		 *
		 * @warning 
		 * When call this optimize() method, ordinary children Wrapper objects in the WrapperGroup
		 * will be substituted with the newly optimized Wrapper objects.
		 */
		void optimize()
		{
			// CLEAR PREVIOUS OPTIMIZATION
			this->clear();

			// CONSTRUCT INSTANCES FROM RESERVEDS
			std::shared_ptr<InstanceArray> instanceArray = this->allocatedInstanceArray;

			// UNTIL UNPACKED INSTANCE DOES NOT EXIST
			while (instanceArray->empty() == false)
				instanceArray = this->pack(instanceArray);
		};

	protected:
		/**
		 * @brief Wrap allocated instances into <b>a new</b> Wrapper.
		 *
		 * @details
		 * <p> Wraps instances to <b>a new</b> Wrapper which is copied from the sample. </p>
		 *
		 * <p> After the wrapping is done, <b>the new</b> Wrapper is registered to the WrapperGroup as a 
		 * child and instances failed to wrap by overloading is returned. </p>
		 *
		 * @param instanceArray Instances to wrap into a new Wrapper.
		 *
		 * @return Instances failed to wrap by overloading.
		 */
		virtual auto pack(std::shared_ptr<InstanceArray> instanceArray) -> std::shared_ptr<InstanceArray>
		{
			boxologic::Boxologic adaptor(std::make_shared<Wrapper>(*sample), instanceArray);
			auto pair = adaptor.pack();

			this->push_back(pair.first);
			return pair.second;
		};

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "wrapperGroup";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = super::toXML();

			if (sample != nullptr)
			{
				xml->insertAllProperties(sample->toXML());

				xml->eraseProperty("type");
				xml->setProperty("price", getPrice());
				xml->setProperty("utilization", getUtilization());
			}

			return xml;
		};
	};
};
};