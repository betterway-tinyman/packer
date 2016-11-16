#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <array>
#include <algorithm>

namespace bws
{
namespace packer
{
	/**
	 * @brief An abstract class of a physical 3D-instance.
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Instance
		: public virtual protocol::Entity<std::string>
	{
	private:
		typedef protocol::Entity<std::string> super;

	protected:
		/**
		 * @brief Name, key of the Instance.
		 */
		std::string name;

		/**
		 * @brief Width of the Instance, length on the X-axis in 3D.
		 */
		double width;

		/**
		 * @brief Height of the Instance, length on the Y-axis in 3D.
		 */
		double height;

		/**
		 * @brief Length of the Instance, length on the Z-axis in 3D.
		 */
		double length;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		Instance() 
			: Instance("", 0, 0, 0)
		{
		};

		/**
		 * @brief Copy Constructor.
		 */
		Instance(const Instance &instance)
			: Instance(instance.name, instance.width, instance.height, instance.length)
		{
		};

		/**
		* @brief Construct from members.
		*
		* @param name 인스턴스명
		* @param width 가로 (x축)
		* @param height 세로 (y축)
		* @param length 길이 (z축)
		*/
		Instance(const std::string &name, double width, double height, double length)
			: super()
		{
			this->name = name;

			this->width = width;
			this->height = height;
			this->length = length;
		};
		virtual ~Instance() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			name = xml->getProperty<std::string>("name");

			width = xml->getProperty<double>("width");
			height = xml->getProperty<double>("height");
			length = xml->getProperty<double>("length");
		};

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Key of an Instance is its name.
		 */
		virtual auto key() const -> std::string override
		{
			return name;
		};

		/**
		 * @brief Get name.
		 */
		auto getName() const -> std::string
		{
			return name;
		};

		/**
		 * @brief Get width, length on the X-axis in 3D.
		 */
		auto getWidth() const -> double
		{
			return width;
		};
		
		/**
		 * @brief Get height, length on the Y-axis in 3D.
		 */
		auto getHeight() const -> double
		{
			return height;
		};
		
		/**
		 * Get length, length on the Z-axis in 3D.
		 */
		auto getLength() const -> double
		{
			return length;
		};
		
		/**
		 * @brief Get (calculate) volume.
		 *
		 * @return width x height x length
		 */
		auto getVolume() const -> double
		{
			return width * height * length;
		};

	protected:
		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
		/**
		 * @brief A type, identifier of derived class. 
		 *
		 * @details Derived types
		 *	\li {@link Product product}
		 *	\li {@link Wrapper wrapper}
		 */
		virtual auto TYPE() const -> std::string = 0;

	public:
		virtual auto TAG() const -> std::string override
		{
			return "instance";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			auto xml = super::toXML();
			xml->setProperty("type", TYPE());
			xml->setProperty("name", name);

			xml->setProperty("width", width);
			xml->setProperty("height", height);
			xml->setProperty("length", length);

			return xml;
		};
	};
};
};