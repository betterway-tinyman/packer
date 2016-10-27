#pragma once

#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Unique lock for reading
	 *
	 * @details
	 * <p> A UniqueAcquire is an object manages a RWMutex with unique ownership in both states. </p>
	 *	\li locked
	 *	\li unlocked
	 *
	 * <p> On construction (or by move-assigning to it), the object locks a RWMutex object on reading
	 * side, for whose locking and unlocking operations becomes responsible. </p>
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
	 * Though, that the UniqueReadLock object does not manage the lifetime of the RWMutex
	 * object in any way: the duration of the RWMutex object shall extend at least until
	 * the destruction of the UniqueReadLock that manages it.
	 *
	 * @handbook [Library - Critical Section](https://github.com/samchon/framework/wiki/CPP-Library-Critical_Section)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class UniqueReadLock
	{
	private:
		/**
		 * @brief Managed mutex
		 */
		const RWMutex *rw_mutex;

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
		UniqueReadLock(const RWMutex &rw_mutex, bool lock = true)
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
		UniqueReadLock(const UniqueReadLock &) = delete;

		/**
		 * @brief Move Constructor
		 *
		 * @param obj An object to move
		 */
		UniqueReadLock(UniqueReadLock &&obj)
		{
			//MOVE
			this->rw_mutex = obj.rw_mutex;
			this->locked = obj.locked;

			//TRUNCATE
			obj.rw_mutex = nullptr;
			obj.locked = false;
		};

		/**
		 * @brief Default Destructor
		 * @details If read lock has done by the UniqueLock, unlock it
		 */
		~UniqueReadLock()
		{
			if (locked == true)
				rw_mutex->readUnlock();
		}

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc RWMutex::readLock()
		 */
		void lock() const
		{
			if (locked == true)
				return;

			rw_mutex->readLock();
			(bool&)locked = true;
		};

		/**
		 * @copydoc RWMutex::readUnlock()
		 */
		void unlock() const
		{
			if (locked == false)
				return;

			rw_mutex->readUnlock();
			(bool&)locked = false;
		};

		/**
		 * @copydoc RWMutex::tryReadLock()
		 */
		//auto tryLock() const -> bool;
	};
};
};