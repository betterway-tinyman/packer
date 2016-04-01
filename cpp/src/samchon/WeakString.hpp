#pragma once
#include <samchon/API.hpp>

#include <samchon/library/IOperator.hpp>

#include <string>
#include <vector>
#include <samchon/IndexPair.hpp>

namespace samchon
{
	/**
	 * @brief A string class only references characeters, reference only
	 *
	 * @details
	 * <p> WeakSring does not consider any construction, modification and destruction of characters. </p>
	 * Thus, you can have greater advantages than std::string on the side of performance and memory,
	 * but of course, you can't modify the characeters at all. </p>
	 *
	 * @image html cpp/subset/library_string.png
	 * @image latex cpp/subset/library_string.png
	 *
	 * @warning
	 * \li WeakString not copy(strcpy) characeters but only references characeters.
	 *	   Be careful about destruction of the characters being referenced by the WeakString
	 * \li WeakString will be used for basic data type in most case.
	 *	   Avoid to use WeakString by pre-definition in header (*.hpp)
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API WeakString
	{
	private:
		/**
		 * @brief An array containing whitespaces
		 */
		static const std::vector<std::string> SPACE_ARRAY;

	public:
		/**
		 * @brief Maximum value for size_t
		 * @details
		 * <p> npos is a static member constant value with the greatest possible value for 
		 * an element of type size_t. </p>
		 *
		 * <p> This value, when used as the value for a len (or sublen) parameter in string's member functions, means "until the end of the string".
		 * As a return value, it is usually used to indicate no matches. </p>
		 * 
		 * <p> This constant is defined with a value of -1, which because size_t is an unsigned integral 
		 * type, it is the largest possible representable value for this type. </p>
		 */
		static const size_t npos = -1;

	private:
		/**
		 * @brief Referenced characters's pointer of begining position
		 */
		const char *data_;

		/**
		 * @brief (Specified) size of referenced characters
		 */
		size_t size_;

	public:
		/* --------------------------------------------------------------------
			CONSTRUCTORS
		-------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor does not reference any character
		 * @details Constructs an empty string, with zero size
		 */
		WeakString();

		/**
		 * @brief
		 * Constructor by characters with specified size
		 *
		 * @details
		 * <p> Constructs by characters to be referenced with limited size. </p>
		 *
		 * <p> Although the original size of data is over the specified size, you can limit referencing 
		 * size of the characters </p>
		 *	\li Referencing a part of characters
		 *
		 * @warning
		 * WeakString only references. Be careful about destruction of the characters (data)
		 *
		 * @param data Target characters to be referenced by string
		 * @param size 
		 *	<p> Specified limit-size of characters to be referenced. </p>
		 *	<p> If the specified size is greater than original size, it will be ignored </p>
		 */
		WeakString(const char *data, size_t size);

		/**
		 * @brief
		 * Constructor by characters of begin and end
		 *
		 * @details
		 * <p> Constructs by characters to be referenced with its end position. </p>
		 * 
		 * <p> Although the original end point of data is over the specified end, you can limit 
		 * end point of the characters. </p> 
		 *	\li Referencing a part of characters
		 *
		 * @warning 
		 * WeakString only references. Be careful about destruction of the characters (data).
		 *
		 * @param begin Target characters to be referenced by string
		 * @param end 
		 *	<p> Specified end point of characters to be referenced. </p>
		 *	<p> If the specified end point is greater than original end point, it will be ignored. </p>
		 */
		WeakString(const char *begin, const char *end);

		/**
		 * @brief Constructor by characters
		 * @details References the null-terminated character sequence pointed by ptr
		 *
		 * @warning WeakString only references. Be careful about destruction of the characeters (data)
		 * @param data Target characters to be referenced by string
		 */
		WeakString(const char *data);

		/**
		 * @brief Constructor by a single character
		 * @details References a single character
		 *
		 * @warning WeakString only references. Be careful about destruction of the characeter (data)
		 * @param ch Target character to be referenced by string
		 */
		WeakString(const char &ch);

		/**
		* @brief Constructor by a initializer list
		* @details References initializer list of character
		*
		* @warning WeakString only references. Be careful about destruction of the characeters (data)
		* @param il Target initializer list of characters to be referenced by string
		*/
		WeakString(std::initializer_list<char> &il);

		/**
		 * @brief Constructor by string
		 * @details References whole chracters of the string
		 *
		 * @warning WeakString only references. Be careful about destruction of the string
		 * @param str Target string to be referenced by string
		 */
		WeakString(const std::string &str);

		/* --------------------------------------------------------------------
			ELEMENT ACCESSORS
		-------------------------------------------------------------------- */
		/**
		 * @brief Get string data; referenced characeters
		 *
		 * @details
		 * Returns a pointer to an array that contains a null-terminated sequence of
		 * characters representing the current value of the string object
		 *
		 * @warning Returned pointer's size can be longer than string's specified size
		 *			if the string references only a part of the characters
		 * @return A pointer of characters being referenced by the string
		 */
		auto data() const -> const char*;

		/**
		 * @brief Returns size of the characters which are being referenced
		 * @details Returns the length of the string, in terms of number of referenced characters
		 *
		 * @return size of characters being referenced by string
		 */
		auto size() const -> size_t;

		/**
		 * @brief Tests wheter string is emtpy
		 *
		 * @detail
		 * <p> Returns wheter characters' size is zero or not. </p>
		 * <p> Of course, string references nothing, then returns false, too. </p>
		 *
		 * @return Wheter size is zero or not
		 */
		auto empty() const -> bool;

		/**
		 * @brief Get character of string
		 * @detail Returns a const reference to the character at the specified position
		 *
		 * @return const reference of character at the specified index
		 */
		auto at(size_t index) const -> const char&;

		/**
		 * @copydoc WeakString::at()
		 */
		auto operator[](size_t index) const -> const char&;

		/* --------------------------------------------------------------------
			FINDERS
		-------------------------------------------------------------------- */
		/**
		 * @brief Finds first occurence in string
		 *
		 * @details
		 * <p> Finds the string after startIndex and returns the position of first occurence of delim. </p>
		 * 
		 * <p> If delim is not found, returns -1 (npos) </p>
		 *
		 * @param delim The substring of the string which to find
		 * @param startIndex Specified starting index of find. Default is 0
		 * @return Index of first occurence of the specified substring or -1
		 */
		auto find(const WeakString &delim, size_t startIndex = NULL) const -> size_t;

		/**
		 * @brief Finds last occurence in string
		 *
		 * @details
		 * Finds the string before endIndex and returns the position of last occurence of delim. 
		 * If delim is not found, returns -1 (npos)
		 *
		 * @param delim The substring of the string which to find
		 * @param endIndex Specified last index of find. Default is size() - 1
		 * @return Index of first occurence of the specified substring or -1
		 */
		auto rfind(const WeakString &delim, size_t endIndex = SIZE_MAX) const -> size_t;

		/**
		 * @brief Finds first occurence in string
		 *
		 * @details
		 * Finds first occurence position of each delim in the string after startIndex
		 * and returns the minimum position of them.
		 *
		 * @note
		 * \li If startIndex is not specified, then starts from 0.
		 * \li If failed to find any substring, returns -1 (npos)
		 *
		 * @param delims The substrings of target(str) which to find
		 * @param startIndex Specified starting index of find. Default is 0
		 * @return pair\<size_t := position, string := matched substring\>
		 */
		auto finds(const std::vector<std::string> &delims, size_t startIndex = 0) const -> IndexPair<WeakString>;
		auto finds(const std::vector<WeakString> &delims, size_t startIndex = 0) const -> IndexPair<WeakString>;

		/**
		 * @brief Finds last occurence in string
		 *
		 * @details
		 * Finds last occurence position of each delim in the string before endIndex
		 * and returns the maximum position of them.
		 * 
		 * @note
		 * \li If index is not specified, then starts str.size() - 1
		 * \li If failed to find any substring, returns -1 (npos)
		 *
		 * @param delims The substrings of target(str) which to find
		 * @param endIndex Specified starting index of find. Default is size() - 1
		 * @return pair\<size_t := position, string := matched substring\>
		 */
		auto rfinds(const std::vector<std::string> &delims, size_t endIndex = SIZE_MAX) const -> IndexPair<WeakString>;
		auto rfinds(const std::vector<WeakString> &delims, size_t endIndex = SIZE_MAX) const -> IndexPair<WeakString>;

		/* --------------------------------------------------------------------
			EXTRACTORS
		-------------------------------------------------------------------- */
		/**
		 * @brief Generates a substring
		 *
		 * @details
		 * Extracts a substring consisting of the characters starts from
		 * startIndex and with a size specified size.
		 *
		 * @param startIndex 
		 *	<p> Index of the first character. </p>
		 *	<p> If startIndex is greater than endIndex, those will be swapped. </p>
		 * @param endIndex 
		 *	<p> Number of characters to include in substring. </p>
		 *	<p> If the specified size is greater than last index of characeters, it will be shrinked. </p>
		 * @return Sub string by specified index and size
		 */
		auto substr(size_t startIndex, size_t endIndex = SIZE_MAX) const -> WeakString;

		/**
		 * @brief Generates a substring
		 *
		 * @details
		 * <p> Extracts a substring consisting of the character specified by
		 * startIndex and all characters up to endIndex - 1. </p>
		 *
		 *	\li If endIndex is not specified, string::size() will be used instead.
		 *	\li If endIndex is greater than startIndex, then those will be swapped
		 *
		 * @param startIndex 
		 *	<p> Index of the first character. </p>
		 *	<p> If startIndex is greater than endIndex, those will be swapped. </p>
		 * @param size 
		 *	<p> Index of the last character - 1. </p>
		 *	<p> If not specified, then string::size() will be used instead. </p>
		 * @return Sub string by specified index(es)
		 */
		auto substring(size_t startIndex, size_t size = SIZE_MAX) const -> WeakString;

		/**
		 * @brief Generates a substring
		 *
		 * @details
		 * <p> Extracts a substring consisting of the characters from specified start to end. </p>
		 *
		 * <p> It's same with substring( ? = (str.find(start) + start.size()), find(end, ?) ) </p>
		 *	\li between(\"ABCD[EFGH]IJK\", \"[\", \"]\") => \"EFGH\"
		 * 
		 * @note
		 * \li If start is not specified, extracts from begin of the string to end
		 * \li If end is not specified, extracts from start to end of the string
		 * \li If start and end are all omitted, returns str, itself.
		 *
		 * @param start A string for separating substring at the front
		 * @param end A string for separating substring at the end
		 * @return substring by specified terms
		 */
		auto between(const WeakString &start = {}, const WeakString &end = {}) const -> WeakString;

		/**
		 * @brief Generates substrings
		 * @details Splits a string in to an array of substrings dividing by the specified delimiter
		 *
		 * @param delim The pattern which specifies where to split the string
		 * @return An array of substrings
	 	 */
		auto split(const WeakString &delim) const -> std::vector<WeakString>;

		/**
		 * @brief Generates substrings
		 *
		 * @details
		 * <p> Splits a string into an array of substrings dividing by delimeters of start and end. </p>
		 * <p> It's the array of substrings adjusted the between. </p>
		 * 
		 * @note
		 * \li If start is omitted, it's same with the split by endStr not having last item
		 * \li If end is omitted, it's same with the split by startStr not having first item
		 * \li If start and end are all omitted, returns string, itself
		 *
		 * @param start 
		 *	<p> A string for separating substring at the front. </p>
		 *	<p> If omitted, it's same with split(end) not having last item. </p>
		 * @param end 
		 *	<p> A string for separating substring at the end. </p>
		 *	<p> If omitted, it's same with split(start) not having first item. </p>
		 * @return An array of substrings
		 */
		auto betweens(const WeakString &start = {}, const WeakString &end = {}) const -> std::vector<WeakString>;

		/* --------------------------------------------------------------------
			TRIMS
		-------------------------------------------------------------------- */
		/**
		 * @brief Removes all designated characters from the beginning and end of the specified string
		 *
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the beginning and end
		 */
		auto trim(const std::vector<std::string> &delims) const -> WeakString;
		
		/**
		 * @brief Removes all designated characters from the beginning of the specified string
		 *
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the beginning
		 */
		auto ltrim(const std::vector<std::string> &delims) const -> WeakString;
		
		/**
		 * @brief Removes all designated characters from the end of the specified string
		 *
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the end
		 */
		auto rtrim(const std::vector<std::string> &delims) const -> WeakString;

		auto trim() const -> WeakString;
		auto ltrim() const -> WeakString;
		auto rtrim() const -> WeakString;

		auto trim(const WeakString &delim) const -> WeakString;
		auto ltrim(const WeakString &delim) const -> WeakString;
		auto rtrim(const WeakString &delim) const -> WeakString;

		auto trim(const std::vector<WeakString> &delims) const -> WeakString;
		auto ltrim(const std::vector<WeakString> &delims) const -> WeakString;
		auto rtrim(const std::vector<WeakString> &delims) const -> WeakString;

		/* --------------------------------------------------------------------
			REPLACERS
		-------------------------------------------------------------------- */
		/**
		 * @brief Replace portion of string once
		 *
		 * @param before A specific word you want to be replaced
		 * @param after A specific word you want to replace
		 * @return A string specific word is replaced once
		 */
		auto replace(const WeakString &before, const WeakString &after) const -> std::string;

		/**
		 * @brief Returns a string specified word is replaced
		 *
		 * @param before A specific word you want to be replaced
		 * @param after A specific word you want to replace
		 * @return A string specified word is replaced
		 */
		auto replaceAll(const WeakString &before, const WeakString &after) const -> std::string;

		/**
		 * @brief Returns a string specified words are replaced
		 *
		 * @param str Target string to replace
		 * @param pairs A specific word's pairs you want to replace and to be replaced
		 * @return A string specified words are replaced
		 */
		auto replaceAll(const std::vector<std::pair<std::string, std::string>> &pairs) const -> std::string;
		auto replaceAll(const std::vector<std::pair<WeakString, WeakString>> &pairs) const -> std::string;

		/**
		 * @brief Convert uppercase letters to lowercase
		 * @details Returns a string that all uppercase characters are converted to lowercase.
		 *
		 * @param wstr Target string to convert uppercase to lowercase
		 * @return A string converted to lowercase
		 */
		auto toLowerCase() const -> std::string;

		/**
		 * @brief Convert uppercase letters to lowercase
		 * @details Returns a string all lowercase characters are converted to uppercase.
		 *
		 * @param str Target string to convert lowercase to uppercase
		 * @return A string converted to uppercase
		 */
		auto toUpperCase() const -> std::string;

		/* --------------------------------------------------------------------
			COMPARISONS
		-------------------------------------------------------------------- */
		auto operator==(const WeakString &str) const -> bool;
		auto operator<(const WeakString &str) const -> bool;
		OPERATOR_METHODS_HEADER(WeakString)

		/* --------------------------------------------------------------------
			CONVERSIONS
		-------------------------------------------------------------------- */
		/**
		 * @brief Get the string content
		 * @details Returns a string object with a copy of the current contents in the WeakString.
		 *
		 * @return A new string copied from the WeakString
		 */
		auto str() const -> std::string;
		operator std::string();
	};
};
