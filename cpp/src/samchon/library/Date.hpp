#pragma once
#include <samchon/API.hpp>

#include <samchon/library/IOperator.hpp>

#include <string>
#include <samchon/WeakString.hpp>
#include <chrono>

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
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API Date
		: public std::chrono::system_clock::time_point
	{
	private:
		typedef std::chrono::system_clock::time_point super;

	protected:
		/**
		 * @brief time_point for 1970-01-01 09:00:00
		 * @details It's the zero(0) in linux_time
		 */
		static std::chrono::system_clock::time_point TP_1970;

		/**
		 * @brief Calculates how many seconds have flowen since 0000-01-01 00:00:00
		 * @details This static method is used for calculate the linux_time (since 1970-01-01 09:00:00)
		 *
		 * @return linux_time of the Date
		 */
		static auto calcSeconds(int year, int month, int date) -> long long;

		/**
		* @brief Calculates and gets an array of final date of each month for that year.
		*
		* @param year The target year
		* @return Array of fianl date of each month for that year
		*/
		static auto calcLastDates(int year)->std::array<int, 12>;

	public:
		/* --------------------------------------------------------------------------
			CONSTRUCTORS
		-------------------------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 *
		 * @detail The date will be now (Now's date from operating system)
		 */
		Date();

		/**
		 * @brief Copy Constructor
		 * @details Copy the date from another Date
		 *
		 * @param date Target to be copied
		 */
		Date(const super&);

		/**
		 * @briref Move Constructor
		 * @details Gets the date from other and truncate the other
		 *
		 * @param date Target to be moved
		 */
		Date(super&&);

		/**
		 * @brief Construct from date elements
		 *
		 * @param year ex) 2015
		 * @param month January to December. 1 ~ 12
		 * @param date day in month. 1 ~ 31
		 * @throw invalid_argument month or date is invalid.
		 */
		Date(int year, int month, int date);

		/**
		 * @brief Construct from string
		 *
		 * @param str A string represents the date. (1991-01-01)
		 * @throw invalid_argument month or date is invalid.
		 */
		Date(const std::string &);

		/**
		* @brief Construct from WeakString
		*
		* @param str A weak_string expressing the date. (1991-01-01)
		* @throw invalid_argument month or date is invalid.
		*/
		Date(const WeakString &);

		/**
		 * @brief Construct from linux_time
		 *
		 * @param linuxTime linux_time to be converted
		 */
		Date(long long linuxTime);
		virtual ~Date() = default;

		/**
		 * @brief Setter by string
		 *
		 * @param str Y-m-d
		 * @throw invalid_argument month or date is invalid.
		 */
		void set(const std::string &);

		/**
		 * @brief Setter by weak_string
		 *
		 * @param wstr Y-m-d
		 * @throw invalid_argument month or date is invalid.
		 */
		virtual void set(const WeakString &);

		/**
		 * @brief Setter by elements of date
		 *
		 * @param year ex) 2015
		 * @param month January to December. 1 ~ 12
		 * @param date day in month. 1 ~ 31
		 * @throw invalid_argument month or date is invalid.
		 */
		void set(int year, int month, int date);

		/**
		 * @brief Setter by linux_time
		 *
		 * @param linuxTime linux_time to be converted
		 */
		void set(long long linuxTime);

	public:
		/* --------------------------------------------------------------------------
			SETTERS
		-------------------------------------------------------------------------- */
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
		virtual void setYear(int);

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
		virtual void setMonth(int);
		/**
		 * Set date of the Date
		 *
		 * @param val Target day in month
		 * @throw invalid_argument When date is over expiration date in month
		 */
		virtual void setDate(int);

		//ADD METHODS
		/**
		 * @brief Add years to the Date
		 *
		 * @param val Years to add
		 */
		virtual void addYear(int);

		/**
		 * @brief Add months to the Date
		 *
		 * @details
		 * Not a matter to val is over 12. If the month is over 12,
		 * then years will be added.
		 *
		 * @param val Months to add
		*/
		virtual void addMonth(int);

		/**
		 * @brief Add weeks to the Date
		 *
		 * @details
		 * Not a matter that val is too huge. If the adding weeks derives
		 * modification in month or year, then it will be.
		 *
		 * @param val Weeks to add
		 */
		virtual void addWeek(int);

		/**
		 * @brief Add days to the Date
		 *
		 * @details
		 * Not a matter that val is too huge. If the adding dates derives
		 * modification in month or year, then it will be.
		 *
		 * @param val Days to add
		 */
		virtual void addDate(int);

		/* --------------------------------------------------------------------------
			GETTERS
		-------------------------------------------------------------------------- */
		/**
		 * @brief Get year of the Date
		 *
		 * @return Year
		 */
		auto getYear() const -> int;

		/**
		 * @brief Get month of the Date
		 *
		 * @return Month as integer (1: January, 12: December)
		 */
		auto getMonth() const -> int;

		/**
		 * @brief Get the day in month of the Date
		 *
		 * @return Day in month (1 to 31)
		 */
		auto getDate() const -> int;

		/**
		 * @brief Get the day in week of the Date
		 *
		 * @return Day in week to integer (0: Sunday, 1: Monday, ..., 6: Saturday)
		 */
		auto getDay() const -> int;

		/* --------------------------------------------------------------------------
			EXPORTERS
		-------------------------------------------------------------------------- */
	protected:
		/**
		 * @brief Converts the Date to struct tm
		 *
		 * @return struct tm representing the Date
		 */
		auto toTM() const -> struct ::tm;

	public:
		/**
		 * @brief Converts the Date to linux_time
		 *
		 * @return linux_time of the Date
		 */
		auto toLinuxTime() const -> long long;

		/**
		 * @brief Converts the Date to std::string
		 *
		 * @return std::string expressing the Date
		 */
		virtual auto toString() const->std::string;
	};
};
};