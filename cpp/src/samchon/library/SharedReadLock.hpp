#pragma once

#include <atomic>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace library
{
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
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SharedReadLock
	{
	private:
		/**
		 * @brief Managed RWMutex
		 */
		const RWMutex *rw_mutex;

		/**
		 * @brief Referencing count sharing same RWMutex.
		 */
		std::atomic<size_t> *reference;

		/**
		 * @brief Whether the mutex was locked by SharedReadLock
		 */
		std::atomic<bool> *locked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from RWMutex
		 *
		 * @param rw_mutex RWMutex to manage
		 * @param lock Whether to lock directly or not
		 */
		SharedReadLock(const RWMutex &rw_mutex, bool lock = true)
		{
			this->rw_mutex = &rw_mutex;
			this->reference = new std::atomic<size_t>(0);
			this->locked = new std::atomic<bool>(false);

			if (lock == true)
				rw_mutex.readLock();
		};

		/**
		 * @brief Copy Constructor
		 */
		SharedReadLock(const SharedReadLock &obj)
		{
			obj.reference->operator++();

			this->rw_mutex = obj.rw_mutex;
			this->reference = obj.reference;
			this->locked = obj.locked;
		};

		/**
		 * @brief Move Constructor
		 */
		SharedReadLock(SharedReadLock&&obj)
		{
			//MOVE
			this->rw_mutex = obj.rw_mutex;
			this->reference = obj.reference;
			this->locked = obj.locked;

			//TRUNCATE
			obj.rw_mutex = nullptr;
			obj.reference = nullptr;
			obj.locked = nullptr;
		};

		/**
		 * @brief Default Destructor
		 */
		~SharedReadLock()
		{
			if (reference == nullptr || reference->operator--() > 0)
				return;

			if (locked->load() == true)
				rw_mutex->readUnlock();

			delete reference;
			delete locked;
		};

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc RWMutex::readLock()
		 */
		void lock() const
		{
			if (locked->load() == true)
				return;

			rw_mutex->readLock();
			locked->store(true);
		};

		/**
		 * @copydoc RWMutex::readUnlock()
		 */
		void unlock() const
		{
			if (locked->load() == false)
				return;

			rw_mutex->readUnlock();
			locked->store(false);
		};

		/**
		 * @copydoc RWMutex::tryAcquire()
		 */
		//auto tryLock() const -> bool;
	};
};
};