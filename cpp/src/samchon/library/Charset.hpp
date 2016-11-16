#pragma once
#include <samchon/API.hpp>

#include <string>
#include <codecvt>
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
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Charset
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
		static auto toMultibyte(const std::string &source) -> std::string
		{
			std::wstring &wstr = toUnicode(source, UTF8);
			std::string &dest = toMultibyte(wstr);

			return dest;
		};

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
		static auto toMultibyte(const std::wstring &source) -> std::string
		{
			using namespace std;
			typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;

			locale &loc = locale("");

			codecvt_t const& codecvt = use_facet<codecvt_t>(loc);
			mbstate_t state = mbstate_t();
			vector<char> buf(source.size() * codecvt.max_length());
			wchar_t const* in_next = source.c_str();
			char* out_next = &buf[0];

			codecvt_base::result r =
				codecvt.out
				(
					state,
					source.c_str(), source.c_str() + source.size(), in_next,
					&buf[0], &buf[0] + buf.size(), out_next
				);

			if (r == codecvt_base::error)
				throw runtime_error("can't convert wstring to string");

			return string(buf.begin(), buf.end());
		};

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
		static auto toUTF8(const std::string &source) -> std::string
		{
			std::wstring &wstr = toUnicode(source, MULTIBYTE);
			std::string &dest = toUTF8(wstr);

			if (dest.back() == NULL)
				dest.pop_back();

			return dest;
		};

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
		static auto toUTF8(const std::wstring &source) -> std::string
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf8Converter;

			return utf8Converter.to_bytes(source);
		};

		/**
		 * @brief Convert multibyte or utf-8 to unicode
		 *
		 * @param str A multibyte or utf-8 string would be converted
		 * @param Designate str is multibyte or utf-8 string
		 * @return A unicode string
		 */
		static auto toUnicode(const std::string &source, int charset) -> std::wstring
		{
			using namespace std;

			if (charset == MULTIBYTE)
			{
				locale &loc = locale("");

				typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
				codecvt_t const& codecvt = use_facet<codecvt_t>(loc);
				mbstate_t state = mbstate_t();
				vector<wchar_t> buf(source.size());
				char const* in_next = source.c_str();
				wchar_t* out_next = &buf[0];

				codecvt_base::result r =
					codecvt.in
					(
						state,
						source.c_str(), source.c_str() + source.size(), in_next,
						&buf[0], &buf[0] + buf.size(), out_next
					);

				if (r == codecvt_base::error)
					throw runtime_error("can't convert string to wstring");

				return wstring(buf.begin(), buf.end());
			}
			else if (charset == UTF8)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf8Converter;
				wstring &wstr = move(utf8Converter.from_bytes(source));

				return wstr;
			}
			else
				return L"";
		};
	};
};
};