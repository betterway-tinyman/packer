#pragma once
#include <samchon/API.hpp>

/**
 * @brief Macro writing writing inline method of comparison operators
 *
 * @details
 * <p> OPERATOR_METHODS_INLINE is a macro realizing all compare operators by only writing
 * operator< and operator== methods. </p>
 *	\li [!=] -> [NOT ==]
 *	\li [<=] -> [< OR ==]
 *	\li [>] -> [NOT < AND NOT ==]
 *	\li [>=] -> [NOT <]
 *
 * @warning Methods operator< and operator== must be written. </p>
 *
 * @param _Ty Type of parameter, what to compare
 */
#define OPERATOR_METHODS_INLINE(_Ty) \
auto operator!=(const _Ty &obj) const -> bool \
{ \
	return !operator==(obj); \
}; \
auto operator<=(const _Ty &obj) const -> bool \
{ \
	return operator<(obj) || operator==(obj); \
}; \
auto operator>(const _Ty &obj) const -> bool \
{ \
	return !operator<=(obj); \
}; \
auto operator>=(const _Ty &obj) const -> bool \
{ \
	return !operator<(obj); \
};

/**
 * @brief Macro writing header of comparison operators' method
 *
 * @details
 * <p> OPERATOR_METHODS_HEADER is a macro realizing all compare operators by only writing
 * operator< and operator== methods. </p>
 *	\li [!=] -> [NOT ==]
 *	\li [<=] -> [< OR ==]
 *	\li [>] -> [NOT < AND NOT ==]
 *	\li [>=] -> [NOT <]
 *
 * @warning Methods operator< and operator== must be written. </p>
 *
 * @param _Ty Type of parameter, what to compare
 */
#define OPERATOR_METHODS_HEADER(_Ty) \
auto operator!=(const _Ty &obj) const -> bool; \
auto operator<=(const _Ty &obj) const -> bool; \
auto operator>(const _Ty &obj) const -> bool; \
auto operator>=(const _Ty &obj) const -> bool;

/**
 * @brief Macro writing body of comparison operators' method
 *
 * @details
 * <p> OPERATOR_METHODS_BODY is a macro realizing all compare operators by only writing
 * operator< and operator== methods. </p>
 *	\li [!=] -> [NOT ==]
 *	\li [<=] -> [< OR ==]
 *	\li [>] -> [NOT < AND NOT ==]
 *	\li [>=] -> [NOT <]
 *
 * @warning Methods operator< and operator== must be written. </p>
 *
 * @param _Class The class operator methods are belonged to
 * @param _Ty Type of parameter, what to compare
 *
 * @author Jeongho Nam
 */
#define OPERATOR_METHODS_BODY(_Class, _Ty) \
auto _Class::operator!=(const _Ty &obj) const -> bool \
{ \
	return !operator==(obj); \
} \
auto _Class::operator<=(const _Ty &obj) const -> bool \
{ \
	return operator<(obj) || operator==(obj); \
} \
auto _Class::operator>(const _Ty &obj) const -> bool \
{ \
	return !operator<=(obj); \
} \
auto _Class::operator>=(const _Ty &obj) const -> bool \
{ \
	return !operator<(obj); \
}

namespace samchon
{
namespace library
{
	/**
	 * @brief Interface for comparision operator
	 *
	 * @details
	 * <p> IOperator is a template class makes enalbe to realizing all compare operators by 
	 * only overriding operator< and operator== methods. </p>
	 *	\li [!=] -> [NOT ==]
	 *	\li [<=] -> [< OR ==]
	 *	\li [>] -> [NOT < AND NOT ==]
	 *	\li [>=] -> [NOT <]
	 *
	 * <p> Methods operator< and operator== are abstract. Overrides them. </p>
	 *
	 * @note 
	 * If you want to realize all comparison operators without inheritance and specifying
	 * clear type of parameter, you can use following macros instead.
	 *
	 *	\li OPERATOR_METHODS_INLINE(_Ty)
	 *	\li OPERATOR_METHODS_HEADER(_Ty)
	 *	\li OPERATOR_METHODS_BODY(_Class, _Ty)
	 *
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	template <typename _Ty>
	class IOperator
	{
	public:
		/**
		 * @brief Default Constructor
		 */
		IOperator()
		{
		};
		virtual ~IOperator() = default;

		/**
		 * @brief Less
		 * @details Overrides method less.
		 *
		 * @param obj The object to compare.
		 * @return The object is less than this.
		 */
		virtual auto operator<(const IOperator&) const -> bool = 0;

		/**
		 * @brief Equal
		 * @details Overrides methods equal.
		 *
		 * @param obj The object to compare.
		 * @return The object is equal with this.
		 */
		virtual auto operator==(const IOperator&) const -> bool = 0;

		auto operator!=(const IOperator<_Ty> &obj) const -> bool
		{
			return !operator==(obj);
		};
		auto operator<=(const IOperator<_Ty> &obj) const -> bool
		{
			return operator<(obj) || operator==(obj);
		};
		auto operator>(const IOperator<_Ty> &obj) const -> bool
		{
			return !operator<(obj) && !operator==(obj);
		};
		auto operator>=(const IOperator<_Ty> &obj) const -> bool
		{
			return !operator<(obj);
		};
	};
};
};