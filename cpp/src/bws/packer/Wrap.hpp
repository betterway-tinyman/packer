#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>
#include <bws/packer/Instance.hpp>

namespace bws
{
namespace packer
{
	class WrapperGroup;
	class Wrapper;

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
		Wrap(Wrapper *wrapper)
			: super()
		{
			this->wrapper = wrapper;
		};

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
		Wrap(Wrapper *wrapper, std::shared_ptr<Instance> instance, double x, double y, double z, int orientation = 1)
			: Wrap(wrapper)
		{
			this->instance = instance;

			this->x = x;
			this->y = y;
			this->z = z;

			this->orientation = orientation;
		};

		virtual ~Wrap() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			// FETCHING WRAPPER OR PRODUCT OBJECT FROM THEIR KEY VALUE MUST BE SPECIFIED 
			// Wrapper->Packer->productArray

			x = xml->getProperty<double>("x");
			y = xml->getProperty<double>("y");
			z = xml->getProperty<double>("z");

			orientation = xml->getProperty<int>("orientation");
		};

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
		void setPosition(double x, double y, double z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		};

		/**
		 * Set orientation.
		 * 
		 * @param orientation Orientation code (1 to 6).
		 */
		void setOrientation(int val)
		{
			this->orientation = val;
		};

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
		void estimateOrientation(double width, double height, double length)
		{
			if (instance->getWidth() == width && instance->getHeight() == height)
				orientation = 1;
			else if (instance->getWidth() == length && instance->getHeight() == height)
				orientation = 2;
			else if (instance->getWidth() == length && instance->getHeight() == width)
				orientation = 3;
			else if (instance->getWidth() == height && instance->getHeight() == width)
				orientation = 4;
			else if (instance->getWidth() == width && instance->getHeight() == length)
				orientation = 5;
			else
				orientation = 6;
		};

		/**
		 * @brief Orientation change is occured in level of the packer.
		 *
		 * @details orientation Packer's new orientation.
		 */
		void changeWrapperOrientation(int orietation)
		{
			if (orientation == 1)
				return;

			// DECLARES
			double x, y, z;
			double width, height, length;

			if (orientation == 2)
			{
				width = instance->getLength();
				height = instance->getHeight();
				length = instance->getWidth();

				x = this->z;
				y = this->y;
				z = ((Instance*)wrapper)->getWidth() - (length + this->x);
			}
			else if (orientation == 3)
			{
				width = instance->getLength();
				height = instance->getWidth();
				length = instance->getHeight();

				x = this->z;
				y = ((Instance*)wrapper)->getWidth() - (height + this->x);
				z = ((Instance*)wrapper)->getHeight() - (length + this->y);
			}
			else if (orientation == 4)
			{
				width = instance->getHeight();
				height = instance->getWidth();
				length = instance->getLength();

				x = this->y;
				y = this->x;
				z = this->z;
			}
			else if (orientation == 5)
			{
				width = instance->getWidth();
				height = instance->getLength();
				length = instance->getHeight();

				x = this->x;
				y = ((Instance*)wrapper)->getLength() - (height + this->z);
				z = this->y;
			}
			else
			{
				width = instance->getHeight();
				height = instance->getLength();
				length = instance->getWidth();

				x = this->y;
				y = this->z;
				z = ((Instance*)wrapper)->getWidth() - (length - this->x);
			}

			this->estimateOrientation(width, height, length);
			this->x = x;
			this->y = y;
			this->z = z;
		};

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
		auto getWrapper() const -> Wrapper*
		{
			return wrapper;
		};

		/**
		 * @brief Get instance.
		 */
		auto getInstance() const -> std::shared_ptr<Instance>
		{
			return instance;
		};

		/**
		 * @brief Get orientation.
		 */
		auto getOrientation() const -> int
		{
			return orientation;
		};

		/**
		 * @brief Get x.
		 */
		auto getX() const -> double
		{
			return x;
		};
		
		/**
		 * @brief Get y.
		 */
		auto getY() const -> double
		{
			return y;
		};
		
		/**
		 * @brief Get z.
		 */
		auto getZ() const -> double
		{
			return z;
		};

		/**
		 * @brief Get volume.
		 */
		auto getVolume() const -> double
		{
			return instance->getVolume();
		};

		/* -----------------------------------------------------------
			DERIVED PROPERTIES
		----------------------------------------------------------- */
		/**
		 * @brief Get (calculate) width.
		 * 
		 * @details 제품의 실제 가로가 아닌, 포장지 내 배치되는 방향에 따른 가로 사이즈를 리턴한다.
		 */
		auto getWidth() const -> double
		{
			switch (orientation)
			{
			case 1: case 5:
				return instance->getWidth();
			case 3: case 4:
				return instance->getHeight();
			default: // 2, 5
				return instance->getLength();
			}
		};

		/**
		 * @brief Get (calculate) height.
		 *
		 * @details 제품의 실제 높이가 아닌, 포장지 내 배치되는 방향에 따른 높이 사이즈를 리턴한다.
		 */
		auto getHeight() const -> double
		{
			switch (orientation)
			{
			case 1: case 2:
				return instance->getHeight();
			case 4: case 6:
				return instance->getWidth();
			default: // 3, 5
				return instance->getLength();
			}
		};

		/**
		 * @brief Get (calculate) length.
		 *
		 * @details 제품의 실제 길이가 아닌, 포장지 내 배치되는 방향에 따른 길이 사이즈를 리턴한다.
		 */
		auto getLength() const -> double
		{
			switch (orientation)
			{
			case 1: case 4:
				return instance->getLength();
			case 2: case 3:
				return instance->getWidth();
			default: // 5, 6
				return instance->getHeight();
			}
		};

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "wrap";
		};
		
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = super::toXML();
			xml->setProperty("instance", instance->key());

			// 나중에 없애야 함
			xml->push_back(instance->toXML());

			xml->setProperty("x", x);
			xml->setProperty("y", y);
			xml->setProperty("z", z);
			xml->setProperty("orientation", orientation);

			return xml;
		};
	};
};
};