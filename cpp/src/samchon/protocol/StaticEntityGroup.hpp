#pragma once
#include <samchon/protocol/Entity.hpp>
#include <samchon/protocol/EntityGroupBase.hpp>

#include <algorithm>

namespace samchon
{
namespace protocol
{
	template <typename Container, typename Key = std::string>
	class StaticEntityGroup
		: public Container, // CONTAINS CHILD ENTITY ELEMENTS
		public virtual Entity<Key>, // I AM A TYPE OF ENTITY TOO
		public EntityGroupBase // INTERFACE
	{
	public:
		typedef Container container_type;

	public:
		/* ------------------------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------------------------ */
		using container_type::container_type;

		virtual void construct(std::shared_ptr<library::XML> xml)
		{
			clear();
			if (xml->has(CHILD_TAG()) == false)
				return;

			std::shared_ptr<library::XMLList> &xml_list = xml->get(CHILD_TAG());
			size_t i = 0;

			assign(xml_list->size(), container_type::value_type());
			for (auto it = begin(); it != end(); it++)
				it->construct(xml_list->at(i++));
		};

		/* ------------------------------------------------------------------------------------
			ACCESSORS
		------------------------------------------------------------------------------------ */
		using container_type::erase;

		void erase(const typename container_type::value_type::key_type &key)
		{
			for (auto it = begin(); it != end(); )
				if (it->key() == key)
					it = erase(it);
				else
					it++;
		};
		
		/**
		 * @brief Get iterator to element.
		 * 
		 * @details
		 * <p> Searches the container for an element with a identifier equivalent to <i>key</i> and returns an 
		 * iterator to it if found, otherwise it returns an iterator to {@link end end()}. </p>
		 *
		 * <p> Two keys are considered equivalent if the container's comparison object returns false reflexively 
		 * (i.e., no matter the order in which the elements are passed as arguments). </p>
		 *
		 * <p> Another member functions, {@link has has()} and {@link count count()}, can be used to just check 
		 * whether a particular <i>key</i> exists. </p>
		 *
		 * @param key Key to be searched for
		 * @return An iterator to the element, if an element with specified <i>key</i> is found, or 
		 *		   {@link end end()} otherwise.
		 */
		auto find(const typename container_type::value_type::key_type &key) -> typename container_type::iterator
		{
			return std::find_if
			(
				begin(), end(),
				[key](const container_type::value_type &entity) -> bool
				{
					return entity.key() == key;
				}
			);
		};

		/**
		 * @brief Get const iterator to element.
		 * 
		 * @details
		 * <p> Searches the container for an element with a identifier equivalent to <i>key</i> and returns an 
		 * iterator to it if found, otherwise it returns an iterator to {@link end end()}. </p>
		 *
		 * <p> Two keys are considered equivalent if the container's comparison object returns false reflexively 
		 * (i.e., no matter the order in which the elements are passed as arguments). </p>
		 *
		 * <p> Another member functions, {@link has has()} and {@link count count()}, can be used to just check 
		 * whether a particular <i>key</i> exists. </p>
		 *
		 * @param key Key to be searched for
		 * @return An iterator to the element, if an element with specified <i>key</i> is found, or 
		 *		   {@link end end()} otherwise.
		 */
		auto find(const typename container_type::value_type::key_type &key) const -> typename container_type::const_iterator
		{
			return std::find_if
			(
				begin(), end(),
				[key](const container_type::value_type &entity) -> bool
				{
					return entity.key() == key;
				}
			);
		};

		/**
		 * @brief Indicates whether a container has an object having the specified identifier. </p>
		 *
		 * @param key An identifier of an Entity
		 * @return If there's the object then true, otherwise false
		 */
		auto has(const typename container_type::value_type::key_type &key) const -> bool
		{
			return std::any_of
			(
				begin(), end(),
				[key](const container_type::value_type &entity) -> bool
				{
					return entity.key() == key;
				}
			);
		};

		/**
		 * @brief Count elements with a specific key.
		 * @details Searches the container for elements whose key is <i>key</i> and returns the number of elements found.
		 *
		 * @return The number of elements in the container with a <i>key</i>.
		 */
		auto count(const typename container_type::value_type::key_type &key) const -> size_t
		{
			return std::count_if
			(
				begin(), end(),
				[key](const container_type::value_type &entity) -> bool
				{
					return entity.key() == key;
				}
			);
		};

		/**
		 * @brief Access the element by specified identifier(key).
		 *
		 * @param key the identifier of the element wants to access
		 * @return The element having the key, or throw exception if there is none.
		 */
		auto get(const typename container_type::value_type::key_type &key) -> typename container_type::value_type&
		{
			auto it = std::find_if
				(
					begin(), end(),
					[key](const container_type::value_type &entity) -> bool
					{
						return entity.key() == key;
					}
				);

			if (it == end())
				throw std::out_of_range("out of range");

			return *it;
		};

		/**
		 * @brief Access the const element by specified identifier(key).
		 *
		 * @param key the identifier of the element wants to access
		 * @return The const element having the key, or throw exception if there is none.
		 */
		auto get(const typename container_type::value_type::key_type &key) const -> const typename container_type::value_type&
		{
			auto it = std::find_if
				(
					begin(), end(),
					[key](const container_type::value_type &entity) -> bool
					{
						return entity.key() == key;
					}
				);

			if (it == end())
				throw std::out_of_range("out of range");

			return *it;
		};

		/* ------------------------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------------------------ */
		/**
		 * @brief Get an XML object represents the EntityGroup
		 *
		 * <p> Archives the EntityGroup's own member variables only to the returned XML object. </p>
		 *
		 * <p> Do not consider about archiving children Entity objects' data in EntityGroup::toXML().
		 * Those children Entity objects will converted to XML object by their own toXML() method. The
		 * insertion of XML objects representing children are done by abstract method of
		 * EntityGroup::toXML(). </p>
		 *
		 * <p> Archives only data of EntityGroup's own. </p>
		 *
		 * \par [Inherited]
		 *		@copydoc Entity::toXML()
		 */
		virtual auto toXML() const -> std::shared_ptr<library::XML>
		{
			std::shared_ptr<library::XML> &xml = Entity::toXML();

			std::shared_ptr<library::XMLList> xmlList(new library::XMLList());
			xmlList->reserve(this->size());

			for (auto it = begin(); it != end(); it++)
				xmlList->push_back(it->toXML());

			xml->set(CHILD_TAG(), xmlList);
			return xml;
		};
	};
};
};