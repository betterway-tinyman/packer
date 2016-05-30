#pragma once

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <samchon/IndexPair.hpp>

#include <samchon/library/IOperator.hpp>
#include <samchon/library/Math.hpp>

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
	class WeakString
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
		WeakString()
		{
			this->data_ = nullptr;
			this->size_ = 0;
		};

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
		WeakString(const char *data, size_t size)
		{
			this->data_ = data;
			this->size_ = size;
		};

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
		WeakString(const char *begin, const char *end)
			: WeakString(begin, end - begin)
		{
		};

		/**
		 * @brief Constructor by characters
		 * @details References the null-terminated character sequence pointed by ptr
		 *
		 * @warning WeakString only references. Be careful about destruction of the characeters (data)
		 * @param data Target characters to be referenced by string
		 */
		WeakString(const char *data)
		{
			this->data_ = data;

			if (data == nullptr)
				this->size_ = 0;
			else
				this->size_ = std::char_traits<char>::length(data);
		};

		/**
		 * @brief Constructor by a single character
		 * @details References a single character
		 *
		 * @warning WeakString only references. Be careful about destruction of the characeter (data)
		 * @param ch Target character to be referenced by string
		 */
		WeakString(const char &ch)
		{
			this->data_ = &ch;
			this->size_ = 1;
		};

		/**
		* @brief Constructor by a initializer list
		* @details References initializer list of character
		*
		* @warning WeakString only references. Be careful about destruction of the characeters (data)
		* @param il Target initializer list of characters to be referenced by string
		*/
		WeakString(std::initializer_list<char> &il)
		{
			if (il.size() == 0)
				this->data_ = nullptr;
			else
				this->data_ = il.begin();

			this->size_ = il.size();
		};

		/**
		 * @brief Constructor by string
		 * @details References whole chracters of the string
		 *
		 * @warning WeakString only references. Be careful about destruction of the string
		 * @param str Target string to be referenced by string
		 */
		WeakString(const std::string &str)
		{
			this->data_ = str.data();
			this->size_ = str.size();
		};

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
		auto data() const -> const char*
		{
			return data_;
		};

		/**
		 * @brief Returns size of the characters which are being referenced
		 * @details Returns the length of the string, in terms of number of referenced characters
		 *
		 * @return size of characters being referenced by string
		 */
		auto size() const -> size_t
		{
			return size_;
		};

		/**
		 * @brief Tests wheter string is emtpy
		 *
		 * @detail
		 * <p> Returns wheter characters' size is zero or not. </p>
		 * <p> Of course, string references nothing, then returns false, too. </p>
		 *
		 * @return Wheter size is zero or not
		 */
		auto empty() const -> bool
		{
			return data_ == nullptr || size_ == 0;
		};

		/**
		 * @brief Get character of string
		 * @detail Returns a const reference to the character at the specified position
		 *
		 * @return const reference of character at the specified index
		 */
		auto at(size_t index) const -> const char&
		{
			if (index > size_)
				throw std::out_of_range("out of range.");

			return *(data_ + index);
		};

		/**
		 * @copydoc WeakString::at()
		 */
		auto operator[](size_t index) const -> const char&
		{
			return *(data_ + index);
		};

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
		auto find(const WeakString &delim, size_t startIndex = NULL) const -> size_t
		{
			size_t j = 0;

			for (size_t i = startIndex; i < size_; i++)
				if (data_[i] != delim[j++])
					j = 0;
				else if (j == delim.size())
					return i - delim.size() + 1;

			return npos;
		};

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
		auto rfind(const WeakString &delim, size_t endIndex = SIZE_MAX) const -> size_t
		{
			if (empty() == true || endIndex == 0)
				return npos;

			size_t j = delim.size() - 1;

			for (long long i = std::min<size_t>(endIndex - 1, size_ - 1); i >= 0; i--)
				if (data_[(size_t)i] != delim[j]) //NOT MATCHED
					j = delim.size() - 1;
				else if (j == 0) //FULLY MATCHED
					return (size_t)i;
				else //PARTIALLY MATCHED,
					j--;

			return npos;
		};

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
		auto finds(const std::vector<std::string> &delims, size_t startIndex = 0) const -> IndexPair<WeakString>
		{
			std::vector<WeakString> wdelims(delims.size());
			for (size_t i = 0; i < delims.size(); i++)
				wdelims[i] = delims[i];

			return finds(wdelims, startIndex);
		};

		auto finds(const std::vector<WeakString> &delims, size_t startIndex = 0) const -> IndexPair<WeakString>
		{
			std::vector<size_t> positionVector;
			positionVector.reserve(delims.size());

			for (size_t i = 0; i < delims.size(); i++)
				positionVector.push_back(find(delims[i], startIndex));

			IndexPair<size_t> &iPair = library::Math::minimum(positionVector);
			return { iPair.get_index(), delims[iPair.getValue()] };
		};

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
		auto rfinds(const std::vector<std::string> &delims, size_t endIndex = SIZE_MAX) const -> IndexPair<WeakString>
		{
			std::vector<WeakString> wdelims(delims.size());
			for (size_t i = 0; i < delims.size(); i++)
				wdelims[i] = delims[i];

			return rfinds(wdelims, endIndex);
		};
		
		auto rfinds(const std::vector<WeakString> &delims, size_t endIndex = SIZE_MAX) const -> IndexPair<WeakString>
		{
			std::vector<size_t> positionVector;
			positionVector.reserve(delims.size());

			size_t position;

			for (size_t i = 0; i < delims.size(); i++)
			{
				position = rfind(delims[i], endIndex);

				if (position != std::string::npos)
					positionVector.push_back(position);
			}

			if (positionVector.empty() == true)
				return { std::string::npos, WeakString() };

			IndexPair<size_t> &iPair = library::Math::maximum(positionVector);
			return { iPair.get_index(), delims[iPair.getValue()] };
		};

		/* --------------------------------------------------------------------
			EXTRACTORS
		-------------------------------------------------------------------- */
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
		 * 
		 * @return Sub string by specified index(es)
		 */
		auto substr(size_t startIndex, size_t size = SIZE_MAX) const -> WeakString
		{
			if (size > size_ || startIndex + size > size_)
				size = size_ - startIndex;

			return WeakString(data_ + startIndex, size);
		};

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
		 *
		 * @return Sub string by specified index and size
		 */
		auto substring(size_t startIndex, size_t endIndex = SIZE_MAX) const -> WeakString
		{
			if (startIndex > endIndex)
				std::swap(startIndex, endIndex);

			if (startIndex == endIndex || startIndex > size_ - 1)
				return WeakString();

			if (endIndex > size_)
				endIndex = size_;

			return WeakString(data_ + startIndex, data_ + endIndex);
		};

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
		auto between(const WeakString &start = {}, const WeakString &end = {}) const -> WeakString
		{
			if (start.empty() == true && end.empty() == true)
				return *this;
			else if (start.empty() == true)
				return substr(0, find(end));
			else if (end.empty() == true)
				return substr(find(start) + start.size());
			else
			{
				size_t startIndex = find(start);

				return substring
				(
					startIndex + start.size(),
					find(end, startIndex + start.size())
				);
			}
		};

		/**
		 * @brief Generates substrings
		 * @details Splits a string in to an array of substrings dividing by the specified delimiter
		 *
		 * @param delim The pattern which specifies where to split the string
		 * @return An array of substrings
	 	 */
		auto split(const WeakString &delim) const -> std::vector<WeakString>
		{
			size_t startIndex = 0;
			size_t x;

			//CONSTRUCT THE LIST OF QUOTES
			std::queue<std::pair<size_t, size_t>> quoteList;
			while ((x = find(delim, startIndex)) != npos)
			{
				quoteList.push({ startIndex, x });
				startIndex = x + delim.size();
			}
			quoteList.push({ startIndex, size() });

			//ASSIGN THE STRING_VECTOR BY SUBSTRING
			std::vector<WeakString> vec;
			vec.reserve(quoteList.size());

			while (quoteList.empty() == false)
			{
				vec.push_back(substring(quoteList.front().first, quoteList.front().second));
				quoteList.pop();
			}
			return vec;
		};

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
		auto betweens(const WeakString &start = {}, const WeakString &end = {}) const -> std::vector<WeakString>
		{
			std::vector<WeakString> vec;

			if (start.empty() == true && end.empty() == true)
				return vec;
			else if (start == end) //NOT EMPTY, BUT EQUALS
			{
				std::queue<std::pair<size_t, size_t>> quoteList;

				size_t x, prevX = -1, n = 0;
				while ((x = find(start, prevX + 1)) != npos)
				{
					if (++n % 2 == 0) //WHEN THE MATCHED NUMBER IS EVEN
						quoteList.push({ prevX, x });
					prevX = x;
				}

				if (quoteList.size() == 0)
					vec.push_back(*this);
				else
				{
					vec.reserve(quoteList.size());
					while (quoteList.empty() == false)
					{
						std::pair<size_t, size_t> &quote = quoteList.front();
						vec.push_back(substring(quote.first + start.size()));

						quoteList.pop();
					}
				}
			}
			else //BEGIN AND END IS DIFFER
			{
				vec = split(start);
				vec.erase(vec.begin());

				if (end.empty() == false)
					for (long long i = (long long)vec.size() - 1; i >= 0; i--)
						if (vec.at((size_t)i).find(end) == npos)
							vec.erase(vec.begin() + (size_t)i);
						else
							vec[(size_t)i] = vec[(size_t)i].between("", end);
			}
			return vec;
		};

		/* --------------------------------------------------------------------
			TRIMS
		-------------------------------------------------------------------- */
		auto trim() const -> WeakString
		{
			return ltrim().rtrim();
		};
		auto ltrim() const -> WeakString
		{
			static const std::vector<std::string> SPACE_ARRAY = { " ", "\t", "\r", "\n" };

			return ltrim(SPACE_ARRAY);
		};
		auto rtrim() const -> WeakString
		{
			static const std::vector<std::string> SPACE_ARRAY = { " ", "\t", "\r", "\n" };

			return rtrim(SPACE_ARRAY);
		};

		auto trim(const WeakString &delim) const -> WeakString
		{
			return ltrim(delim).rtrim(delim);
		};
		auto ltrim(const WeakString &delim) const -> WeakString
		{
			return ltrim(std::vector<WeakString>({delim}));
		};
		auto rtrim(const WeakString &delim) const -> WeakString
		{
			return rtrim(std::vector<WeakString>({ delim }));
		};

		/**
		 * @brief Removes all designated characters from the beginning and end of the specified string
		 *
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the beginning and end
		 */
		auto trim(const std::vector<std::string> &delims) const -> WeakString
		{
			return ltrim(delims).rtrim(delims);
		};
		
		/**
		 * @brief Removes all designated characters from the beginning of the specified string
		 *
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the beginning
		 */
		auto ltrim(const std::vector<std::string> &delims) const -> WeakString
		{
			std::vector<WeakString> wdelims(delims.size());
			for (size_t i = 0; i < delims.size(); i++)
				wdelims[i] = delims[i];

			return ltrim(wdelims);
		};
		
		/**
		 * @brief Removes all designated characters from the end of the specified string
		 *
		 * @param delims Designated character(s)
		 * @return Updated string where designated characters was removed from the end
		 */
		auto rtrim(const std::vector<std::string> &delims) const -> WeakString
		{
			std::vector<WeakString> wdelims(delims.size());
			for (size_t i = 0; i < delims.size(); i++)
				wdelims[i] = delims[i];

			return rtrim(wdelims);
		};

		auto trim(const std::vector<WeakString> &delims) const -> WeakString
		{
			return ltrim(delims).rtrim(delims);
		};
		auto ltrim(const std::vector<WeakString> &delims) const -> WeakString
		{
			WeakString str(data_, size_);
			IndexPair<size_t> indexPair = { 0, 0 };

			while (str.empty() == false)
			{
				std::vector<size_t> indexVec;
				indexVec.reserve(delims.size());

				for (size_t i = 0; i < delims.size(); i++)
					indexVec.push_back(str.find(delims[i]));

				indexPair = library::Math::minimum(indexVec);
				if (indexPair.getValue() == 0)
				{
					size_t size = delims[indexPair.get_index()].size();

					str.data_ += size;
					str.size_ -= size;
				}
				else
					break;
			}

			return str;
		};
		auto rtrim(const std::vector<WeakString> &delims) const -> WeakString
		{
			WeakString str(data_, size_);
			IndexPair<size_t> pairIndex;

			while (str.empty() == false)
			{
				std::vector<size_t> indexVec;
				indexVec.reserve(delims.size());

				for (size_t i = 0; i < delims.size(); i++)
				{
					size_t index = str.rfind(delims[i]);
					if (index != npos)
						indexVec.push_back(index);
				}
				if (indexVec.empty() == true)
					break;

				pairIndex = library::Math::maximum(indexVec);
				size_t size = delims[pairIndex.get_index()].size();

				if (pairIndex.getValue() == str.size() - size)
					str.size_ -= size;
				else
					break;
			}
			return str;
		};

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
		auto replace(const WeakString &before, const WeakString &after) const -> std::string
		{
			size_t index = find(before);
			if (index == npos)
				return str();

			std::string str;
			str.reserve(size() - before.size() + after.size());

			str.append(substr(0, index).str());
			str.append(after.str());
			str.append(substr(index + before.size()).str());

			return str;
		};

		/**
		 * @brief Returns a string specified word is replaced
		 *
		 * @param before A specific word you want to be replaced
		 * @param after A specific word you want to replace
		 * @return A string specified word is replaced
		 */
		auto replaceAll(const WeakString &before, const WeakString &after) const -> std::string
		{
			return replaceAll({ { before, after } });
		};

		/**
		 * @brief Returns a string specified words are replaced
		 *
		 * @param str Target string to replace
		 * @param pairs A specific word's pairs you want to replace and to be replaced
		 * @return A string specified words are replaced
		 */
		auto replaceAll(const std::vector<std::pair<std::string, std::string>> &pairs) const -> std::string
		{
			std::vector<std::pair<WeakString, WeakString>> wPairs(pairs.size());
			for (size_t i = 0; i < pairs.size(); i++)
				wPairs[i] = { pairs[i].first, pairs[i].second };

			return replaceAll(wPairs);
		};

		auto replaceAll(const std::vector<std::pair<WeakString, WeakString>> &pairs) const -> std::string
		{
			if (pairs.empty() == true)
				return this->str();

			std::list<std::pair<size_t, size_t>> foundPairList;
			//1ST IS STR-INDEX FROM FIND
			//2ND IS PAIR-INDEX

			size_t size = this->size();
			size_t index;
			size_t i;

			//FIND POSITION-INDEX IN ORIGINAL STRING
			for (i = 0; i < pairs.size(); i++)
			{
				index = 0;

				while (true)
				{
					index = find(pairs[i].first, index);
					if (index == npos)
						break;

					size -= pairs[i].first.size();
					size += pairs[i].second.size();

					foundPairList.push_back({ index++, i });
				}
			}

			if (foundPairList.empty() == true)
				return str();

			foundPairList.sort();

			//REPLACE
			std::string str;
			str.reserve((size_t)size);

			index = 0;

			while (foundPairList.empty() == false)
			{
				auto it = foundPairList.begin();
				auto &before = pairs[it->second].first;
				auto &after = pairs[it->second].second;

				str.append(substring(index, it->first).str());
				str.append(after.str());

				index = it->first + before.size();
				foundPairList.pop_front();
			}
			if (index <= this->size() - 1)
				str.append(substr(index).str());

			return str;
		};

		/**
		 * @brief Convert uppercase letters to lowercase
		 * @details Returns a string that all uppercase characters are converted to lowercase.
		 *
		 * @param wstr Target string to convert uppercase to lowercase
		 * @return A string converted to lowercase
		 */
		auto toLowerCase() const -> std::string
		{
			std::string &str = this->str();

			for (size_t i = 0; i < str.size(); i++)
				if ('A' <= str[i] && str[i] <= 'Z')
					str[i] = tolower(str[i]);

			return str;
		};

		/**
		 * @brief Convert uppercase letters to lowercase
		 * @details Returns a string all lowercase characters are converted to uppercase.
		 *
		 * @param str Target string to convert lowercase to uppercase
		 * @return A string converted to uppercase
		 */
		auto yoUpperCase() const -> std::string
		{
			std::string &str = this->str();

			for (size_t i = 0; i < str.size(); i++)
				if ('a' <= str[i] && str[i] <= 'z')
					str[i] = toupper(str[i]);

			return str;
		};

		/* --------------------------------------------------------------------
			COMPARISONS
		-------------------------------------------------------------------- */
		auto operator==(const WeakString &str) const -> bool
		{
			if (size_ != str.size_)
				return false;
			else if (data_ == str.data_)
				return true;

			for (size_t i = 0; i < size(); i++)
				if (data_[i] != str.data_[i])
					return false;

			return true;
		};

		auto operator<(const WeakString &str) const -> bool
		{
			size_t minSize = std::min<size_t>(size(), str.size());

			for (size_t i = 0; i < minSize; i++)
				if (this->at(i) == str[i])
					continue;
				else if (this->at(i) < str[i])
					return true;
				else
					return false;

			if (this->size() == minSize && this->size() != str.size())
				return true;
			else
				return false;
		};

		OPERATOR_METHODS_INLINE(WeakString)

		/* --------------------------------------------------------------------
			CONVERSIONS
		-------------------------------------------------------------------- */
		/**
		 * @brief Get the string content
		 * @details Returns a string object with a copy of the current contents in the WeakString.
		 *
		 * @return A new string copied from the WeakString
		 */
		auto str() const -> std::string
		{
			return std::string(data_, data_ + size_);
		};

		operator std::string()
		{
			return str();
		};
	};
};
