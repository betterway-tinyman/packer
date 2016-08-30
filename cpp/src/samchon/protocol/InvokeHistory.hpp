#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/Entity.hpp>

#include <samchon/library/Date.hpp>
#include <samchon/protocol/Invoke.hpp>

namespace samchon
{
namespace protocol
{
	class InvokeHistory : public Entity
	{
	private:
		typedef Entity super;

	protected:
		size_t uid;

		std::string listener;

		library::Date startTime;

		library::Date endTime;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		InvokeHistory() : super()
		{
		};

		InvokeHistory(std::shared_ptr<Invoke> invoke) : super()
		{
			uid = invoke->get("invoke_history_uid")->getValue<size_t>();
			listener = invoke->getListener();

			startTime = std::chrono::system_clock::now();
		};

		virtual ~InvokeHistory() = default;

		virtual void construct(std::shared_ptr<library::XML> xml) override
		{
			uid = xml->getProperty<size_t>("uid");
			listener = xml->getProperty("listener");

			startTime = std::chrono::system_clock::from_time_t(0);
			endTime = std::chrono::system_clock::from_time_t(0);

			startTime += std::chrono::duration<long long, std::ratio_multiply<std::ratio<100i64, 1i64>, std::nano>>(xml->getProperty<long long>("startTime"));
			endTime += std::chrono::duration<long long, std::ratio_multiply<std::ratio<100i64, 1i64>, std::nano>>(xml->getProperty<long long>("endTime"));
		};

		void notifyEnd()
		{
			endTime = std::chrono::system_clock::now();
		};

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		auto getUID() const -> size_t
		{
			return uid;
		};

		auto getListener() const -> std::string
		{
			return listener;
		};

		auto getStartTime() const -> library::Date
		{
			return startTime;
		};

		auto getEndTime() const -> library::Date
		{
			return endTime;
		};

		auto getElapsedTime() const -> long long
		{
			return (endTime - startTime).count();
		};

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto TAG() const -> std::string
		{
			return "invokeHistory";
		};

		virtual auto toXML() const -> std::shared_ptr<library::XML>
		{
			std::shared_ptr<library::XML> &xml = super::toXML();
			xml->setProperty("uid", uid);
			xml->setProperty("listener", listener);
			xml->setProperty("startTime", startTime.time_since_epoch().count());
			xml->setProperty("endTime", endTime.time_since_epoch().count());

			return xml;
		};

		virtual auto toInvoke() const -> std::shared_ptr<Invoke>
		{
			return std::make_shared<Invoke>("report_invoke_history", this->toXML());
		};
	};
};
};