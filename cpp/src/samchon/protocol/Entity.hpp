#pragma once

#include <string>
#include <memory>

#include <samchon/library/XML.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An entity, a standard data class
	 * 
	 * @details
	 * <p> Entity is a class for standardization of expression method using on network I/O by XML. If 
	 * Invoke is a standard message protocol of Samchon Framework which must be kept, Entity is a 
	 * recommended semi-protocol of message for expressing a data class. Following the semi-protocol
	 * Entity is not imposed but encouraged. </p>
	 *
	 * <p> As we could get advantages from standardization of message for network I/O with Invoke, 
	 * we can get additional advantage from standardizing expression method of data class with Entity. 
	 * We do not need to know a part of network communication. Thus, with the Entity, we can only 
	 * concentrate on entity's own logics and relationships between another entities. Entity does not
	 * need to how network communications are being done. </p>
	 *
	 * <p> @image html  cpp/protocol_entity.png
	 *	   @image latex cpp/protocol_entity.png </p>
	 * 
	 * \par Example Sources
	 *	\li example::entity
	 *	\li example::packer
	 *	\li example::tsp
	 * 
	 * @note
	 * <p> I say repeatedly. Expression method of Entity is recommended, but not imposed. It's a semi
	 * protocol for network I/O but not a essential protocol must be kept. The expression method of
	 * Entity, using on network I/O, is expressed by XML string. </p>
	 *
	 * <p> If your own network system has a critical performance issue on communication data class, 
	 * it would be better to using binary communication (with ByteArray or boost::serialization).
	 * Don't worry about the problem! Invoke also provides methods for binary data (ByteArray). </p>
	 *
	 * @see protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Entity
	{
	public:
		/**
		 * @brief A tag name when represented by XML
		 *
		 * @return A tag name
		 */
		virtual auto TAG() const->std::string = 0;

	public:
		virtual ~Entity() = default;

		/**
		 * @brief Construct data of the Entity from an XML object
		 *
		 * @details
		 * <p> Overrides the construct() method and fetch data of member variables from the XML. </p>
		 *
		 * <p> By recommended guidance, data representing member variables are contained in properties
		 * of the put XML object. </p>
		 *
		 * @param xml An xml used to construct data of entity
		 */
		virtual void construct(std::shared_ptr<library::XML>) = 0;

		/**
		 * @brief Get a key that can identify the Entity uniquely.
		 *
		 * @details
		 * <p> If identifier of the Entity is not atomic value, returns a string represents the composite
		 * identifier. If identifier of the Entity is not string, converts the identifier to string and
		 * returns the string. </p>
		 *
		 * @return An identifier
		 */
		virtual auto key() const -> std::string
		{
			return "";
		};

		/**
		 * @brief Get an XML object represents the Entity.
		 *
		 * @details
		 * <p> Returns an XML object that can represents the Entity containing member variables into properties. </p>
		 *
		 * <p> A member variable (not object, but atomic value like number, string or date) is categorized
		 * as a property within the framework of entity side. Thus, when overriding a toXML() method and
		 * archiving member variables to an XML object to return, puts each variable to be a property
		 * belongs to only an XML object. </p>
		 *
		 * <p> Don't archive the member variable of atomic value to XML::value causing enormouse creation
		 * of XML objects to number of member variables. An Entity must be represented by only an XML
		 * instance (tag). </p>
		 *
		 * <table>
		 *	<tr>
		 *		<th> Standard Usage </th>
		 *		<th> Non-standard usage abusing value </th>
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
		 * @return An XML object representing the Entity.
		 */
		virtual auto toXML() const -> std::shared_ptr<library::XML>
		{
			auto xml = std::make_shared<library::XML>();
			xml->setTag(this->TAG());

			return xml;
		}
	};
};
};