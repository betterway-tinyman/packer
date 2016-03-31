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
	class Semaphore;
		
	/**
	 * @brief Shared acquire from a Semaphore.
	 *
	 * @details
	 * <p> A SharedAcquire is an object manages a Semaphore with shared ownership in both states. </p>
	 *	\li acquired
	 *	\li released
	 *
	 * <p> On default construction, the object acquires a Semaphore object, for whose acquiring and 
	 * releasing operations becomes responsible. When copy constructions, responsibilities of
	 * acquiring and releasing Semaphore are shared with copied with those SharedAcquire objects. </p>
	 *
	 * <p> The class shared_lock is a general-purpose shared mutex ownership wrapper allowing deferred 
	 * locking, timed locking and transfer of lock ownership. Locking a shared_lock locks the associated 
	 * shared mutex in shared mode (to lock it in exclusive mode, std::unique_lock can be used) </p>
	 *
	 * <p> This class guarantees a released status on destruction of all shared objects (even if not 
	 * called explicitly). Therefore it is especially useful as an object with automatic duration, 
	 * as it guarantees the Semaphore object is properly released in case an exception is thrown. </p>
	 *
	 * <p> Referenced comments of std::unique_lock </p>
	 *	\li http://www.cplusplus.com/reference/mutex/unique_lock/
	 *
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * <p> Though, that the SharedAcquire object does not manage the lifetime of the Semaphore
	 * object in any way: the duration of the Semaphore object shall extend at least until
	 * the destruction of the SharedAcquire that manages it. </p>
	 *
	 * @see library::UniqueAcquire
	 * @see samchon::library
	 * 
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API SharedAcquire
	{
	private:
		/**
		 * @brief Managed Semaphore
		 */
		Semaphore *semaphore;

		/**
		 * @brief Referencing count sharing same Semaphore.
		 */
		std::atomic<size_t> *reference;

		/**
		 * @brief Whether the mutex was locked by SharedAcquire
		 */
		std::atomic<bool> *isLocked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from Semaphore
		 *
		 * @param semaphore Semaphore to manage
		 * @param doLock Whether to lock directly or not
		 */
		SharedAcquire(Semaphore &, bool = true);

		/**
		 * @brief Copy Constructor
		 */
		SharedAcquire(const SharedAcquire &);

		/**
		 * @brief Move Constructor
		 */
		SharedAcquire(SharedAcquire&&);

		/**
		 * @brief Default Destructor
		 */
		~SharedAcquire();

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