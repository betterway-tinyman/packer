#pragma once
#include <samchon/API.hpp>

#include <queue>
#include <unordered_set>
#include <unordered_map>

#include <memory>
#include <functional>
#include <condition_variable>
#include <mutex>

#include <samchon/library/RWMutex.hpp>
#include <samchon/library/Event.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Abstract class for dispatching Event
	 *
	 * @details
	 * <p> EventDispatcher is the base class for all classes that dispatch events. </p>
	 *
	 * <p> All the events are sent asynchronously. To avoid from creating tooo enourmouse threads
	 * dispatching events, all event sending processes will acuiqre a semaphore. The default permitted
	 * size of the semaphore is 2. </p>
	 * 
	 *	\li Number of thread pools used to sending events is 2.
	 *
	 * <p> @image html  cpp/library_event.png
	 *	   @image latex cpp/library_event.png </p>
	 *
	 * \par Example Source
	 *		@includelineno example/event/main.cpp
	 *
	 * @deprecated 
	 * <p> EventDispatcher is a candidate to be deprecated. </p>
	 * <p> Since C++11, calling member method of a class by a new thread passing by static 
	 * method and using void pointer are recommeded to avoid. As the reason, using <i>std::thread</i> 
	 * and <i>std::bind</i> will be better. </p>
	 *
	 *	\li std::thread: http://www.cplusplus.com/reference/thread/thread/
	 *	\li std::bind: http://www.cplusplus.com/reference/functional/bind/
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>, Jun Ryung Ju <https://github.com/ArtBlnd>
	 */
	class EventDispatcher
	{
	public:
		typedef void(*Listener)(std::shared_ptr<Event>, void*);
		// typedef std::function<void(std::shared_ptr<Event>, void*)> Listener;

	private:
		/**
		 * @brief A container storing listeners
		 */
		std::unordered_map<int, std::unordered_map<Listener, std::unordered_set<void*>>> listeners;

		/**
		 * @brief A rw_mutex for concurrency
		 */
		RWMutex mtx;

	public:
		/* ----------------------------------------------------------
			CONSTRUCTORS
		---------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		EventDispatcher()
		{
		};

		/**
		 * @brief Copy Constructor
		 *
		 * @details
		 * Copying an EventDispatcher instance does not copy the event listeners attached to it.
		 * (If your newly created node needs an event listener, you must attach the listener after creating the node.)
		 *
		 * @param eventDispatcher The object to copy
		 */
		EventDispatcher(const EventDispatcher &)
		{
			// DO NOT COPY LISTENERS
		};

		/**
		 * @brief Move Constructor
		 *
		 * @param eventDispatcher The object to move
		 */
		EventDispatcher(EventDispatcher &&obj)
		{
			UniqueWriteLock obj_uk(obj.mtx);
			{
				listeners = move(obj.listeners);
			}
			obj_uk.unlock();

			std::unique_lock<std::mutex> s_uk(sMtx());

			for (auto it = eventMap().begin(); it != eventMap().end(); it++)
				if (it->first == &obj)
				{
					auto event = it->second;

					it = eventMap().erase(it);
					eventMap().insert(it, { this, event });
				}
		};

		/**
		 * @brief Default Destructor
		 */
		virtual ~EventDispatcher()
		{
			UniqueWriteLock my_uk(mtx);
			std::unique_lock<std::mutex> s_uk(sMtx());

			for (auto it = eventMap().begin(); it != eventMap().end();)
				if (it->first == this)
					eventMap().erase(it++);
				else
					it++;
		};

		/* ----------------------------------------------------------
			ADD-REMOVE EVENT LISTENERS
		---------------------------------------------------------- */
		/**
		 * @brief Register an event listener
		 *
		 * @details
		 * Registers an event listener object with an EventDispatcher object
		 * so that the listener receives notification of an event.
		 *
		 * @warning Copying an EventDispatcher instance does not copy the event listeners attached to it.
		 *			(If your newly created node needs an event listener, you must attach the listener after creating the node.)
		 *			However, if you move an EventDispatcher instance, the event listeners attached to it move along with it.
		 *
		 * @warning If you no longer need an event listener, remove it by calling removeEventListener,
		 *			or EventDispatcher already try to send events to the no longer needed listener and
		 *			it can cause some confliction.
		 *
		 * @param type The type of event.
		 * @param listener The listener function processes the event.
		 * @param addiction Something to be addicted following the listener.
		 */
		void addEventListener(int type, Listener listener, void *addiction = nullptr)
		{
			UniqueWriteLock uk(mtx);

			listeners[type][listener].insert(addiction);
		};

		/**
		 * @brief Remove a registered event listener
		 *
		 * @details
		 * Removes a listener from the EventDispatcher object.
		 * If there is no matching listener registered with the EventDispatcher object, a call to this method has no effect
		 *
		 * @param type The type of event.
		 * @param listener The listener function to remove.
		 * @param addiction Somethhing to be addicted following the listener.
		 */
		void removeEventListener(int type, Listener listener, void *addiction = nullptr)
		{
			UniqueWriteLock uk(mtx);
			if (listeners.count(type) == 0)
				return;

			// TEST WHETHER HAS THE LISTENER
			if (listeners.count(type) == 0 ||
				listeners[type].count(listener) == 0 ||
				listeners[type][listener].count(addiction) == 0)
				return;

			listeners[type][listener].erase(addiction);

			if (listeners[type][listener].empty() == true)
				listeners[type].erase(listener);

			if (listeners[type].empty() == true)
				listeners.erase(type);

			// NEED TO DELETE FROM EVENT MAP
		};

		/**
		 * @brief Dispatches an event to all listeners
		 *
		 * @details
		 * <p> Dispatches an event into the event flow in the background.
		 * The Event::source is the EventDispatcher object upon which the dispatchEvent. </p>
		 *
		 * @param event The Event object that is dispatched into the event flow.
		 * @return Whether there's some listener to listen the event
		 */
		void dispatch(std::shared_ptr<Event> event)
		{
			// STARTS BACK-GROUND PROCESS IF NOT STARTED
			start();

			UniqueReadLock my_uk(mtx);
			if (listeners.count(event->getType()) == 0)
				return;

			my_uk.unlock();

			std::unique_lock<std::mutex> s_uk(sMtx());
			eventMap().insert({ this, event });

			cv().notify_all();
		};

	private:
		void deliver(std::shared_ptr<Event> event)
		{
			UniqueReadLock my_uk(mtx);
			if (listeners.count(event->getType()) == 0)
				return;

			auto listenerMap = listeners[event->getType()];
			my_uk.unlock();

			for (auto it = listenerMap.begin(); it != listenerMap.end(); it++)
			{
				Listener listener = it->first;

				for (auto s_it = it->second.begin(); s_it != it->second.end(); s_it++)
				{
					void *addiction = *s_it;

					listener(event, addiction);
				}
			}
		};

		/* ----------------------------------------------------------
			MEMBERS OF STATIC
		---------------------------------------------------------- */
		static bool& started()
		{
			static bool flag = false;
			return flag;
		};
		static std::condition_variable& cv()
		{
			static std::condition_variable obj;
			return obj;
		};
		static std::mutex& cv_mtx()
		{
			static std::mutex obj;
			return obj;
		};

		static std::unordered_multimap<EventDispatcher*, std::shared_ptr<Event>>& eventMap()
		{
			static std::unordered_multimap<EventDispatcher*, std::shared_ptr<Event>> map;
			return map;
		};

		static std::mutex& sMtx()
		{
			static std::mutex obj;
			return obj;
		};

		static void start()
		{
			std::unique_lock<std::mutex> uk(sMtx());
			if (started() == true)
				return;

			started() = true;
			uk.unlock();

			for (size_t i = 0; i < THREAD_SIZE(); i++)
				std::thread([]()
				{
					while (true)
					{
						while (true)
						{
							std::unique_lock<std::mutex> uk(sMtx());
							if (eventMap().empty() == true)
								break;

							auto pair = *eventMap().begin();
							eventMap().erase(eventMap().begin());

							uk.unlock();

							EventDispatcher *obj = pair.first;
							std::shared_ptr<Event> &event = pair.second;

							obj->deliver(event);
						}

						std::unique_lock<std::mutex> cv_uk(cv_mtx());
						cv().wait(cv_uk);
					}
				}).detach();
		};

	public:
		/**
		 * @brief Numer of threads for background.
		 */
		static size_t& THREAD_SIZE() 
		{
			static size_t val = 2;
			return val;
		};
	};
};
};