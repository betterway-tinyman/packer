#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

namespace bws
{
namespace packer
{
	class Wrapper;
	class Instance;

	class WrapperGroup;

	/**
	 * @brief An act wrap(ping).
	 *
	 * <p> Wrap is a class represents an act wrapping an Instance to a Wrapper.
	 * To represent the relationship, Wrap uses Bridge and Capsular patterns to links and 
	 * intermediates relationship between Wrapper and Instance. </p>
	 *
	 * <p> Wrap also helps packing optimization and 3d-visualization with its own members 
	 * orientation and position variables x, y and z. </p> 
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Wrap
		: public protocol::Entity<>
	{
		friend class WrapperGroup;

	private:
		typedef protocol::Entity<> super;

	protected:
		/**
		 * @brief A wrapper wrapping the instance.
		 *
		 * @details The wrapping is done in the wrapper.
		 */
		Wrapper *wrapper;

		/**
		 * @brief An instance wrapped into the wrapper.
		 *
		 * @details Not only a Product, but also a Wrapper also can be wrapped in the greater wrapper.
		 */
		std::shared_ptr<Instance> instance;

		/**
		 * Coordinate-X of the instance placement in the wrapper.
		 */
		double x;
		
		/**
		 * Coordinate-Y of the instance placement in the wrapper.
		 */
		double y;
		
		/**
		 * Coordinate-Z of the instance placement in the wrapper.
		 */
		double z;
		
		/**
		  Placement orientation of wrapped instance.
		 */
		int orientation;

	public:
		/* ===========================================================
			CONSTRUCTORS AND RELATEDS
				- CONSTRUCTORS
				- SETTERS
				- ESTIMATERS
		==============================================================
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from a Wrapper.
		 *
		 * @param wrapper A wrapper who will contain an instance.
		 */
		Wrap(Wrapper*);

		/**
		 * @brief Construct from a Wrapper and Instance with its position and orientation. 
		 *
		 * @param wrapper A wrapper who contains the instance.
		 * @param instance An instance contained into the wrapper.
		 * @param x Coordinate-X of the instance placement in the wrapper.
		 * @param y Coordinate-Y of the instance placement in the wrapper.
		 * @param z Coordinate-Z of the instance placement in the wrapper.
		 * @param orientation Placement orientation of wrapped instance.
		 */
		Wrap(Wrapper*, std::shared_ptr<Instance>, double, double, double, int = 1);

		virtual ~Wrap() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* ===========================================================
			SETTERS
				- MEMBERS
				- ESTIMATERS
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		/**
		 * Set position.
		 *
		 * @param x Coordinate-X of the instance placement in the wrapper.
		 * @param y Coordinate-Y of the instance placement in the wrapper.
		 * @param z Coordinate-Z of the instance placement in the wrapper.
		 */
		void setPosition(double, double, double);

		/**
		 * Set orientation.
		 * 
		 * @param orientation Orientation code (1 to 6).
		 */
		void setOrientation(int);

		/* -----------------------------------------------------------
			ESTIMATERS
		----------------------------------------------------------- */
		/**
		 * @brief Estimate orientation by given size.
		 *
		 * @param width Width by placement.
		 * @param height Height by placement.
		 * @param length Length by placement.
		 */
		void estimateOrientation(double, double, double);

		/**
		 * @brief Orientation change is occured in level of the packer.
		 *
		 * @details orientation Packer's new orientation.
		 */
		void changeWrapperOrientation(int orietation);

		/* ===========================================================
			GETTERS
				- MEMBERS
				- DERIVED PROPERTIES
		==============================================================
			MEMBERS
		----------------------------------------------------------- */
		/**
		 * @brief Get wrapper.
		 */
		auto getWrapper() const -> Wrapper*;

		/**
		 * @brief Get instance.
		 */
		auto getInstance() const -> std::shared_ptr<Instance>;

		/**
		 * @brief Get orientation.
		 */
		auto getOrientation() const -> int;

		/**
		 * @brief Get x.
		 */
		auto getX() const -> double;
		
		/**
		 * @brief Get y.
		 */
		auto getY() const -> double;
		
		/**
		 * @brief Get z.
		 */
		auto getZ() const -> double;

		/**
		 * @brief Get volume.
		 */
		auto getVolume() const -> double;

		/* -----------------------------------------------------------
			DERIVED PROPERTIES
		----------------------------------------------------------- */
		/**
		 * @brief Get (calculate) width.
		 * @details 제품의 실제 가로가 아닌, 포장지 내 배치되는 방향에 따른 가로 사이즈를 리턴한다.
		 */
		auto getWidth() const -> double;

		/**
		 * @brief Get (calculate) height.
		 *
		 * @details 제품의 실제 높이가 아닌, 포장지 내 배치되는 방향에 따른 높이 사이즈를 리턴한다.
		 */
		auto getHeight() const -> double;

		/**
		 * @brief Get (calculate) length.
		 *
		 * @details 제품의 실제 길이가 아닌, 포장지 내 배치되는 방향에 따른 길이 사이즈를 리턴한다.
		 */
		auto getLength() const -> double;

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override;
		
		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};