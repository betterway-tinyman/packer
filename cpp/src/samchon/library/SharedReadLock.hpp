#pragma once
#include <samchon/API.hpp>

namespace std
{
	template <typename _Ty>
	struct atomic;
};
namespace samchon
{
namespace library
{
	class RWMutex;

	/**
	 * @brief Shared lock from a RWMutex.
	 *
	 * @details
	 * <p> A SharedReadLock is an object manages a RWMutex with shared ownership in both states. </p>
	 *	\li locked
	 *	\li unlocked
	 *
	 * <p> On default construction, the object acquires a RWMutex object, for whose locking and 
	 * unlocking operations becomes responsible. When copy constructions, responsibilities of
	 * locking and unlocking RWMutex are shared with copied with those SharedReadLock objects. </p>
	 *
	 * <p> The class shared_lock is a general-purpose shared mutex ownership wrapper allowing deferred 
	 * locking, timed locking and transfer of lock ownership. Locking a shared_lock locks the associated 
	 * shared mutex in shared mode (to lock it in exclusive mode, std::unique_lock can be used) </p>
	 *
	 * <p> This class guarantees a unlocked status on destruction of all shared objects (even if not 
	 * called explicitly). Therefore it is especially useful as an object with automatic duration, 
	 * as it guarantees the RWMutex object is properly unlocked in case an exception is thrown. </p>
	 *
	 * <p> Referenced comments of std::unique_lock </p>
	 *	\li http://www.cplusplus.com/reference/mutex/unique_lock/
	 *
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * <p> Though, that the SharedReadLock object does not manage the lifetime of the RWMutex
	 * object in any way: the duration of the RWMutex object shall extend at least until
	 * the destruction of the SharedReadLock that manages it. </p>
	 *
	 * @see library::UniqueAcquire
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API SharedReadLock
	{
	private:
		/**
		 * @brief Managed RWMutex
		 */
		const RWMutex *semaphore;

		/**
		 * @brief Referencing count sharing same RWMutex.
		 */
		std::atomic<size_t> *reference;

		/**
		 * @brief Whether the mutex was locked by SharedReadLock
		 */
		std::atomic<bool> *isLocked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from RWMutex
		 *
		 * @param semaphore RWMutex to manage
		 * @param doLock Whether to lock directly or not
		 */
		SharedReadLock(const RWMutex &, bool = true);

		/**
		 * @brief Copy Constructor
		 */
		SharedReadLock(const SharedReadLock &);

		/**
		 * @brief Move Constructor
		 */
		SharedReadLock(SharedReadLock&&);

		/**
		 * @brief Default Destructor
		 */
		~SharedReadLock();

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
		 * @copydoc RWMutex::tryAcquire()
		 */
		//auto tryLock() const -> bool;
	};
};
};