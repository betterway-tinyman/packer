#pragma once
#include <samchon/HashMap.hpp>

#include <initializer_list>
#include <samchon/WeakString.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief URLVariables class is for representing variables of HTTP
	 *
	 * @details
	 * <p> URLVariables class allows you to transfer variables between an application and server.
	 * When transfering, URLVariables will be converted to a URI string. </p>
	 *	\li URI: Uniform Resource Identifier
	 *
	 * <p> Use URLVariabels objects with methods of HTTPLoader class. </p>
	 *
	 * @image html cp7p/subset/library_http.png
	 * @image latex cpp/subset/library_http.png
	 *
	 * <h4> Example code </h4>
	 * @includelineno example/url_variables/main.cpp
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class URLVariables
		: public HashMap<std::string, std::string>
	{
	private:
		typedef HashMap<std::string, std::string> super;

	public:
		/* ------------------------------------------------------------
			CONSTRUCTORS
		 ------------------------------------------------------------ */
		 /**
		  * @brief Default Constructor
		  */
		URLVariables() : super()
		{
		};

		/**
		 * @brief Constructor by a string representing encoded properties
		 * @details Converts the variable string to properties of the specified URLVariables object.
		 *
		 *	\li URLVariables(\"id=jhnam88&name=Jeongho+Nam") => {{\"id\", \"jhnam88\"}, {\"name\", \"Jeongho Nam <http://samchon.org>\"}}
		 *
		 * @param A uri-encoded string containing pair of properties
		 */
		URLVariables(const WeakString &flashVars) : super()
		{
			std::vector<WeakString> &items = flashVars.split("&");
			for (size_t i = 0; i < items.size(); i++)
			{
				WeakString &item = items[i];
				size_t index = item.find("=");

				if (index == std::string::npos)
					continue;

				std::string &key = item.substr(0, index).str();
				std::string &value = decode(item.substr(index + 1));

				set(key, value);
			}
		};

	public:
		/**
		 * @brief Encode a string into a valid URI
		 * @details Encodes a string to follow URI standard format.
		 *
		 * @param A string to encode to URI
		 * @return A string converted to URI
		 */
		static auto encode(const WeakString &wstr) -> std::string
		{
			std::string res;
			res.reserve(wstr.size() * 3);

			for (size_t i = 0; i < wstr.size(); i++)
			{
				unsigned char ch = wstr[i];

				if
					(
						('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z')
						|| ('0' <= ch && ch <= '9')
						|| ch == '-' || ch == '_' || ch == '.' || ch == '~'
						|| ch == '@' || ch == ':' || ch == '/' || ch == '\\'
					)
					res.push_back(ch);
				else if (ch == ' ')
					res.append("%20");
				else
					res.append
					({
						'%',
						toHex(ch >> 4),
						toHex(ch & 0x0F)
					});
			}
			return res;
		};

		/**
		 * @brief Decode a URI string
		 * @details Decodes a URI string to its original
		 *
		 * @param A string encoded
		 * @return A string decoded from URI
		 */
		static auto decode(const WeakString &wstr) -> std::string
		{
			std::string res;
			res.reserve(wstr.size());

			for (size_t i = 0; i < wstr.size(); i++)
			{
				const char ch = wstr[i];

				if (ch == '%' && wstr.size() > i + 2)
				{
					char ch1 = fromHex(wstr[i + 1]);
					char ch2 = fromHex(wstr[i + 2]);
					char decoded = (ch1 << 4) | ch2;

					res.append({ decoded });
					i += 2;
				}
				else
					res.append({ ch });
			}
			return res;
		};

	private:
		/* ------------------------------------------------------------
			URI ENCODING & DECONDING
		------------------------------------------------------------ */
		static auto toHex(unsigned char ch) -> char
		{
			static const std::string lookup = "0123456789ABCDEF";

			return lookup[ch];
		};

		static auto fromHex(unsigned char ch) -> char
		{
			if (ch <= '9' && ch >= '0')
				ch -= '0';
			else if (ch <= 'f' && ch >= 'a')
				ch -= 'a' - 10;
			else if (ch <= 'F' && ch >= 'A')
				ch -= 'A' - 10;
			else
				ch = 0;

			return ch;
		};

	public:
		/* ------------------------------------------------------------
			TO_STRING
		------------------------------------------------------------ */
		/**
		 * @brief Get the string representing URLVariables
		 * @details Returns a string object representing URLVariables following the URI\n
		 *
		 *	\li URLVariables({{"id", "jhnam88"}, {"name", "Jeongho Nam <http://samchon.org>"}}).toString() => "id=jhnam88&name=Jeongho+Nam"
		 *
		 * @return A string representing URLVariables following the URI
		 */
		auto toString() const -> std::string
		{
			std::string str = "";
			for (const_iterator it = begin(); it != end(); it++)
			{
				if (it != begin())
					str.append("&");

				str.append(it->first);
				str.append("=");
				str.append(encode(it->second));
			}
			return str;
		};
	};
};
};