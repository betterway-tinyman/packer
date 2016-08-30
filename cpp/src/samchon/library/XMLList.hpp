#pragma once
#include <samchon/API.hpp>

#include <vector>
#include <memory>

namespace samchon
{
namespace library
{
	class /*SAMCHON_FRAMEWORK_API*/ XML;

	/**
	 * @brief A list of XML, tags are same.
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	typedef std::vector<std::shared_ptr<XML>> XMLList;

	//SAMCHON_FRAMEWORK_EXTERN template class SAMCHON_FRAMEWORK_API std::vector<std::shared_ptr<XML>>;
};
};