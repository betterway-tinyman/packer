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

namespace samchon
{
namespace library
{
	class Event;

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
	class SAMCHON_FRAMEWORK_API EventDispatcher
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
		EventDispatcher();

		/**
		 * @brief Copy Constructor
		 *
		 * @details
		 * Copying an EventDispatcher instance does not copy the event listeners attached to it.
		 * (If your newly created node needs an event listener, you must attach the listener after creating the node.)
		 *
		 * @param eventDispatcher The object to copy
		 */
		EventDispatcher(const EventDispatcher &);

		/**
		 * @brief Move Constructor
		 *
		 * @param eventDispatcher The object to move
		 */
		EventDispatcher(EventDispatcher &&);

		/**
		 * @brief Default Destructor
		 */
		virtual ~EventDispatcher();

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
		void addEventListener(int, Listener, void* = nullptr);

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
		void removeEventListener(int, Listener, void* = nullptr);

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
		void dispatch(std::shared_ptr<Event>);

	private:
		void deliver(std::shared_ptr<Event>);

		/* ----------------------------------------------------------
			MEMBERS OF STATIC
		---------------------------------------------------------- */
		static bool started;
		static std::condition_variable cv;
		static std::mutex cv_mtx;

		static std::unordered_multimap<EventDispatcher*, std::shared_ptr<Event>> eventMap;
		static std::mutex sMtx;

		static void start();

	public:
		/**
		 * @brief Numer of threads for background.
		 */
		static size_t THREAD_SIZE;
	};
};
};