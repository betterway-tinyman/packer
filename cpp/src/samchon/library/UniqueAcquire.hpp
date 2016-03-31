#pragma once
#include <samchon/API.hpp>

namespace samchon
{
namespace library
{
	class Semaphore;

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
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API UniqueAcquire
	{
	private:
		/**
		 * @brief Managed semaphore
		 */
		Semaphore *semaphore;

		/**
		 * @brief Whether the semaphore was acquired by the UniqueLock
		 */
		bool isLocked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from semaphore
		 *
		 * @param semaphore Semaphore to manage
		 * @param doLock Whether to lock directly or not
		 */
		UniqueAcquire(Semaphore &, bool = true);

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
		UniqueAcquire(const UniqueAcquire &) = delete;

		/**
		 * @brief Move Constructor
		 *
		 * @param An object to move
		 */
		UniqueAcquire(UniqueAcquire&&);

		/**
		 * @brief Default Destructor
		 * @details If read lock has done by the UniqueLock, unlock it
		 */
		~UniqueAcquire();

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc Semaphore::acquire()
		 */
		void acquire();

		/**
		 * @copydoc Semaphore::release()
		 */
		void release();

		/**
		 * @copydoc Semaphore::tryAcquire()
		 */
		//auto tryAcquire() -> bool;
	};
};
};