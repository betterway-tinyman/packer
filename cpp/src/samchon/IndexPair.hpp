#pragma once

#include <utility>

namespace samchon
{
	/**
	 * @brief A pair of index and its value(T)
	 * @tparam T Type of value.
	 *
	 * @details
	 * <p> IndexPair is a std::pair<size_t, T> class. </p>
	 *
	 * <p> IndexPair expresses index and value. It's useful for expressing finding literals,
	 * specified object, calculating mininum or maximum with its position. </p>
	 *
	 * \par [Inherited] 
	 * <p> This class couples together a pair of values, which may be of different 
	 * types (T1 and T2). The individual values can be accessed through its public members 
	 * first and second. </p>
	 *
	 * <p> Pairs are a particular case of tuple. </p>
	 *
	 * <p> Referenced comments of std::pair </p>
	 *	\li http://www.cplusplus.com/reference/utility/pair/
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	template<class T>
	class IndexPair
		: public std::pair<size_t, T>
	{
	private:
		typedef std::pair<size_t, T> super;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		//using super::super;
		
		/**
		 * @brief Default Constructor.
		 */
		IndexPair()
			: super() 
		{
		};

		/**
		 * @brief Copy Constructor.
		 */
		template<class U> IndexPair(const IndexPair<U> &obj)
			: super(obj)
		{
		};

		/**
		 * @brief Move Constructor.
		 */
		template<class U> IndexPair(const IndexPair<U> &&obj)
			: super(obj)
		{
		};

		/**
		 * @brief Construct from members.
		 */
		IndexPair(size_t index, const T &val)
			: super(index, val)
		{
		};

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Get index
		 *
		 * @return Index of the pair
		 */
		auto get_index() const -> size_t
		{
			return first;
		};

		/**
		 * @brief Get reference of value
		 *
		 * @return Reference of value of the pair
		 */
		auto getValue() -> T&
		{
			return second;
		};

		/**
		 * @brief Get const reference of const value
		 *
		 * @return Const reference of value of the pair
		 */
		auto getValue() const -> const T&
		{
			return second;
		};
	};
};