#pragma once
#include <samchon/API.hpp>

#include <string>
#include <samchon/WeakString.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief A utility class supporting conversion between multiple character-sets
	 *
	 * @details 
	 * <p> Charset class, it's easier to think iconv in linux </p> 
	 * <p> Supported character-sets </p>
	 *
	 *	\li Multibyte (ANSI)
	 *	\li UTF-8
	 *	\li Unicode (UTF-16)
	 *
	 * @image html cpp/subset/library_string.png
	 * @image latex cpp/subset/library_string.png
	 *
	 * @warning In some system, std::wstring is not for utf-8 but for unicode (utf-16).
	 *
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API Charset
	{
	public:
		enum : int
		{
			MULTIBYTE = 4,
			UTF8 = 8
		};

		/**
		 * @brief Convert utf-8 to multibyte
		 *
		 * @details
		 * <p> Converts utf-8 string to multibyte string. </p>
		 * <p> If the utf-8 string has header int the front, it will be erased. </p>
		 *
		 * @warning If utf-8 header is not at the front of string,
		 *			it can't be erased and may cause breakage on letters
		 * @param str A utf-8 string would be converted to multibyte
		 * @return A multibyte string
		 */
		static auto toMultibyte(const std::string &)->std::string;

		/**
		 * @brief Convert unicode to multibyte
		 *
		 * @details
		 * Converts unicode string to multibyte string\n
		 * If the unicode string has header in the front, it will be erased
		 *
		 * @warning If unicode header is not at the front of string,
		 *			it can't be erased and may cause breakage on letters
		 * @param str A unicode string to be converted
		 * @return A multibyte string
		 */
		static auto toMultibyte(const std::wstring &)->std::string;

		/**
		 * @brief Convert multibyte to utf-8
		 *
		 * @details
		 * Converts multibyte string to utf-8 string\n
		 * If the inputted string is not multibyte, then may cause breakage on letters
		 *
		 * @warning Converted utf-8 string does not contain the header
		 * @param str A multibyte string would be converted
		 * @return A utf-8 string
		 */
		static auto toUTF8(const std::string &)->std::string;

		/**
		 * @brief Convert unicode to utf-8
		 *
		 * @details
		 * Converts unicode string to utf-8 string\n
		 * Generated utf-8 string follows unicode string wheter to have header or not
		 *	\li If the unicode string has header on first, utf-8 will also have the header, too
		 *  \li If not, generated utf-8 string doesn't have the header, either
		 *
		 * @param str A unicode string would be converted
		 * @return A utf-8 string
		 */
		static auto toUTF8(const std::wstring &)->std::string;

		/**
		 * @brief Convert multibyte or utf-8 to unicode
		 *
		 * @details
		 *
		 *
		 * @param str A multibyte or utf-8 string would be converted
		 * @param Designate str is multibyte or utf-8 string
		 * @return A unicode string
		 */
		static auto toUnicode(const std::string &, int)->std::wstring;
	};
};
};