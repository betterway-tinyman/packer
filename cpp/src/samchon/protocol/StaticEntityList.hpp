#pragma once
#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/IEntityGroup.hpp>
#include <list>

#include <samchon/library/XML.hpp>
#include <memory>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An Entity and a static list containing Entity objects
	 *
	 * @tparam T A type of children Entity. Must be a class derived from an Entity.
	 *
	 * @details
	 * <p> StaticEntityList is a static array containing children objects derived from an Entity class.
	 *
	 * <p> StaticEntityList, its allocated memory space for each child is fixed and cannot contain derived a 
	 * entity class from the child entity type. It has advantages on performance by direct accessment and
	 * static memory allocation for child, however, its increasement of performance is not eminent. </p>
	 *
	 *	\li StaticEntityList<Animal> is specified
	 *	\li Dog is extended from the Animal
	 *	\li StaticEntityList<Animal> cannot contain Dog. You can try insertion of the Dog class into the StaticEntityList, 
	 *		but only memory size of Animal is allocated, so that derived member variables and methods are all
	 *		truncated.
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::Entity
	 *
	 * @warning
	 * <p> If a data structure has recursive and hierarchical relationship, never use the StaticEntityList.
	 * StaticEntityList's memory allocation is static. The recursive relationship causes infinite memory
	 * allocation. It must generates stack overflow. </p>
	 *
	 * <p> Use EntityGroup instead. </p>
	 *	\li class NTCriteria : public StaticEntityList<NTCriteria> causes a stack overflow.
	 *	\li class NTCriteria : public SharedEntityList<NTCriteria> doesn't cause the stack overflow.
	 *
	 * @warning
	 * <p> Unlike StaticEntityArray containing children serialized, StaticEntityList's static memory allocation and
	 * accessment for a child, its advantage doesn't seemed to have eminent advantage on performance. As 
	 * there's not meaningful advantage but has only disadvantage that cannot containing derived objects, 
	 * I'm planning to depreciate the StaticEntityList in next generation. </p>
	 *
	 * <p> Use SharedEntityList instead will be better I think. </p>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template <typename T>
	class StaticEntityList
		: public virtual Entity, public std::list<T>, //CLASSES
		public virtual IEntityGroup //INTERFACE
	{
	private:
		typedef std::list<T> container_type;
		typedef T entity_type;

	public:
		/**
		 * @brief Default Constructor
		 */
		using container_type::container_type;
		
		/**
		 * @brief Construct data of the Entity from an XML object
		 *
		 * <p> Constructs the StaticEntityList's own member variables only from the input XML object. </p>
		 *
		 * <p> Do not consider about constructing children Entity objects' data in StaticEntityList::construct().
		 * Those children Entity objects' data will constructed by their own construct() method. Even insertion
		 * of XML objects representing children are done by abstract method of StaticEntityList::toXML(). </p>
		 *
		 * <p> Constructs only data of StaticEntityList's own. </p>
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
			assign(xmlList->size(), T());

			for (size_t i = 0; i < xmlList->size(); i++)
				at(i).construct(xmlList->at(i));
		}

		/**
		 * @brief Get an XML object represents the StaticEntityList
		 *
		 * <p> Archives the StaticEntityList's own member variables only to the returned XML object. </p>
		 *
		 * <p> Do not consider about archiving children Entity objects' data in StaticEntityList::toXML().
		 * Those children Entity objects will converted to XML object by their own toXML() method. The
		 * insertion of XML objects representing children are done by abstract method of
		 * StaticEntityList::toXML(). </p>
		 *
		 * <p> Archives only data of StaticEntityList's own. </p>
		 *
		 * \par [Inherited]
		 *		@copydoc Entity::toXML()
		 */
		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> &xml = Entity::toXML();

			std::shared_ptr<library::XMLList> xmlList(new library::XMLList());
			xmlList->reserve(this->size());

			for (size_t i = 0; i < size(); i++)
				xmlList->push_back(at(i).toXML());

			xml->set(CHILD_TAG(), xmlList);
			return xml;
		};
	};
};
};