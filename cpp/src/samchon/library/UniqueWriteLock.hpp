#pragma once

#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Unique lock for writing
	 *
	 * @details
	 * <p> A UniqueWriteLock is an object manages a RWMutex with unique ownership in both states. </p>
	 *	\li acquired
	 *	\li released
	 *
	 * <p> On construction (or by move-assigning to it), the object locks a RWMutex object on writing side,
	 * for whose locking and unlocking operations becomes responsible. </p>
	 *
	 * <p> The object supports both states: locked and unlocked. </p>
	 *
	 * <p> This class guarantees a unlocked status on destruction (even if not called explicitly).
	 * Therefore it is especially useful as an object with automatic duration, as it guarantees
	 * the RWMutex object is properly unlocked in case an exception is thrown. </p>
	 *
	 * <p> Referenced comments of std::unique_lock </p>
	 *	\li http://www.cplusplus.com/reference/mutex/unique_lock/
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_critical_section.png)
	 *
	 * @note
	 * Though, that the UniqueWriteLock object does not manage the lifetime of the RWMutex
	 * object in any way: the duration of the RWMutex object shall extend at least until
	 * the destruction of the UniqueWriteLock that manages it.
	 *
	 * @handbook [Library - Critical Section](https://github.com/samchon/framework/wiki/CPP-Library-Critical_Section)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class UniqueWriteLock
	{
	private:
		/**
		 * @brief Managed mutex
		 */
		RWMutex *rw_mutex;

		/**
		 * @brief Whether the mutex was locked by UniqueLock
		 */
		bool locked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from mutex
		 *
		 * @param rw_mutex Mutex to manage
		 * @param lock Whether to lock directly or not
		 */
		UniqueWriteLock(RWMutex &rw_mutex, bool lock = true)
		{
			this->rw_mutex = &rw_mutex;
			this->locked = false;

			if (lock == true)
				this->lock();
		};

		/**
		 * @brief Prohibited Copy Constructor
		 *
		 * @details
		 * <p> UniqueAcquire can't be copied. Use pointer, reference instead. </p>
		 * <p> If what you want is UniqueAcquire(s) references each other and unlock when all related
		 * UniqueAcquire objects are destructed, SharedAcquire is the best way. </p>
		 *
		 * @param obj Tried object to copy.
		 */
		UniqueWriteLock(const UniqueWriteLock &) = delete;

		/**
		 * @brief Move Constructor
		 *
		 * @param obj An object to move
		 */
		UniqueWriteLock(UniqueWriteLock &&obj)
		{
			//MOVE
			this->rw_mutex = obj.rw_mutex;
			this->locked = obj.locked;

			//TRUNCATE
			obj.rw_mutex = nullptr;
			obj.locked = false;
		};

		/**
		 * @brief Destructor
		 *
		 * @details
		 * If write lock has done by the UniqueLock, unlock it
		 */
		~UniqueWriteLock()
		{
			if (locked == true)
				rw_mutex->writeUnlock();
		};

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc RWMutex::writeLock()
		 */
		void lock()
		{
			if (locked == true)
				return;

			rw_mutex->writeLock();
			locked = true;
		};

		/**
		 * @copydoc RWMutex::writeUnlock()
		 */
		void unlock()
		{
			if (locked == false)
				return;

			rw_mutex->writeUnlock();
			locked = false;
		};

		/**
		 * @copydoc RWMutex::tryLock()
		 */
		//auto tryLock() -> bool;
	};
};
};