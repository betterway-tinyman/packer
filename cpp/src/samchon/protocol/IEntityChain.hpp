#pragma once

namespace samchon
{
namespace protocol
{
	class EntityBase;

	/**
	 * @brief A chain of entity.
	 *
	 * @details
	 * <p> IEntityChain is an interface takes role of control about an entity by chain of responsibliy 
	 * pattern. It's suitable for a supplementary processes are required on an entity like calculation
	 * or historical regression about an entity. </p>
	 *
	 * ![Basic Components](http://samchon.github.io/framework/images/design/cpp_class_diagram/protocol_entity.png)
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class IEntityChain
	{
	protected:
		/**
		 * @brief An entity related with the chain.
		 */
		EntityBase *entity;

	public:
		/**
		 * @brief Default Constructor.
		 *
		 * @param entity A related entity.
		 */
		IEntityChain(EntityBase *entity)
		{
			entity = this->entity;
		};
		virtual ~IEntityChain() = default;
	};
};
};