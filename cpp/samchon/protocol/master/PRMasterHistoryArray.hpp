#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/master/PRInvokeHistoryArray.hpp>

#include <atomic>

namespace samchon
{
namespace protocol
{
namespace master
{
	class ParallelSystemArray;
	class PRMasterHistory;

	/**
	 * @brief An array of invoke histories of master.
	 *  
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API PRMasterHistoryArray
		: public PRInvokeHistoryArray
	{
		friend class PRMasterHistory;

	protected:
		typedef PRInvokeHistoryArray super;

		/**
		 * @brief A master the array of histories is belonged to.
		 */
		ParallelSystemArray *master;

	public:
		/**
		 * @brief Construct from a master.
		 */
		PRMasterHistoryArray(ParallelSystemArray*);
		virtual ~PRMasterHistoryArray() = default;

		SHARED_ENTITY_ARRAY_ELEMENT_ACCESSOR_HEADER(PRMasterHistory)
	};
};
};
};