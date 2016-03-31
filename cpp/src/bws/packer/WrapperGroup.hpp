#pragma once
#include <bws/packer/API.hpp>

#include <bws/packer/WrapperArray.hpp>

namespace bws
{
namespace packer
{
	class Wrapper;
	class InstanceArray;

	/**
	 * @brief A group of Wrapper(s) with same type.
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
		WrapperGroup();

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
		WrapperGroup(const std::string &, double, double, double, double, double);
		
		/**
		 * @brief Construct from a sample Wrapper.
		 *
		 * @param sample A sample, standard of the WrapperGroup.
		 */
		WrapperGroup(std::shared_ptr<Wrapper>);

		virtual ~WrapperGroup() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

	public:
		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		virtual auto key() const -> std::string override;

		/**
		 * @brief Get sample.
		 *
		 * @return A sample, standard of the group.
		 */
		auto getSample() const -> std::shared_ptr<Wrapper>;

		/**
		 * @brief Get allocated instances.
		 *
		 * @return Allocated instances.
		 */
		auto getAllocatedInstanceArray() const -> std::shared_ptr<InstanceArray>;

		/**
		 * Get (calculate) price.
		 *
		 * @return (Price of the sample) x (numbers of children Wrappers)
		 */
		virtual auto getPrice() const -> double override;

		virtual auto getUtilization() const -> double override;

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
		auto allocate(std::shared_ptr<Instance>, size_t n = 1) -> bool;
		
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
		void optimize();

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
		virtual auto pack(std::shared_ptr<InstanceArray>) -> std::shared_ptr<InstanceArray>;

	public:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;

		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};