#pragma once
#include <samchon/API.hpp>

namespace samchon
{
namespace library
{
	class RWMutex;

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
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * Though, that the UniqueReadLock object does not manage the lifetime of the RWMutex
	 * object in any way: the duration of the RWMutex object shall extend at least until
	 * the destruction of the UniqueReadLock that manages it.
	 *
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API UniqueReadLock
	{
	private:
		/**
		 * @brief Managed mutex
		 */
		const RWMutex *mtx;

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
		UniqueReadLock(const RWMutex &, bool = true);

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
		 * @param An object to move
		 */
		UniqueReadLock(UniqueReadLock&&);

		/**
		 * @brief Default Destructor
		 * @details If read lock has done by the UniqueLock, unlock it
		 */
		~UniqueReadLock();

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc RWMutex::readLock()
		 */
		void lock() const;

		/**
		 * @copydoc RWMutex::readUnlock()
		 */
		void unlock() const;

		/**
		 * @copydoc RWMutex::tryReadLock()
		 */
		//auto tryLock() const -> bool;
	};
};
};