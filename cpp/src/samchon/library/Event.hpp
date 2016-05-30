#pragma once

namespace samchon
{
namespace library
{
	class EventDispatcher;

	/**
	 * @brief Represent an event running on background.
	 *
	 * @details
	 * <p> The Event class is used as the base class for the creation of Event objects, 
	 * which are passed as parameters to event listeners when an event occurs. </p>
	 *
	 * <p> The properties of the Event class carry basic information about an event, 
	 * such as the event's type or source (who made the event) of the event. </p>
	 *
	 * <p> For many events, such as the events represented by the Event class constants, 
	 * this basic information is sufficient. Other events, however, may require more detailed 
	 * information. </p>
	 *
	 * <p> @image html  cpp/library_event.png
	 *	   @image latex cpp/library_event.png </p>
	 *
	 * \par Example Source
	 *		@includelineno example/event/main.cpp
	 *
	 * @deprecated 
	 * <p> Event is a candidate to be deprecated. </p>
	 * <p> Since C++11, calling member method of a class by a new thread passing by static 
	 * method and using void pointer are recommeded to avoid. As the reason, using <i>std::thread</i> 
	 * and <i>std::bind will</i> be better. </p>
	 *
	 *	\li std::thread: http://www.cplusplus.com/reference/thread/thread/
	 *	\li std::bind: http://www.cplusplus.com/reference/functional/bind/
	 * 
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Event
	{
	public:
		enum : int
		{
			ACTIVATE = 1,
			COMPLETE = 2,
			REMOVED = -1
		};

	protected:
		/**
		 * @brief Source of the event
		 */
		EventDispatcher *source;

		/**
		 * @brief Type of the event
		 */
		int type;

	public:
		/**
		 * @brief Construct from source and type
		 * @details The event object owns its source and type
		 *
		 * @param source Source of the event; who made the event
		 * @param type Type of the event
		 */
		Event(EventDispatcher *source, int type)
		{
			this->source = source;
			this->type = type;
		};
		virtual ~Event() = default;

		/**
		 * @brief Get source of the Event
		 */
		auto getSource() const -> EventDispatcher*
		{
			return source;
		};

		/**
		 * @brief Get type of the Event
		 */
		auto getType() const -> int
		{
			return type;
		};
	};
};
};