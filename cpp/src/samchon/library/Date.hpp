#pragma once
#include <chrono>

#include <array>
#include <string>

#include <samchon/WeakString.hpp>
#include <samchon/library/StringUtil.hpp>
#include <samchon/library/IOperator.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Date (year, month, day in month)
	 * 
	 * @details
	 * <p> Date class is a chrono::time_point\<std::chrono::system_clock\>. </p>
	 * <p> Date represents the date with year, month and day in month.
	 * The basic date (local time) is from your operating system. </p>
	 *
	 * \par [Inherited]
	 * <p> A time_point object expresses a point in time relative to a clock's epoch. </p>
	 *
	 * <p> Internally, the object stores an object of a duration type, and uses the Clock 
	 * type as a reference for its epoch. </p>
	 *
	 * Referenced comments of std::chrono::time_point
	 *	\li http://www.cplusplus.com/reference/chrono/time_point/
	 *
	 * @image html cpp/subset/library_date.png
	 * @image latex cpp/subset/library_date.png
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Date
		: public std::chrono::system_clock::time_point
	{
	private:
		typedef std::chrono::system_clock::time_point super;

		/**
		 * @brief Calculates how many seconds have flowen since 0000-01-01 00:00:00
		 * @details This static method is used for calculate the linux_time (since 1970-01-01 09:00:00)
		 *
		 * @return linux_time of the Date
		 */
		static auto calc_seconds(int year, int month, int date) -> long long
		{
			std::array<int, 12> &months = calc_last_dates(year);
			long long total;

			//연, 월, 일의 총 일수
			total = (year - 1) * 365 + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400);

			//달수만큼 날자를 더함
			for (int i = 0; i < month - 1; i++)
				total += months[i];
			total += date;

			//일자에 3,600을 곱한다
			total = (total * 24 * 60 * 60);
			return total;
		};

		/**
		* @brief Calculates and gets an array of final date of each month for that year.
		*
		* @param year The target year
		* @return Array of fianl date of each month for that year
		*/
		static auto calc_last_dates(int year) -> std::array<int, 12>
		{
			//Last dates of each month
			static std::array<int, 12> months = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

			//When the year contains leaf month, then make expiration date of Feburary to 29
			if (year % 4 == 0)
				if (!(year % 100 == 0 && year % 400 != 0))
					months[1] = 29;

			return months;
		};

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 *
		 * @detail The date will be now (Now's date from operating system)
		 */
		Date() : super(std::chrono::system_clock::now())
		{
		};

		/**
		 * @brief Copy Constructor
		 * @details Copy the date from another Date
		 *
		 * @param date Target to be copied
		 */
		Date(const super &date) : super(date)
		{
		};

		/**
		 * @briref Move Constructor
		 * @details Gets the date from other and truncate the other
		 *
		 * @param date Target to be moved
		 */
		Date(super &&date) : super(move(date))
		{
		};

		/**
		 * @brief Construct from date elements
		 *
		 * @param year ex) 2015
		 * @param month January to December. 1 ~ 12
		 * @param date day in month. 1 ~ 31
		 * @throw invalid_argument month or date is invalid.
		 */
		Date(int year, int month, int date, int hour = 0, int min = 0, int sec = 0)
		{
			set(year, month, date, hour, min, sec);
		};

		/**
		 * @brief Construct from string
		 *
		 * @param str A string represents the date. (1991-01-01)
		 * @throw invalid_argument month or date is invalid.
		 */
		Date(const std::string &str)
		{
			set(str);
		};

		Date(const WeakString &wstr)
		{
			set(wstr);
		};

		/**
		 * @brief Construct from linux_time
		 *
		 * @param linuxTime linux_time to be converted
		 */
		Date(long long linuxTime)
		{
			set(linuxTime);
		};

		/**
		 * @brief Setter by string
		 *
		 * @param str Y-m-d
		 * @throw invalid_argument month or date is invalid.
		 */
		void set(const WeakString &wstr)
		{
			int year, month, date, hour, min, sec;
			WeakString date_wstr;
			WeakString time_wstr;

			size_t delimiter = wstr.find(" ");

			if (delimiter == std::string::npos)
				date_wstr = wstr;
			else
			{
				date_wstr = wstr.substr(0, delimiter);
				time_wstr = wstr.substr(delimiter);
			}

			std::vector<WeakString> &ymd = date_wstr.split("-");
			year = std::stoi(ymd[0]);
			month = std::stoi(ymd[1]);
			date = std::stoi(ymd[2]);

			if (time_wstr.empty() == true)
				hour = min = sec = 0;
			else
			{
				std::vector<WeakString> &hms = time_wstr.between(" ").split(":");
				int hour = std::stoi(hms[0]);
				int min = std::stoi(hms[1]);
				int sec;

				if (hms.size() == 3)
					sec = std::stoi(hms[2]);
				else
					sec = 0;
			}

			set(year, month, date, hour, min, sec);
		};

		/**
		 * @brief Setter by elements of date
		 *
		 * @param year ex) 2015
		 * @param month January to December. 1 ~ 12
		 * @param date day in month. 1 ~ 31
		 * @throw invalid_argument month or date is invalid.
		 */
		void set(int year, int month, int date, int hour = 0, int min = 0, int sec = 0)
		{
			std::array<int, 12> &monthArray = calc_last_dates(year);

			if (month > 12)
				throw std::invalid_argument("month is over 12");
			if (date > monthArray[month - 1])
				throw std::invalid_argument("date is over expiration date");

			long long linuxTime = calc_seconds(year, month, date);
			linuxTime -= (calc_seconds(1970, 1, 1) + 9 * 60 * 60);
			linuxTime += hour * 60 * 60 + min * 60 + sec;

			set(linuxTime);
		};

		/**
		 * @brief Setter by linux_time
		 *
		 * @param linuxTime linux_time to be converted
		 */
		void set(long long linuxTime)
		{
			*this = std::chrono::system_clock::from_time_t(linuxTime);
		};

	public:
		/* =========================================================
			ACCESSORS
				- GETTERS
				- SETTERS
				- ADDERS
		============================================================
			- GETTERS
		--------------------------------------------------------- */
		/**
		 * @brief Get year of the Date
		 *
		 * @return Year
		 */
		auto getYear() const -> int
		{
			auto &tm = toTM();
			return tm.tm_year + 1900;
		};

		/**
		 * @brief Get month of the Date
		 *
		 * @return Month as integer (1: January, 12: December)
		 */
		auto getMonth() const -> int
		{
			auto &tm = toTM();
			return tm.tm_mon + 1;
		};

		/**
		 * @brief Get the day in month of the Date
		 *
		 * @return Day in month (1 to 31)
		 */
		auto getDate() const -> int
		{
			auto &tm = toTM();
			return tm.tm_mon + 1;
		};

		/**
		 * @brief Get the day in week of the Date
		 *
		 * @return Day in week to integer (0: Sunday, 1: Monday, ..., 6: Saturday)
		 */
		auto getDay() const -> int
		{
			auto &tm = toTM();
			return tm.tm_wday;
		};

		/**
		 * @brief Get hour
		 *
		 * @return hour of the Date to integer
		 */
		auto getHour() const -> int
		{
			struct tm &tm = toTM();
			return tm.tm_hour;
		};

		/**
		 * @brief Get minute
		 *
		 * @return minute of the Date to integer
		 */
		auto getMinute() const -> int
		{
			struct tm &tm = toTM();
			return tm.tm_min;
		};

		/**
		 * @brief Get second
		 *
		 * @return second of the Date to integer
		 */
		auto getSecond() const -> int
		{
			struct tm &tm = toTM();
			return tm.tm_sec;
		};

		/* ---------------------------------------------------------
			SETTERS
		--------------------------------------------------------- */
		/**
		 * @brief Set year of the Date
		 *
		 * @details
		 * If previous date is leaf month's expiration date and
		 * target year what you want is not leaf, the date will be 28
		 *
		 * \li 2000-02-29 -> setYear(2001) -> 2001-02-28
		 * \li 2001-02-28 -> setYear(2000) -> 2000-02-08
		 *
		 * @param val Target year
		 */
		void setYear(int year)
		{
			int month = getMonth();
			int new_date = getDate();

			//윤달에 29일인데 addYear을 하려는 경우
			if
				(
					month == 2 && new_date == 29 //현재가 윤달 29일
					&&
					!(
						year == 4 &&
						!((year % 100 == 0 && year % 400 != 0))
						) //바꿀 연도의 2월은 윤달이 아님
				)
				new_date = 28;

			set(year, month, new_date);
		};

		/**
		 * @brief Set month of the Date
		 *
		 * @details
		 * If the expiration date of the month will be shrinked, the date will be changed to the expiration date
		 *
		 * \li 2000-03-31 -> setMonth(4) -> 2000-04-30
		 * \li 2007-08-31 -> setMonth(9) -> 2007-09-30
		 *
		 * @param val Target month (1 - 12)
		 * @throw invalid_argument month is out of range
		 */
		void setMonth(int month)
		{
			if (month > 12)
				throw std::invalid_argument("month is over 12");

			//달력
			int year = getYear();
			int new_date = getDate();

			std::array<int, 12> &months = calc_last_dates(year);

			//해당 월의 마지막 일을 초과할 때, 조정한다
			//EX-> 4월인데 31일이면 4월 30일로 조정
			if (new_date > months[month - 1])
				new_date = months[month - 1];

			set(year, month, new_date);
		};

		/**
		 * Set date of the Date
		 *
		 * @param val Target day in month
		 * @throw invalid_argument When date is over expiration date in month
		 */
		void setDate(int val)
		{
			set(getYear(), getMonth(), val);
		};

		/**
		 * @brief Set hour
		 *
		 * @param val Target hour (0 - 23)
		 * @throw invalid_argument When hour is out of range
		 */
		void setHour(int val)
		{
			addHour(val - getHour());
		};

		/**
		 * @brief Set minute
		 *
		 * @param val Target minute (0 - 59)
		 * @throw invalid_argument When minute is out of range
		 */
		void setMinute(int val)
		{
			addMinute(val - getMinute());
		};

		/**
		 * @brief Set hour
		 *
		 * @param val Target second (0 - 59)
		 * @throw invalid_argument When second is out of range
		 */
		void setSecond(int val)
		{
			addSecond(val - getSecond());
		};

		/* --------------------------------------------------------------------------
			ADDERS
		-------------------------------------------------------------------------- */
		//ADD METHODS
		/**
		 * @brief Add years to the Date
		 *
		 * @param val Years to add
		 */
		void addYear(int val)
		{
			//현재가 2012년 2월 29일(윤달)인데 1년을 추가하면 2013년 2월 28일로 변함
			setYear(getYear() + val);
		};

		/**
		 * @brief Add months to the Date
		 *
		 * @details
		 * Not a matter to val is over 12. If the month is over 12,
		 * then years will be added.
		 *
		 * @param val Months to add
		*/
		void addMonth(int val)
		{
			//현재가 3월 31일인데 1월이 추가되면 날짜는 4월 30일로 변한다
			int year = getYear();
			int month = getMonth() + val;
			int date = getDate();

			if (month > 12)
			{
				year = year + (month - 1) / 12;
				month = month % 13;
			}

			//달력
			std::array<int, 12> &months = calc_last_dates(year);

			//해당 월의 마지막 일을 초과할 때, 조정한다
			//EX-> 4월인데 31일이면 4월 30일로 조정
			if (date > months[month - 1])
				date = months[month - 1];

			set(year, month, date);
		};

		/**
		 * @brief Add weeks to the Date
		 *
		 * @details
		 * Not a matter that val is too huge. If the adding weeks derives
		 * modification in month or year, then it will be.
		 *
		 * @param val Weeks to add
		 */
		void addWeek(int val)
		{
			addDate(7 * val);
		};

		/**
		 * @brief Add days to the Date
		 *
		 * @details
		 * Not a matter that val is too huge. If the adding dates derives
		 * modification in month or year, then it will be.
		 *
		 * @param val Days to add
		 */
		void addDate(int val)
		{
			addHour(24 * val);
		};

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
		void addHour(int val)
		{
			operator+=(std::chrono::hours(val));
		};

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
		void addMinute(int val)
		{
			operator+=(std::chrono::minutes(val));
		};

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
		void addSecond(int val)
		{
			operator+=(std::chrono::seconds(val));
		};

		/* --------------------------------------------------------------------------
			EXPORTERS
		-------------------------------------------------------------------------- */
		/**
		 * @brief Converts the Date to struct tm
		 *
		 * @return struct tm representing the Date
		 */
		auto toTM() const -> struct ::tm
		{
			struct tm tm;
			time_t tt = std::chrono::system_clock::to_time_t(*this);
			localtime_s(&tm, &tt);

			return tm;
		};

		/**
		 * @brief Converts the Date to linux_time
		 *
		 * @return linux_time of the Date
		 */
		auto toLinuxTime() const -> long long
		{
			std::chrono::system_clock::duration duration = *this - std::chrono::system_clock::from_time_t(0);
			std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);

			return seconds.count();
		};

		/**
		 * @brief Converts the Date to std::string
		 *
		 * @return std::string expressing the Date
		 */
		auto toString() const -> std::string
		{
			struct tm &tm = toTM();

			if (tm.tm_hour == 0 && tm.tm_min == 0 && tm.tm_sec == 0)
				return StringUtil::substitute
				(
					"{1}-{2}-{3}",
					tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday
				);
			else
				return StringUtil::substitute
				(
					"{1}-{2}-{3} {4}:{5}:{6}",
					tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec
				);
		};
	};
};
};