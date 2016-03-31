#pragma once

#include <map>

namespace samchon
{
	/**
	 * @brief Customized std::map.
	 *
	 * @details
	 * <p> Map is a std::map some methods are modified. </p>
	 *
	 * <ul>
	 *	<li>Addicted methods</li>
	 *	<ul>
	 *		<li> has(find(key) != end()) method is addicted </li>
	 *		<li> at method is addicted </li>
	 *		<li> set method is addicted </li>
	 *		<li> pop method is addicted </li>
	 *	</ul>
	 *	<li>Modified methods</li>
	 *	<ul>
	 *		<li> at was deprecated (get is different with at) </li>
	 *	</ul>
	 * </ul>
	 *
	 * \par [Inherited] 
	 * <p>Maps are associative containers that store elements formed by a combination of 
	 * a key value and a mapped value, following a specific order. </p>
	 *
	 * <p> In a map, the key values are generally used to sort and uniquely identify the elements, 
	 * while the mapped values store the content associated to this key. The types of key and 
	 * mapped value may differ, and are grouped together in member type value_type, which is 
	 * a pair type combining both: </p>
	 *
	 * \li typedef pair<const Key, T> value_type;
	 *
	 * <p> Internally, the elements in a map are always sorted by its key following a specific 
	 * strict weak ordering criterion indicated by its internal comparison object (of type Compare). </p>
	 *
	 * <p> map containers are generally slower than unordered_map containers to access individual 
	 * elements by their key, but they allow the direct iteration on subsets based on their order. </p>
	 *
	 * <p> Maps are typically implemented as binary search trees. </p>
	 *
	 * <p> The mapped values in a map can be accessed directly by their corresponding key 
	 * using the bracket operator ((operator[]). </p>
	 * 
	 * <p> Maps are typically implemented as binary search trees. </p>
	 *
	 * Referenced comments of std::map
	 *	\li http://www.cplusplus.com/reference/map/map/
	 * 
	 * @tparam _Kty 
	 * <p> Type of the keys. Each element in a map is uniquely identified by its key value. </p>
	 * <p> Aliased as member type map::key_type. </p>
	 *
	 * @tparam _Ty 
	 * <p> Type of the mapped value. Each element in a map stores some data as its mapped value. </p>
	 * <p> Aliased as member type map::mapped_type. </p>
	 *
	 * @tparam _Pr 
	 * <p> A binary predicate that takes two element keys as arguments and returns a bool. 
	 * The expression comp(a,b), where comp is an object of this type and a and b are key values, 
	 * shall return true if a is considered to go before b in the strict weak ordering the 
	 * function defines. </p>
	 *
	 * <p> The map object uses this expression to determine both the order the elements follow 
	 * in the container and whether two element keys are equivalent (by comparing them reflexively: 
	 * they are equivalent if !comp(a,b) && !comp(b,a)). No two elements in a map container 
	 * can have equivalent keys. </p>
	 *
	 * <p> This can be a function pointer or a function object. This defaults to std::less<T>, 
	 * which returns the same as applying the less-than operator (a<b). </p>
	 *
	 * <p> Aliased as member type map::key_compare. </p>
	 *
	 * @tparam _Alloc 
	 * <p> Type of the allocator object used to define the storage allocation model. By default, 
	 * the allocator class template is used, which defines the simplest memory allocation model 
	 * and is value-independent. </p>
	 *
	 * <p> Aliased as member type map::allocator_type. </p>
	 *
	 * @author Jeongho Nam
	 */
	template <typename _Kty, typename _Ty, typename _Pr = std::less<_Kty>, typename _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
	class Map
		: public std::map<_Kty, _Ty, _Pr, _Alloc>
	{
	private:
		typedef std::map<_Kty, _Ty, _Pr, _Alloc> super;

		//PROHIBIT ACCESS TO AT
		auto at(const _Kty &) -> _Ty& = delete;
		auto at(const _Kty &) const -> const _Ty& = delete;

	public:
		using super::super;

		/**
		 * @brief Whether have the item or not
		 *
		 * @details
		 * Indicates whether a map has an item having the specified identifier.
		 *
		 * @param key Key value of the element whose mapped value is accessed.
		 * @return Whether the map has an item having the specified identifier
		 */
		auto has(const _Kty &key) const -> bool
		{
			return count(key) != 0;
		};

		/**
		 * @brief Get element
		 *
		 * @details
		 * Returns a reference to the mapped value of the element identified with key
		 *
		 * @warning
		 * get is different with std::map's at. 
		 * get does not create object but throws excention if the matched key doesn't exist.
		 *
		 * @param key Key value of the element whose mapped value is accessed.
		 * @throw exception out of range
		 * @return A reference object of the mapped value (_Ty)
		 */
		auto get(const _Kty &key) -> _Ty&
		{
			return find(key)->second;
		};
		auto get(const _Kty &key) const -> const _Ty&
		{
			return find(key)->second;
		};

		/**
		 * @brief Set element
		 *
		 * @details
		 * <p> Set an item as the specified identifier. </p>
		 * 
		 * <p>If the identifier is already in map, change value of the identifier. <br/>
		 * If not, then insert the object with the identifier. </p>
		 * 
		 * @param key Key value of the element whose mapped value is accessed.
		 * @param val Value, the item.
		 */
		void set(const _Kty &key, const _Ty &val)
		{
			iterator it = find(key);
			if (it != end())
				erase(it);

			insert({ key, val });
		};
		void set(const _Kty &key, const _Ty &&val)
		{
			iterator it = find(key);
			if (it != end())
				erase(it);
				
			insert({ key, val });
		};

		/**
		 * @brief Pop item
		 * @details Removes an item having specified key and returns the removed element.
		 *
		 * @return An item released by pop
		 */
		auto pop(const _Kty &key) -> _Ty
		{
			iterator it = find(key);
			_Ty val = it->second;

			erase(it);

			return std::move(val);
		};
	};
};