#pragma once
#include <bws/packer/API.hpp>

#include <samchon/protocol/Entity.hpp>

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
		: public virtual protocol::Entity
	{
	private:
		typedef protocol::Entity super;

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
		Instance();

		/**
		 * @brief Copy Constructor.
		 */
		Instance(const Instance &);

		/**
		* @brief Construct from members.
		*
		* @param name 인스턴스명
		* @param width 가로 (x축)
		* @param height 세로 (y축)
		* @param length 길이 (z축)
		*/
		Instance(const std::string &, double, double, double);
		virtual ~Instance() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * Key of an Instance is its name.
		 */
		virtual auto key() const -> std::string override;

		/**
		 * @brief Get name.
		 */
		auto getName() const -> std::string;

		/**
		 * @brief Get width, length on the X-axis in 3D.
		 */
		auto getWidth() const -> double;
		
		/**
		 * @brief Get height, length on the Y-axis in 3D.
		 */
		auto getHeight() const -> double;
		
		/**
		 * Get length, length on the Z-axis in 3D.
		 */
		auto getLength() const -> double;
		
		/**
		 * @brief Get (calculate) volume.
		 *
		 * @return width x height x length
		 */
		auto getVolume() const -> double;

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
		virtual auto TAG() const -> std::string override;

		virtual auto toXML() const -> std::shared_ptr<library::XML> override;;
	};
};
};