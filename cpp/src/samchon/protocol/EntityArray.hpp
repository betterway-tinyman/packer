#pragma once
#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IEntityGroup.hpp>
#include <vector>

#include <samchon/library/XML.hpp>
#include <memory>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An Entity and a static array containing Entity objects
	 *
	 * @tparam _Ty A type of children Entity. Must be a class derived from an Entity.
	 *
	 * @details
	 * <p> EntityArray is a static array containing children objects derived from an Entity class.
	 *
	 * <p> The EntityArray has advantages of performance like fast iteration or fast access because the
	 * EntityArray is serialized. However, as EntityArray is serialized, allocated memory space for each
	 * child is fixed and cannot contain derived a entity class from the child entity type. </p>
	 *
	 *	\li EntityArray<Animal> is specified
	 *	\li Dog is extended from the Animal
	 *	\li EntityArray<Animal> cannot contain Dog. You can try insertion of the Dog class into the EntityArray, 
	 *		but only memory size of Animal is allocated, so that derived member variables and methods are all
	 *		truncated.
	 *
	 * <p> Thus, it's suitable for when fast accessment and iteration like an entity class containing
	 * mathmatical or statistical data and methods which doesn't seemed to be inherited. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::Entity
	 *
	 * @warning
	 * <p> If a data structure has recursive and hierarchical relationship, never use the EntityArray.
	 * EntityArray's memory allocation is static. The recursive relationship causes infinite memory
	 * allocation. It must generates stack overflow. </p>
	 *
	 * <p> Use EntityGroup instead. </p>
	 *	\li class NTCriteria : public EntityArray<NTCriteria> causes a stack overflow.
	 *	\li class NTCriteria : public SharedEntityArray<NTCriteria> doesn't cause the stack overflow.
	 */
	template <typename _Ty>
	class EntityArray
		: public virtual Entity, public std::vector<_Ty>, //CLASSES
		public virtual IEntityGroup //INTERFACE
	{
	protected:
		typedef Entity super;
		typedef _Ty entity_type;

	public:
		/**
		 * @brief Default Constructor
		 */
		EntityArray() {};
		virtual ~EntityArray() = default;

		/**
		 * @brief Construct data of the Entity from an XML object
		 *
		 * <p> Constructs the EntityArray's own member variables only from the input XML object. </p>
		 *
		 * <p> Do not consider about constructing children Entity objects' data in EntityArray::construct().
		 * Those children Entity objects' data will constructed by their own construct() method. Even insertion
		 * of XML objects representing children are done by abstract method of EntityArray::toXML(). </p>
		 *
		 * <p> Constructs only data of EntityArray's own. </p>
		 *
		 * \par [Inherited]
		 *		@copydoc Entity::construct()
		 */
		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			clear();
			if (xml->has(CHILD_TAG()) == false)
				return;

			std::shared_ptr<library::XMLList> &xmlList = xml->get(CHILD_TAG());
			assign(xmlList->size());

			for (size_t i = 0; i < xmlList->size(); i++)
				at(i).construct(xmlList->at(i));
		}

		/**
		 * @brief Get an XML object represents the EntityArray
		 *
		 * <p> Archives the EntityArray's own member variables only to the returned XML object. </p>
		 *
		 * <p> Do not consider about archiving children Entity objects' data in EntityArray::toXML().
		 * Those children Entity objects will converted to XML object by their own toXML() method. The
		 * insertion of XML objects representing children are done by abstract method of
		 * EntityArray::toXML(). </p>
		 *
		 * <p> Archives only data of EntityArray's own. </p>
		 *
		 * \par [Inherited]
		 *		@copydoc Entity::toXML()
		 */
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> &xml = super::toXML();

			std::shared_ptr<library::XMLList> xmlList(new XMLList());
			xmlList->reserve(this->size());

			for (size_t i = 0; i < size(); i++)
				xmlList->push_back(at(i).toXML());

			xml->set(CHILD_TAG(), xmlList);
			return xml;
		};
	};
};
};