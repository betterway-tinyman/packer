#pragma once
#include <samchon/protocol/Entity.hpp>

#include <sstream>
#include <samchon/ByteArray.hpp>
#include <samchon/WeakString.hpp>

namespace samchon
{
namespace protocol
{
	class Invoke;

	/**
	 * @brief A parameter of an Invoke.
	 *
	 * @details
	 * <p> A parameter with its name, type and value in an Invoke message. </p>
	 *
	 * <p> InvokeParameter supports 4 types pre-defined. </p>
	 *	\li number
	 *	\li string
	 *	\li XML
	 *	\li ByteArray
	 *
	 * <p> You can specify InvokeParameter to have some type which is not one of the basic 4 types, but
	 * values of the custom type must be enable to expressed by one of those types; number, string, XML
	 * and ByteArray. </p>
	 *
	 * <p> @image html  cpp/protocol_invoke.png
	 *	   @image latex cpp/protocol_invoke.png </p>
	 *
	 * \par Example Sources
	 *	\li example::invoke
	 *	\li example::interaction
	 *
	 * @note
	 * <p> A member variable void* representing any type of value is deprecated. </p>
	 *
	 * <p> Since C++11, method of void pointer (Declare a variable as void pointer whose type can
	 * not specified. And convert the pointer type to specified one for each case) is recommended
	 * to avoid. </p>
	 *
	 * <p> As that reason, the <i>void *value</i> was a candidate to be deprecated or to be
	 * replaced to <i>boost::any</i>. And the <i>void* value</i> is really deprecated since
	 * version v1.0. </p>
	 *
	 * \par [Inherited]
	 *		@copydetails protocol::Entity
	 *
	 * @see protocol::Invoke
	 * @see samchon::protocol
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class InvokeParameter
		: public virtual Entity
	{
		friend class Invoke;

	protected:
		typedef Entity super;

		/**
		 * @brief A name can represent the parameter
		 */
		std::string name;

		/**
		 * @brief Type of the parameter
		 *
		 * @details
		 * <p> List of pre-defined types </p>
		 *	\li number
		 *	\li string
		 *	\li XML
		 *	\li ByteArray
		 */
		std::string type;

		/**
		 * @brief A string value if the type is "string" or "number"
		 */
		std::string str;

		/**
		 * @brief An XML object if the type is "XML"
		 */
		std::shared_ptr<library::XML> xml;

		/**
		 * @brief A binary value if the type is "ByteArray"
		 */
		ByteArray byte_array;

	public:
		/* ----------------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		InvokeParameter() : super()
		{
		};

		/**
		 * @brief Construct from arguments
		 *
		 * @details
		 * <p> Specifies all arguments of an InvokeParameter. You can specify a custom type, that is not
		 * one of number or string, but value of the type must be enable to expressed by a string. </p>
		 *
		 * <p> If you want to express an object or container group, use XML instead;
		 * <i>Invoke::InvokeParameter(string, XML)</i>. </p>
		 *
		 * @param name A name can represent the InvokeParameter
		 * @param type Type of value in the InvokeParameter
		 * @param value A value capsuled by a string
		 */
		InvokeParameter(const std::string &name, const std::string &type, const std::string &val)
			: super()
		{
			this->name = name;
			this->type = type;
			this->str = val;
		};

		/**
		 * @brief Construct with its name and a value
		 *
		 * @details
		 * <p> Type of the InvokeParameter will be determined automatically. </p>
		 *
		 * <p> Type supported in the template constructor: </p>
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
		 * 	<li> XML </li>
		 *	<ul>
		 *		<li> std::shared_ptr<library::XML> </li>
		 *		<li> Entity; protocol::Entity::toXML() </li>
		 *	</ul>
		 *  <li> ByteArray </li>
		 * </ul>
		 *
		 * @tparam _Ty Type of value
		 */
		template <typename T>
		InvokeParameter(const std::string &name, const T &val)
			: super()
		{
			this->name = name;

			construct_by_varadic_template(val);
		};

		InvokeParameter(const std::string &name, const char *ptr)
		{
			this->name = name;
			this->type = "string";

			this->str = ptr;
		};

		//MOVE CONSTRUCTORS
		/**
		 * @brief Construct from the name and a moved string
		 */
		InvokeParameter(const std::string &name, std::string &&str)
		{
			this->name = name;
			this->type = "string";

			this->str = move(str);
		};

		/**
		 * @brief Construct from name and a moved ByteArray
		 */
		InvokeParameter(const std::string &name, ByteArray &&byte_array)
		{
			this->name = name;
			this->type = "ByteArray";

			this->byte_array = move(byte_array);
		};

		virtual ~InvokeParameter() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			if (xml->hasProperty("name") == true)
				this->name = xml->getProperty("name");
			else
				this->name = "";

			this->type = xml->getProperty("type");

			if (type == "XML")
				this->xml = xml->begin()->second->at(0);
			else if (type == "ByteArray")
			{
				size_t size = xml->getValue<size_t>();

				byte_array.reserve(size);
			}
			else
				this->str = xml->getValue();
		};

		auto byteArrayCapacity() const -> size_t
		{
			return byte_array.capacity();
		};

		void setByteArray(ByteArray &&ba)
		{
			byte_array = move(ba);
		};

	protected:
		template <typename T>
		void construct_by_varadic_template(const T &val)
		{
			this->type = "number";

			std::stringstream sstream;
			sstream << val;

			this->str = move(sstream.str());
		};
		template<> void construct_by_varadic_template(const std::string &str)
		{
			this->type = "string";
			this->str = str;
		};
		template<> void construct_by_varadic_template(const WeakString &wstr)
		{
			this->type = "string";
			this->str = wstr.str();
		};
		template<> void construct_by_varadic_template(const ByteArray &byte_array)
		{
			this->type = "ByteArray";
			this->byte_array = byte_array;
		};

		template<> void construct_by_varadic_template(const std::shared_ptr<library::XML> &xml)
		{
			this->type = "XML";
			this->xml = xml;
		};

	public:
		/* ----------------------------------------------------------
			GETTERS
		---------------------------------------------------------- */
		virtual auto key() const -> std::string override
		{
			return name;
		};

		/**
		 * @brief Get name
		 */
		auto get_name() const->std::string
		{
			return name;
		};

		/**
		 * @brief Get type
		 */
		auto getType() const->std::string
		{
			return type;
		};

		/**
		 * @brief Get value
		 *
		 * @tparam _Ty Type of value to get
		 */
		template<typename T> auto getValue() const -> T
		{
			double val = std::stod(str);

			return (T)val;
		};
		template<> auto getValue() const -> std::string
		{
			return str;
		};
		template<> auto getValue() const -> WeakString
		{
			return str;
		};
		template<> auto getValue() const -> std::shared_ptr<library::XML>
		{
			return xml;
		};
		template<> auto getValue() const -> ByteArray
		{
			return byte_array;
		};

		/**
		 * @brief Get value as XML object
		 * @details Same with getValue< std::shared_ptr<library::XML> >();
		 */
		auto getValueAsXML() const -> std::shared_ptr<library::XML>
		{
			return xml;
		};

		/**
		 * @brief Reference value
		 *
		 * @tparam _Ty Type of value to reference
		 */
		template <typename T> auto referValue() const -> const T&;
		template<> auto referValue() const -> const std::string&
		{
			return str;
		};
		template<> auto referValue() const -> const ByteArray&
		{
			return byte_array;
		};

		/**
		 * @brief Move value
		 *
		 * @tparam _Ty Type of value to move
		 */
		template <typename T> auto moveValue() -> T;
		template<> auto moveValue() -> std::string
		{
			return move(str);
		};
		template<> auto moveValue() -> ByteArray
		{
			return move(byte_array);
		};

	public:
		/* ----------------------------------------------------------
			EXPORTERS
		---------------------------------------------------------- */
		virtual auto TAG() const -> std::string override
		{
			return "parameter";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> &xml = super::toXML();

			if (name.empty() == false)
				xml->setProperty("name", name);
			xml->setProperty("type", type);

			if (type == "XML")
				xml->push_back(this->xml);
			else if (type == "ByteArray")
				xml->setValue(byte_array.size());
			else
				xml->setValue(str);

			return xml;
		};
	};
};
};