#pragma once
#include <samchon/API.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/library/XMLList.hpp>

#include <sstream>
#include <string>
#include <samchon/WeakString.hpp>

namespace std
{
	template<class T, class _Alloc> class list;
};
namespace samchon
{
namespace library
{
	/**
	 * @brief XML is a class representing xml object
	 *
	 * @details 
	 * <p> The XML class provides methods and properties for working with XML objects. </p>
	 * 
	 * <p> The XML class (along with the XMLList and Namespace) implements 
	 * the powerful XML-handling standard defined in ECMAScript for XML (E4X) specification. </p>
	 *
	 * <p> XML class has a recursive, hierarchical relationship. </p>
	 * 
	 * <p> All XML objects're managed by shared_ptr. </p>
	 *	\li XML contains XMLList from dictionary of shared pointer<XMLList>
	 *  \li XMLList contains XML from vector of shared pointer<XML>
	 *  \li Even if user creates an XML object directly, it's the basic principle to use shared pointer
	 * 
	 * @image html cpp/subset/library_xml.png
	 * @image latex cpp/subset/library_xml.png
	 *
	 * @note 
	 * <p> Parsing comment is not supported yet. </p>
	 * <p> It's not recommeded to creating an XML object which is not being managed by shared pointer. </p>
	 * 
	 * @warning 
	 * <p> Do not abuse values for expressing member variables. </p>
	 *
	 * <table>
	 *	<tr>
	 *		<th>Standard Usage</th>
	 *		<th>Non-standard usage abusing value</th>
	 *	</tr>
	 *	<tr>
	 *		<td>
	 *			\<memberList\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<member id='jhnam88' name='Jeongho+Nam' birthdate='1988-03-11' /\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<member id='master' name='Administartor' birthdate='2011-07-28' /\>\n
	 *			\</memberList\>
	 *		</td>
	 *		<td>
	 *			\<member\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<id\>jhnam88\</id\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<name\>Jeongho+Nam\</name\>\n
	 *			&nbsp;&nbsp;&nbsp;&nbsp; \<birthdate\>1988-03-11\</birthdate\>\n
	 *			\</member\>
	 *		</td>
	 *	</tr>
	 * </table>
	 * 
	 * @includelineno example/xml/main.cpp
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API XML
		: public HashMap<std::string, std::shared_ptr<XMLList>>
	{
	private:
		typedef HashMap<std::string, std::shared_ptr<XMLList>> super;

		/**
		 * @brief Tag name
		 *
		 * @details
		 *	\li \<<b>tag</b> label='property' /\>: tag => \"tag\"
		 *  \li \<<b>price</b> high='1500' low='1300' open='1450' close='1320' /\>: tag => \"price\"
		 */
		std::string tag;

		/**
		 * @brief Value of the XML
		 *
		 * @details
		 *  \li \<parameter name='age' type='int'\><b>26</b>\</parameter\>: value => 26
		 *	\li \<price high='1500' low='1300' open='1450' close='1320' /\>: tag => null
		 */
		std::string value;

		/**
		 * @brief Properties belongs to the XML
		 *
		 * @details
		 * A Dictionary of properties accessing each property by its key.
		 *	\li \<price <b>high='1500' low='1300' open='1450' close='1320'</b> /\>:
		 *		propertyMap => {{\"high\": 1500}, {\"low\": 1300}, {\"open\": 1450}, {\"close\", 1320}}
		 *	\li \<member <b>id='jhnam88' name='Jeongho+Nam' comment='Hello.+My+name+is+Jeongho+Nam'</b> \>:
		 *		propertyMap => {{\"id\", \"jhnam88\"}, {\"name\", \"Jeongho Nam <http://samchon.org>\"}, {\"comment\", \"Hello. My name is Jeongho Nam <http://samchon.org>\"}}
		 */
		HashMap<std::string, std::string> propertyMap;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 *
		 * @warning Declare XML to managed by shared pointer
		 */
		XML();

		/**
		 * @brief Copy Constructor
		 *
		 * @details
		 * Not copying (shared) pointer of children xml objects,
		 * but copying the real objects of children xml
		 */
		XML(const XML &);

		/**
		 * @brief Move Constructor
		 */
		XML(XML &&);

		/**
		 * @brief Constructor by string
		 *
		 * @details
		 * Parses a string so that constructs an XML object
		 *
		 * @param str A string representing xml object
		 * @warning Declare XML to managed by shared pointer
		 */
		XML(WeakString);

	private:
		/**
		 * @brief Protected Constructor by string for child
		 *
		 * @details
		 * Parses a string so that creates an XML object
		 * It is called for creating children XML objects from parent XML object.
		 *
		 * @param parent Parent object who will contains this XML object
		 * @param str A string to be parsed
		 */
		XML(XML*, WeakString &);

		void construct(WeakString &);
		void constructKey(WeakString &);
		void constructProperty(WeakString &);
		auto constructValue(WeakString &) -> bool;
		void constructChildren(WeakString &);

	public:
		/**
		 * @brief Add children xml objects by string representing them
		 *
		 * @param str A string representing xml objects whould be belonged to this XML
		 */
		void push_back(const WeakString &);

		/**
		 * @brief Add children xml
		 *
		 * @param xml An xml object you want to add
		 */
		void push_back(const std::shared_ptr<XML>);

		/**
		 * @brief Add all properties from another XML
		 *
		 * @details
		 * \par Copies all properties from target to here.
		 *
		 * @warning Not a category of assign, but an insert.
		 * @param xml Target xml object to deliver its properties
		 */
		void addAllProperty(const std::shared_ptr<XML>);

		/* -----------------------------------------------------------
			SETTERS
		----------------------------------------------------------- */
		/**
		 * @brief Set tag (identifier) of the XML
		 *
		 * @see XML::tag
		 */
		void setTag(const std::string &);

		/**
		 * @brief Set value of the XML
		 *
		 * @tparam _Ty Type of the value
		 * @param val The value to set
		 *
		 * @warning Do not abuse values for expressing member variables
		 * <table>
		 *	<tr>
		 *		<th>Standard Usage</th>
		 *		<th>Non-standard usage abusing value</th>
		 *	</tr>
		 *	<tr>
		 *		<td>
		 *			\<memberList\>\n
		 *			&nbsp;&nbsp;&nbsp;&nbsp;\<member id='jhnam88' name='Jeongho+Nam' birthdate='1988-03-11' /\>\n
		 *			&nbsp;&nbsp;&nbsp;&nbsp;\<member id='master' name='Administartor' birthdate='2011-07-28' /\>\n
		 *			\</memberList\>
		 *		</td>
		 *		<td>
		 *			\<member\>\n
		 *				\<id\>jhnam88\</id\>\n
		 *				\<name\>Jeongho+Nam\</name\>\n
		 *				\<birthdate\>1988-03-11\</birthdate\>\n
		 *			\</member\>
		 *		</td>
		 *	</tr>
		 * </table>
		 */
		template <typename T>
		void setValue(const T &val)
		{
			std::stringstream sstream;
			sstream << val;

			this->value = std::move(sstream.str());
		};
		template<> void setValue(const std::string &val)
		{
			this->value = val;
		};
		template<> void setValue(const WeakString &val)
		{
			this->value = val.str();
		};

		template <typename T>
		void insertValue(const std::string &tag, const T &val)
		{
			std::shared_ptr<XML> xml(new XML());
			xml->setTag(tag);
			xml->setValue(val);

			push_back(xml);
		};

		/**
		 * @brief Set a property with its key
		 */
		template<typename T>
		void setProperty(const std::string &key, const T &val)
		{
			std::stringstream sstream;
			sstream << val;

			propertyMap.set(key, sstream.str());
		};
		template<> void setProperty(const std::string &key, const std::string &val)
		{
			propertyMap.set(key, val);
		};
		template<> void setProperty(const std::string &key, const WeakString &val)
		{
			propertyMap.set(key, val.str());
		};

		/**
		 * @brief Erase a property by its key
		 *
		 * @param key The key of the property to erase
		 * @throw exception Unable to find the element
		 */
		void eraseProperty(const std::string&);

		/**
		 * @brief Remove all properties in the XML
		 */
		void clearProperties();

		/* -----------------------------------------------------------
			GETTERS
		----------------------------------------------------------- */
	public:
		/**
		 * @brief Get key; identifer of the XML
		 *
		 * @return tag, identifer of the XML
		 * @see XML::tag
		 */
		auto getTag() const->std::string;

		/**
		 * @brief Get value of the XML
		 */
		template<class T = std::string> auto getValue() const -> T
		{
			std::stringstream sstream;
			sstream << this->value;

			T val;
			sstream >> val;

			return std::move(val);
		};

		template<> auto getValue() const -> std::string
		{
			return value;
		};
		template<> auto getValue() const -> WeakString
		{
			return value;
		};

		/**
		 * @brief Get property
		 */
		template<class T = std::string> auto getProperty(const std::string &key) const -> T
		{
			std::stringstream sstream;
			sstream << propertyMap.get(key);

			T val;
			sstream >> val;

			return std::move(val);
		};

		template<> auto getProperty(const std::string &key) const -> std::string
		{
			return propertyMap.get(key);
		};
		template<> auto getProperty(const std::string &key) const -> WeakString
		{
			return propertyMap.get(key);
		};

		/**
		 * @brief Test wheter a property exists or not
		 */
		auto hasProperty(const std::string &) const -> bool;

		/**
		 * @brief Get propertyMap
		 */
		auto getPropertyMap() const -> const HashMap<std::string, std::string>&;

		/* -----------------------------------------------------------
			FILTERS
		----------------------------------------------------------- */
	private:
		auto calcMinIndex(const std::vector<size_t>&) const->size_t;

		auto encodeValue(const WeakString &) const->std::string;
		auto decodeValue(const WeakString &) const->std::string;
		auto encodeProperty(const WeakString &) const->std::string;
		auto decodeProperty(const WeakString &) const->std::string;

		/* -----------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------- */
	public:
		/**
		 * @brief Get the string content.
		 * @details Returns a string representation of the XML and its all children.
		 *
		 * @return A string represents the XML.
		 */
		auto toString(size_t level = 0) const->std::string;
	};

	SAMCHON_FRAMEWORK_EXTERN template class SAMCHON_FRAMEWORK_API std::shared_ptr<XML>;
	SAMCHON_FRAMEWORK_EXTERN template class SAMCHON_FRAMEWORK_API HashMap<std::string, std::shared_ptr<XMLList>>;
	SAMCHON_FRAMEWORK_EXTERN template class SAMCHON_FRAMEWORK_API HashMap<std::string, std::string>;
};
};