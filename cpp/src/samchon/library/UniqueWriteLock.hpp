#pragma once
#include <samchon/API.hpp>

namespace samchon
{
namespace library
{
	class RWMutex;

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
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * Though, that the UniqueWriteLock object does not manage the lifetime of the RWMutex
	 * object in any way: the duration of the RWMutex object shall extend at least until
	 * the destruction of the UniqueWriteLock that manages it.
	 *
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API UniqueWriteLock
	{
	private:
		/**
		 * @brief Managed mutex
		 */
		RWMutex *mtx;

		/**
		 * @brief Whether the mutex was locked by UniqueLock
		 */
		bool isLocked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from mutex
		 *
		 * @param mtx Mutex to manage
		 * @param doLock Whether to lock directly or not
		 */
		UniqueWriteLock(RWMutex &, bool = true);

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
		 * @param An object to move
		 */
		UniqueWriteLock(UniqueWriteLock&&);

		/**
		 * @brief Destructor
		 *
		 * @details
		 * If write lock has done by the UniqueLock, unlock it
		 */
		~UniqueWriteLock();

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc RWMutex::writeLock()
		 */
		void lock();

		/**
		 * @copydoc RWMutex::writeUnlock()
		 */
		void unlock();

		/**
		 * @copydoc RWMutex::tryLock()
		 */
		//auto tryLock() -> bool;
	};
};
};