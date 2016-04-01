#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <samchon/library/Datetime.hpp>

namespace samchon
{
namespace protocol
{
	class Invoke;

	/**
	 * @brief A history of an Invoke message
	 *
	 * @details
	 * <p> InvokeHistory is a class for reporting history log of an Invoke message with elapsed time 
	 * from a slave to its master.</p>
	 *
	 * <p> With the elapsed time, consumed time for a process of handling the Invoke message, 
	 * InvokeHistory is reported to the master. The master utilizies the elapsed time to estimating
	 * performances of each slave system. With the estimated performan index, master retrives the
	 * optimal solution of distributing processes. </p>
	 *
	 * @image html  cpp/protocol_invoke.png
	 * @image latex cpp/protocol_invoke.png
	 *
	 * @see samchon::protocol
	 * @see samchon::protocol::master
	 * @see samchon::protocol::slave
	 *
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API InvokeHistory
		: public virtual Entity
	{
	protected:
		typedef Entity super;

		/**
		 * @brief An identifier.
		 */
		size_t uid;

		/**
		 * @brief A listener of the Invoke message.
		 *
		 * @details
		 * <p> InvokeHistory does not archive entire data of an Invoke message. InvokeHistory only
		 * archives its listener. The first, formal reason is to save space, avoid wasting spaces. </p>
		 *
		 * <p> The second, complicate reason is on an aspect of which systems are using the
		 * InvokeHistory class. InvokeHistory is designed to let slave reports to master elapsed time
		 * of a process used to handling the Invoke message. If you want to archive entire history log
		 * of Invoke messages, then the subject should be master, not the slave using InvokeHistory
		 * classes. </p>
		 */
		std::string listener;

		/**
		 * @brief Start time of the history
		 *
		 * @details
		 * <p> Means start time of a process handling the Invoke message. The start time not only
		 * has ordinary arguments represented Datetime (year to seconds), but also has very precise
		 * values under seconds, which is expressed as nano seconds (10^-9). </p>
		 *
		 * <p> The precise start time will be used to calculate elapsed time with end time. </p>
		 */
		library::Datetime startTime;

		/**
		 * @brief End time of the history
		 *
		 * @details
		 * <p> Means end time of a process handling the Invoke message. The end time not only
		 * has ordinary arguments represented Datetime (year to seconds), but also has very precise
		 * values under seconds, which is expressed as nano seconds (10^-9). </p>
		 *
		 * <p> The precise end time will be used to calculate elapsed time with start time. </p>
		 */
		library::Datetime endTime;

	public:
		/* -----------------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------------- */
		/**
		 * @brief Default Constructor.
		 */
		InvokeHistory();

		/**
		 * @brief Construct from an Invoke message.
		 *
		 * @details
		 * <p> InvokeHistory does not archive entire Invoke message, only archives its listener. </p>
		 *
		 * @param invoke A message to archive its history log
		 */
		InvokeHistory(std::shared_ptr<Invoke>);
		virtual ~InvokeHistory() = default;

		virtual void construct(std::shared_ptr<library::XML>) override;

		/**
		 * @brief Notify end of the process.
		 *
		 * @details
		 * <p> Notifies end of a process handling the matched Invoke message to InvokeHistory. </p>
		 * <p> InvokeHistory archives the end datetime and calculates elapsed time as nanoseconds. </p>
		 */
		virtual void notifyEnd();

		/* -----------------------------------------------------------------
			GETTERS
		----------------------------------------------------------------- */
	public:
		virtual auto key() const->std::string override;

		/**
		 * @brief Get uid.
		 */
		auto getUID() const->size_t;

		/**
		 * @brief Get listener.
		 */
		auto getListener() const->std::string;

		/**
		 * @brief Get start time.
		 */
		auto getStartTime() const->library::Datetime;

		/**
		 * @brief Get end time.
		 */
		auto getEndTime() const->library::Datetime;

		/**
		 * @brief Get elapsed time.
		 *
		 * @details
		 * <p> Calculates elapsed time frrom start, end time and Returns it. </p>
		 *
		 * <p> The elapsed time will be reported to a master and the master will utilize the elapsed
		 * time to estimating performance of the slave system, the InvokeHistory is belonged to. </p>
		 *
		 * @return An elapsed time expressed as nano seconds.
		 */
		auto calcElapsedTime() const -> long long;

		/* -----------------------------------------------------------------
			EXPORTERS
		----------------------------------------------------------------- */
	public:
		virtual auto TAG() const->std::string override;

		virtual auto toXML() const->std::shared_ptr<library::XML> override;

		/**
		 * @brief Get an Invoke message
		 *
		 * @details
		 * <p> Returns an Invoke message to report to a master that how much time was elapsed on a
		 * process handling the Invoke message. In master, those reports are used to estimate
		 * performance of each slave system. </p>
		 *
		 * @return An Invoke message to report master.
		 */
		auto toInvoke() const->std::shared_ptr<Invoke>;
	};
};
};