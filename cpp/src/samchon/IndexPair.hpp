#pragma once
#include <utility>

namespace samchon
{
	/**
	 * @brief 
	 * A pair of index and its value(_Ty)
	 *
	 * @details
	 * <p> IndexPair is a std::pair<size_t, _Ty> class. </p>
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
	 * @author Jeongho Nam
	 */
	template<class _Ty>
	class IndexPair
		: private std::pair<size_t, _Ty>
	{
	private:
		typedef std::pair<size_t, _Ty> super;

	public:
		/**
		 * @brief Inherited constructors
		 */
		using super::super;
		
		/**
		 * @brief Get index
		 *
		 * @return Index of the pair
		 */
		auto getIndex() const -> size_t
		{
			return super::first;
		};

		/**
		 * @brief Get const value
		 *
		 * @return Reference of value of the pair
		 */
		auto getValue() -> _Ty&
		{
			return super::second;
		};

		/**
		 * @brief Get value
		 *
		 * @return Const reference of value of the pair
		 */
		auto getValue() const -> const _Ty&
		{
			return super::second;
		};
	};
};