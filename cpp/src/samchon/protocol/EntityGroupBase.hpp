#pragma once

#include <string>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An iternface for entity group
	 *
	 * @details
	 * <p> IEntityGroup is an interface for a container of entity. It does not have any special role or 
	 * function. The IEntityGorup is only for discriminate an entity is single instance or gorup of
	 * entity. </p>
	 *
	 * <ul>
	 *	<li> Group of entity is divided into StaticEntityArray and EntityGroup. </li>
	 *	<ul>
	 *		<li> Even they are template classes so that cannot be discriminated globally. </li>
	 *	</ul>
	 *	<li> dynamic_cast<IEntityGroup>(<i>some entity instance</i>) </li>
	 *	<ul>
	 *		<li> If true then, it's not single entity instance, but group of entity. </li>
	 *		<li> False then, it's a single instance. </li>
	 *	</ul>
	 * </ul>
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class EntityGroupBase
	{
	public:
		virtual ~EntityGroupBase() = default;

	public:
		/**
		* @brief A tag name of children
		 *
		 * @details
		 * <p>&lt; TAG&gt;\n
		 * &nbsp;&nbsp;&nbsp;&nbsp; &lt;CHILD_TAG /&gt;\n
		 * &nbsp;&nbsp;&nbsp;&nbsp; &lt;CHILD_TAG /&gt;\n
		 * &lt;/TAG&gt; </p>
		 */
		virtual auto CHILD_TAG() const->std::string = 0;
	};
};
};