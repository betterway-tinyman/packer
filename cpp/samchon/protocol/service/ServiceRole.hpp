#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SystemRole.hpp>

namespace samchon
{
namespace protocol
{
namespace service
{
	class Service;

	class SAMCHON_FRAMEWORK_API ServiceRole
		: public SystemRole
	{
	private:
		typedef SystemRole super;

	protected:
		/**
		 * @brief Related service
		 */
		Service *service;

	public:
		/**
		 * @brief Construct from service and name
		 *
		 * @param service A service the role is belonged to
		 * @param name Name of the role
		 */
		ServiceRole(Service*, const std::string&);
		virtual ~ServiceRole() = default;

		virtual void sendData(std::shared_ptr<Invoke>) override;
	};
};
};
};