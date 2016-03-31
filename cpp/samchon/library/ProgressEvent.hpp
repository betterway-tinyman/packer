#pragma once
#include <samchon/API.hpp>

#include <samchon/library/Event.hpp>

namespace samchon
{
namespace library
{
	/** 
	 * @brief Event representing a progress
	 *
	 * @details
	 * \par [Inherited]
	 * @copydetails library::Even
	 */
	class SAMCHON_FRAMEWORK_API ProgressEvent
		: public Event
	{
	private:
		typedef Event super;

		enum : int
		{
			ACTIVATE = 1,
			COMPLETE = 2,
			REMOVED = -1
		};

	protected:
		/**
		 * @brief The number of current progress
		 */
		size_t numerator;

		/**
		 * @brief The number of total progress
		 * @warning denominator cannot be zero
		 */
		size_t denominator;

	public:
		enum : int
		{
			PROGRESS = 11
		};

		/**
		 * @brief Construct from source and progress
		 *
		 * @param source Source of the event; who made the event
		 * @param numerator The number of current progress
		 * @param denominator The number of total progress
		 *
		 * @throw exception denominator is zero
		 */
		ProgressEvent(EventDispatcher *, size_t, size_t);
		virtual ~ProgressEvent() = default;

		/**
		 * @brief Get numerator, number of current progress
		 */
		auto getNumerator() const->size_t;

		/**
		 * @brief Get denominator, number of total progress
		 */
		auto getDenominator() const->size_t;

		/**
		 * @brief Get percentage of the progress
		 */
		auto getPercent() const -> double;
	};
};
};