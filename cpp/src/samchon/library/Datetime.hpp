#pragma once
#include <samchon/API.hpp>

#include <samchon/library/Date.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Date and time
	 *
	 * @details
	 * <p> Datetime is a Date having hours, minutes and seconds addictionally. </p>
	 *
	 * \par [Inherited]
	 * @copydetails library::Date
	 */
	class SAMCHON_FRAMEWORK_API Datetime
		: public Date
	{
	private:
		typedef Date super;

	public:
		/* --------------------------------------------------------------------------
			CONSTRUCTORS
		-------------------------------------------------------------------------- */
		/**
		 * @default Default Constructor
		 *
		 * @details The datetime will be now (Now from operating system)
		 */
		Datetime();

		/**
		 * @copydoc Date::Date(const super&)
		 */
		Datetime(const std::chrono::system_clock::time_point&);

		/**
		 * @copydoc Date::Date(super&&)
		 */
		Datetime(std::chrono::system_clock::time_point&&);

		/**
		 * @brief Construct from datetime elements
		 *
		 * @param year ex) 2015
		 * @param month January to December. 1 ~ 12
		 * @param date Day in month. 1 ~ 31
		 * @param hour Hours (0-23)
		 * @param min Minutes (0-59)
		 * @param sec Seconds (0-59)
		 *
		 * @throw invalid_argument parameter is out of its own range.
		 */
		Datetime(int year, int month, int date, int hour = 0, int min = 0, int sec = 0);

		/**
		 * @brief Construct from string
		 *
		 * @param str A string representing the date. (1991-01-01 09:27:03)
		 * @throw invalid_argument parameter is out of its own range.
		 */
		Datetime(const std::string &);

		/**
		 * @brief Construct from WeakString
		 *
		 * @param wstr A weak_string expressing the date. (1991-01-01 09:27:03)
		 * @throw invalid_argument parameter is out of its own range.
		 */
		Datetime(const WeakString &);

		/**
		 * @brief Construct from linux_time
		 *
		 * @param linuxTime linux_time to be converted
		 */
		Datetime(long long linuxTime);
		virtual ~Datetime() = default;

		/**
		* @brief Setter by string
		*
		* @param str wstr A string expressing the date. (1991-01-01 09:27:03)
		* @throw invalid_argument parameter is out of its own range.
		*/
		void set(const std::string &);

		/**
		 * @brief deque by weak_string
		 *
		 * @param wstr A weak_string expressing the date. (1991-01-01 09:27:03)
		 * @throw invalid_argument parameter is out of its own range.
		 */
		virtual void set(const WeakString &) override;

		/**
		 * @throw invalid_argument parameter is out of range (ex: month -> 13)
		 */
		void set(int year, int month, int date, int hours = 0, int minutes = 0, int seconds = 0);

		/**
		 * @inheritDoc
		 */
		void set(long long linuxTime);

		/* --------------------------------------------------------------------------
			SETTERS
		-------------------------------------------------------------------------- */
		//SETTERS
		/**
		 * @copydoc Date::setYear()
		 */
		virtual void setYear(int);

		/**
		 * @copydoc Date::setMonth()
		 */
		virtual void setMonth(int);

		/**
		 * @copydoc Date::setDate()
		 */
		virtual void setDate(int);

		/**
		 * @brief Set hour
		 *
		 * @param val Target hour (0 - 23)
		 * @throw invalid_argument When hour is out of range
		 */
		void setHour(int);

		/**
		 * @brief Set minute
		 *
		 * @param val Target minute (0 - 59)
		 * @throw invalid_argument When minute is out of range
		 */
		void setMinute(int);
		/**
		 * @brief Set hour
		 *
		 * @param val Target second (0 - 59)
		 * @throw invalid_argument When second is out of range
		 */
		void setSecond(int);

		//ADDERS
		/**
		 * @copydoc Date::addYear()
		 */
		virtual void addYear(int) override;

		/**
		 * @copydoc Date::addMonth()
		 */
		virtual void addMonth(int) override;

		/**
		 * @copydoc Date::addWeek()
		 */
		virtual void addWeek(int) override;

		/**
		 * @copydoc Date::addDate()
		 */
		virtual void addDate(int) override;

		/**
		 * @brief Add hours
		 *
		 * @details
		 * <p> It's same with operator+=(chrono::hours(val)) </p>
		 *
		 * <p> Do not worry about the out of range of the hour(val)
		 * Parent items like date, month and years will be adjusted automatically. </p>
		 *
		 * @param val hours to add
		 */
		void addHour(int);
		/**
		 * @brief Add minutes
		 *
		 * \par It's same with operator+=(chrono::minutes(val))
		 *
		 * \par
		 * Do not worry about the out of range of the minutes(val)
		 * Parent items like date, month and years will be adjusted automatically.
		 *
		 * @param val minutes to add
		 */
		void addMinute(int);

		/**
		 * @brief Add seconds
		 *
		 * @details
		 * \par It's same with operator+=(chrono::seconds(val))
		 *
		 * \par
		 * Do not worry about the out of range of the seconds(val)
		 * Parent items like date, month and years will be adjusted automatically.
		 *
		 * @param val seconds to add
		 */
		void addSecond(int);

		/* --------------------------------------------------------------------------
			GETTERS
		-------------------------------------------------------------------------- */
		/**
		 * @brief Get hour
		 *
		 * @return hour of the Datetime to integer
		 */
		auto getHour() const -> int;
		/**
		 * @brief Get minute
		 *
		 * @return minute of the Datetime to integer
		 */
		auto getMinute() const -> int;
		/**
		 * @brief Get second
		 *
		 * @return second of the Datetime to integer
		 */
		auto getSecond() const -> int;

		/* --------------------------------------------------------------------------
			EXPORTERS
		-------------------------------------------------------------------------- */
		/**
		* @brief Converts the Datetime to std::string
		*
		* @return std::string expressing the Datetime
		*/
		virtual auto toString() const->std::string override;
	};
};
};