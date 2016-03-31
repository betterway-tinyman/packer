#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace protocol
{
	/**
	 * @brief An array of InvokeHistory.
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API InvokeHistoryArray
		: public SharedEntityArray<InvokeHistory>
	{
	protected:
		typedef SharedEntityArray<InvokeHistory> super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		InvokeHistoryArray();
		virtual ~InvokeHistoryArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->InvokeHistory* override;

	public:
		virtual auto TAG() const->std::string;
		virtual auto CHILD_TAG() const->std::string;
	};
};
};