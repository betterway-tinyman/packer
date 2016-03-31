#pragma once
#include <samchon/API.hpp>

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
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API InvokeParameter
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
		ByteArray byteArray;

	public:
		/* ----------------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		InvokeParameter();

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
		InvokeParameter(const std::string &, const std::string &, const std::string &);

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
		template <typename _Ty>
		InvokeParameter(const std::string &name, const _Ty &val)
			: super()
		{
			this->name = name;

			construct_by_varadic_template(val);
		};

		InvokeParameter(const std::string &, const char*);

		//MOVE CONSTRUCTORS
		/**
		 * @brief Construct from the name and a moved string
		 */
		InvokeParameter(const std::string &, std::string &&);

		/**
		 * @brief Construct from name and a moved ByteArray
		 */
		InvokeParameter(const std::string &, ByteArray &&);

		virtual ~InvokeParameter() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		auto reservedByteArraySize() const->size_t;
		void setByteArray(ByteArray &&);

	protected:
		template <typename _Ty>
		void construct_by_varadic_template(const _Ty &val)
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
		template<> void construct_by_varadic_template(const ByteArray &byteArray)
		{
			this->type = "ByteArray";
			this->byteArray = byteArray;
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
		virtual auto key() const->std::string override;

		/**
		 * @brief Get name
		 */
		auto getName() const->std::string;

		/**
		 * @brief Get type
		 */
		auto getType() const->std::string;

		/**
		 * @brief Get value
		 *
		 * @tparam _Ty Type of value to get
		 */
		template<typename _Ty> auto getValue() const -> _Ty
		{
			std::stringstream sstream;
			sstream << this->str;

			_Ty val;
			sstream >> val;

			return move(val);
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
			return byteArray;
		};

		/**
		 * @brief Get value as XML object
		 * @details Same with getValue< std::shared_ptr<library::XML> >();
		 */
		auto getValueAsXML() const->std::shared_ptr<library::XML>;

		/**
		 * @brief Reference value
		 *
		 * @tparam _Ty Type of value to reference
		 */
		template <typename _Ty> auto referValue() const -> const _Ty&;
		template<> auto referValue() const -> const std::string&
		{
			return str;
		};
		template<> auto referValue() const -> const ByteArray&
		{
			return byteArray;
		};

		/**
		 * @brief Move value
		 *
		 * @tparam _Ty Type of value to move
		 */
		template <typename _Ty> auto moveValue()->_Ty;
		template<> auto moveValue() -> std::string
		{
			return move(str);
		};
		template<> auto moveValue() -> ByteArray
		{
			return move(byteArray);
		};

	public:
		/* ----------------------------------------------------------
			EXPORTERS
		---------------------------------------------------------- */
		virtual auto TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;

		/**
		 * @brief Get a string of sql statement used to archive history log
		 */
		auto toSQL() const->std::string;
	};
};
};