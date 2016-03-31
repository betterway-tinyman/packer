#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/InvokeHistoryArray.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class PRInvokeHistory;

	/**
	 * @brief An array of PRInvokeHistory.
	 *
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API PRInvokeHistoryArray
		: public InvokeHistoryArray
	{
	protected:
		typedef InvokeHistoryArray super;

	public:
		/**
		 * @brief Default Constructor.
		 */
		PRInvokeHistoryArray();
		virtual ~PRInvokeHistoryArray() = default;

	protected:
		virtual auto createChild(std::shared_ptr<library::XML>)->InvokeHistory* override;

	public:
		SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(PRInvokeHistory)
	};
};
};
};