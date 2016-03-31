#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace protocol
{
namespace master
{
	class PRMasterHistory;
	class ParallelSystemArray;
	class ParallelSystem;

	/**
	 * @brief A reported history of an Invoke message
	 *
	 * @details
	 * \par [Inherited]
	 *		@copydetails protocol::InvokeHistory
	 */
	class SAMCHON_FRAMEWORK_API PRInvokeHistory
		: public InvokeHistory
	{
		friend class ParallelSystemArray;
		friend class PRMasterHistory;

	private:
		typedef InvokeHistory super;

	public:
		/**
		 * @brief A history of master.
		 */
		PRMasterHistory *masterHistory;

		/**
		 * @brief A system the history is belonged to.
		 */
		ParallelSystem *system;

		/**
		 * @brief Start index
		 */
		size_t index;

		/**
		 * @brief The Size
		 */
		size_t size;

	public:
		/* --------------------------------------------------------------------
			CONSTRUCTORS
		-------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		PRInvokeHistory();

		/**
		 * @brief Default Constructor.
		 *
		 * @param masterHistory A master history that the segment history is belonged to.
		 * @param system A system that the history is belonged to.
		 * @param invoke An invoke message that the history must represent.
		 */
		PRInvokeHistory(PRMasterHistory*, ParallelSystem*, std::shared_ptr<Invoke>);

		virtual ~PRInvokeHistory() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		virtual void notifyEnd() override;

		/* --------------------------------------------------------------------
			GETTERS
		-------------------------------------------------------------------- */
		/**
		 * @brief Get index.
		 */
		inline auto getIndex() const->size_t;

		/**
		 * @brief Get size.
		 */
		auto getSize() const->size_t;

		/**
		 * @brief Calculate average of elapsed time for each segmentation.
		 */
		auto calcAverageElapsedTime() const -> double;

		/* --------------------------------------------------------------------
			EXPORTER
		-------------------------------------------------------------------- */
		virtual auto toXML() const->std::shared_ptr<library::XML> override;
	};
};
};
};