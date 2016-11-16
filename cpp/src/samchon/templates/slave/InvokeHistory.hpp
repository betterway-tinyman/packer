#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <samchon/library/Date.hpp>
#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
namespace templates
{
namespace slave
{
	/**
	 * History of an {@link Invoke} message.
	 * 
	 * The {@link InvokeHistory} is a class archiving history log of an {@link Invoke} message with elapsed time. This 
	 * {@link InvokeHistory} class is used to report elapsed time of handling a requested process from **slave** to 
	 * **master** system. 
	 * 
	 * The **master** system utilizes derived {@link InvokeHistory} objects to compute performance indices.
	 * - {@link ParallelSytem.getPerformance}
	 * - {@link DistributedProcess.getResource}
	 * 
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class InvokeHistory 
		: public protocol::Entity<size_t>
	{
	private:
		typedef protocol::Entity<size_t> super;

	private:
		size_t uid_;

		std::string listener_;
		library::Date start_time_;
		library::Date end_time_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		InvokeHistory() : super()
		{
		};

		/**
		 * Construct from an {@link Invoke} message.
		 * 
		 * @param invoke An {@link Invoke} message requesting a *parallel or distributed process*.
		 */
		InvokeHistory(std::shared_ptr<protocol::Invoke> invoke)
			: super()
		{
			uid_ = invoke->get("_History_uid")->getValue<size_t>();
			listener_ = invoke->getListener();

			start_time_ = std::chrono::system_clock::now();
		};

		virtual ~InvokeHistory() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			// UID AND LISTENER
			uid_ = xml->getProperty<size_t>("uid");
			listener_ = xml->getProperty("listener");

			//--------
			// START AND END TIME
			//--------
			// INIT TIMES TO DEFAULT (0; 1970-01-01 09:00:00
			start_time_ = std::chrono::system_clock::from_time_t(0);
			end_time_ = std::chrono::system_clock::from_time_t(0);

			// ADD NUMBERS WHO REPRESENT LINUX_TIME
			start_time_ += std::chrono::duration<long long, std::ratio_multiply<std::ratio<100i64, 1i64>, std::nano>>(xml->getProperty<long long>("startTime"));
			end_time_ += std::chrono::duration<long long, std::ratio_multiply<std::ratio<100i64, 1i64>, std::nano>>(xml->getProperty<long long>("endTime"));
		};

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		virtual auto key() const -> size_t override
		{
			return uid_;
		};

		/**
		 * Get end time.
		 */
		auto getUID() const -> size_t
		{
			return uid_;
		};

		/**
		 * Get {@link Invoke.getListener listener} of the {@link Invoke} message.
		 */
		auto getListener() const -> std::string
		{
			return listener_;
		};

		auto getStartTime() const -> library::Date
		{
			return start_time_;
		};
		auto getEndTime() const -> library::Date
		{
			return end_time_;
		};

		void setStartTime(const library::Date &val)
		{
			start_time_ = val;
		};
		void setEndTime(const library::Date &val)
		{
			end_time_ = val;
		};

		/**
		 * Compute elapsed time.
		 * 
		 * @return nanoseconds.
		 */
		auto computeElapsedTime() const -> long long
		{
			return (end_time_ - start_time_).count();
		};

		/**
		 * Complete the history.
		 * 
		 * Completes the history and determines the {@link getEndTime end time}.
		 */
		void complete()
		{
			end_time_ = std::chrono::system_clock::now();
		};

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string
		{
			return "history";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML> override
		{
			std::shared_ptr<library::XML> &xml = super::toXML();
			xml->setProperty("uid", uid_);
			xml->setProperty("listener", listener_);

			xml->setProperty("startTime", start_time_.time_since_epoch().count());
			xml->setProperty("endTime", end_time_.time_since_epoch().count());

			return xml;
		};

		auto toInvoke() const -> std::shared_ptr<protocol::Invoke>
		{
			return std::make_shared<protocol::Invoke>("_Report_history", toXML());
		};
	};
};
};
};