#pragma once
#include <samchon/API.hpp>

#include <string>
#include <sstream>

namespace samchon
{
	class WeakString;
};

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An interface supporting conversion to html.
	 *
	 * @details 
	 * <p> IHTMEntity is an interface supporting conversion method to html tags. The IHTMLEntity is used
	 * to documentation or printing data of the entity to web-screen; In C++, documentation is main reason
	 * to implementing the IHTMLEntity and in JS, printing on web-screen is main reason. </p>
	 *
	 * <p> IHTMLEntity provides abstract method toHTML() and varadic template methods for constituting
	 * a table. I'm planning to support lots of utility methods for html tags but I've not exprienced 
	 * developing the homepage or html service. If you're familiar with the html, please contribute to
	 * the IHTMLEntity interface. </p>
	 *
	 * @image html  cpp/protocol_entity.png
	 * @image latex cpp/protocol_entity.png
	 *
	 * \par Example source
	 * @includelineno example/entity/main.cpp
	 *
	 * <h4> Result of the example </h4>
	 * @image html  cpp/result/example_entity.png
	 * @image latex cpp/result/example_entity.png
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API IHTMLEntity
	{
	public:
		/* ------------------------------------------------------------------------------------
			CONSTRUCTORS
		------------------------------------------------------------------------------------ */
		/**
		 * @brief Default Constructor.
		 */
		IHTMLEntity();
		virtual ~IHTMLEntity() = default;

		/* ------------------------------------------------------------------------------------
			EXPORTERS
		------------------------------------------------------------------------------------ */
		/**
		 * @brief Get a string represents a \<th\> tag.
		 *
		 * @tparam _Ty Type of an argument to be contained with a \<td\> tag.
		 * @tparam _Args Left varadic template arguments' type
		 *
		 * @details
		 * <p> Returns a string represents a title columns with \<th\> tag by varadic template method. </p>
		 *
		 * <p> Supported parameters: that can converted to string </p>
		 * <ul>
		 *	<li> number </li>
		 *	<ul>
		 *		<li> (unsigned) short </li>
		 *		<li> (unsigned) long </li>
		 *		<li> (unsigned) long long </li>
		 *		<li> (unsigned) int </li>
		 *		<li> float </li>
		 *		<li> double </li>
		 *		<li> long double </li>
		 *	</ul>
		 *	<li> string </li>
		 *	<ul>
		 *		<li> std::string </li>
		 *		<li> WeakString </li>
		 *	</ul>
		 * </ul>
		 *
		 * @param val An element to be contained with \<td\> a tag.
		 * @param args Left argument to be contained with \<td\> tags.
		 * @return A string of \<th\> tag represents title columns.
		 */
		template <typename T, typename ... _Args>
		static auto toTH(const T &val, const _Args& ... args) -> std::string
		{
			std::string str;
			size_t i = 0;
			size_t size = sizeof...(args)+1;

			fetchTX("th", str, i, size, val, args...);
			return str;
		};
		template <typename T>
		static auto toTH(const T &val) -> std::string
		{
			std::string str =
				"\t<th>\n" +
				"\t\t" + toTD(val) + "\n" +
				"\t</th>";

			return str;
		};

		/**
		 * @brief Get a string represents a \<tr\> tag.
		 *
		 * @tparam _Ty Type of an argument to be contained with a \<tr\> tag.
		 * @tparam _Args Left varadic template arguments' type
		 *
		 * @details
		 * <p> Returns a string represents a data columns of a row with \<tr\> tag by varadic template method. </p>
		 *
		 * <p> Supported parameters: that can converted to string </p>
		 * <ul>
		 *	<li> number </li>
		 *	<ul>
		 *		<li> (unsigned) short </li>
		 *		<li> (unsigned) long </li>
		 *		<li> (unsigned) long long </li>
		 *		<li> (unsigned) int </li>
		 *		<li> float </li>
		 *		<li> double </li>
		 *		<li> long double </li>
		 *	</ul>
		 *	<li> string </li>
		 *	<ul>
		 *		<li> std::string </li>
		 *		<li> WeakString </li>
		 *	</ul>
		 * </ul>
		 *
		 * @param val An element to be contained with \<tr\> a tag.
		 * @param args Left argument to be contained with \<tr\> tags.
		 * @return A string of \<th\> tag represents data columns of a row.
		 */
		template <typename T, typename ... _Args>
		static auto toTR(const T &val, const _Args& ... args) -> std::string
		{
			std::string str;
			size_t i = 0;
			size_t size = sizeof...(args)+1;

			fetchTX("tr", str, i, size, val, args...);
			return str;
		};
		template <typename T>
		static auto toTR(const T &val) -> std::string
		{
			std::string str =
				"\t<tr>\n" +
				"\t\t" + toTD(val) + "\n" +
				"\t</tr>";

			return str;
		};

		/**
		 * @brief Get an html string represents the Entity.
		 *
		 * @return A string represents the Entity by html tags.
		 */
		virtual auto toHTML() const->std::string = 0;

	protected:
		template <typename T, typename ... _Args>
		static void fetchTX(const std::string &tag, std::string &str, size_t &index, size_t size, const T &val, const _Args& ... args)
		{
			fetchTX(tag, str, index, size, val);

			index++;
			fetchTX(tag, str, index, size, args...);
		};
		template <typename T>
		static void fetchTX(const std::string &tag, std::string &str, size_t &index, size_t size, const T &val)
		{
			if (index == 0)
				str += "\t<" + tag + ">\n";

			str += "\t\t" + toTD(val) + "\n";

			if (index == size - 1)
				str += "\t</" + tag + ">";
		};

		template <typename T>
		static auto toTD(const T &val) -> std::string
		{
			std::stringstream ss;

			ss << "<td>" << val << "</td>";
			return ss.str();
		};
		template<> static auto toTD(const WeakString &)->std::string;
	};
};
};