#pragma once

#include <samchon/library/Semaphore.hpp>

namespace samchon
{
namespace library
{
	/**
	 * @brief Unique acquire from a Semaphore.
	 *
	 * @details
	 * <p> A UniqueAcquire is an object manages a Semaphore with unique ownership in both states. </p>
	 *	\li acquired
	 *	\li released
	 *
	 * <p> On construction (or by move-assigning to it), the object acquires a semaphore object,
	 * for whose acquiring and releasing operations becomes responsible. </p>
	 *
	 * <p> This class guarantees a released status on destruction (even if not called explicitly).
	 * Therefore it is especially useful as an object with automatic duration, as it guarantees
	 * the semaphore object is properly released in case an exception is thrown. </p>
	 *
	 * <p> Referenced comments of std::unique_lock </p>
	 *	\li http://www.cplusplus.com/reference/mutex/unique_lock/
	 *
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * <p> Though, that the UniqueAcquire object does not manage the lifetime of the semaphore
	 * object in any way: the duration of the semaphore object shall extend at least until
	 * the destruction of the UniqueAcquire that manages it. </p>
	 *
	 * <p> The UniqueAcquire is movable, but not copyable. </p>
	 *
	 * @see samchon::library
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class UniqueAcquire
	{
	private:
		/**
		 * @brief Managed semaphore
		 */
		Semaphore *semaphore;

		/**
		 * @brief Whether the semaphore was acquired by the UniqueLock
		 */
		bool locked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from semaphore
		 *
		 * @param semaphore Semaphore to manage
		 * @param lock Whether to lock directly or not
		 */
		UniqueAcquire(Semaphore &semaphore, bool lock = true)
		{
			this->semaphore = &semaphore;
			this->locked = false;

			if (lock == true)
				this->acquire();
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
		UniqueAcquire(const UniqueAcquire &obj) = delete;

		/**
		 * @brief Move Constructor
		 *
		 * @param obj An object to move
		 */
		UniqueAcquire(UniqueAcquire &&obj)
		{
			//MOVE
			this->semaphore = obj.semaphore;
			this->locked = obj.locked;

			//TRUNCATE
			obj.semaphore = nullptr;
			obj.locked = false;
		};

		/**
		 * @brief Default Destructor
		 * @details If read lock has done by the UniqueLock, unlock it
		 */
		~UniqueAcquire()
		{
			if (locked)
				semaphore->release();
		};

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc Semaphore::acquire()
		 */
		void acquire()
		{
			if (locked)
				return;

			semaphore->acquire();
			locked = true;
		};

		/**
		 * @copydoc Semaphore::release()
		 */
		void release()
		{
			if (locked == false)
				return;

			semaphore->release();
			locked = false;
		};

		/**
		 * @copydoc Semaphore::tryAcquire()
		 */
		//auto tryAcquire() -> bool;
	};
};
};