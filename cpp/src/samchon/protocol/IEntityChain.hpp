#pragma once
#include <samchon/API.hpp>

namespace samchon
{
namespace protocol
{
	class Entity;

	/**
	 * @brief A chain of entity.
	 *
	 * @details
	 * <p> IEntityChain is an interface takes role of control about an entity by chain of responsibliy 
	 * pattern. It's suitable for a supplementary processes are required on an entity like calculation
	 * or historical regression about an entity. </p>
	 *
	 * @image html  cpp/protocol_entity.png
	 * @image latex cpp/protocol_entity.png
	 *
	 * @see samchon::protocol
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API IEntityChain
	{
	protected:
		/**
		 * @brief An entity related with the chain.
		 */
		Entity *entity;

	public:
		/**
		 * @brief Default Constructor.
		 *
		 * @param entity A related entity.
		 */
		IEntityChain(Entity*);
		virtual ~IEntityChain() = default;
	};
};
};