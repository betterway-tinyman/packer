#pragma once

#include <atomic>
#include <samchon/library/Semaphore.hpp>

namespace samchon
{
namespace library
{
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
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_critical_section.png)
	 *
	 * @note
	 * <p> Though, that the SharedAcquire object does not manage the lifetime of the Semaphore
	 * object in any way: the duration of the Semaphore object shall extend at least until
	 * the destruction of the SharedAcquire that manages it. </p>
	 *
	 * @handbook [Library - Critical Section](https://github.com/samchon/framework/wiki/CPP-Library-Critical_Section)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SharedAcquire
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
		std::atomic<bool> *locked;

	public:
		/* -----------------------------------------------------------
			CONSTRUCTORS
		----------------------------------------------------------- */
		/**
		 * @brief Construct from Semaphore
		 *
		 * @param semaphore Semaphore to manage
		 * @param lock Whether to lock directly or not
		 */
		SharedAcquire(Semaphore &semaphore, bool lock = true)
		{
			this->semaphore = &semaphore;
			this->reference = new std::atomic<size_t>(0);
			this->locked = new std::atomic<bool>(false);

			if (lock == true)
				this->acquire();
		};

		/**
		 * @brief Copy Constructor
		 */
		SharedAcquire(const SharedAcquire &obj)
		{
			obj.reference->operator++();

			this->semaphore = obj.semaphore;
			this->reference = obj.reference;
			this->locked = obj.locked;
		};

		/**
		 * @brief Move Constructor
		 */
		SharedAcquire(SharedAcquire &&obj)
		{
			//MOVE
			this->semaphore = obj.semaphore;
			this->reference = obj.reference;
			this->locked = obj.locked;

			//TRUNCATE
			obj.semaphore = nullptr;
			obj.reference = nullptr;
			obj.locked = nullptr;
		};

		/**
		 * @brief Default Destructor
		 */
		~SharedAcquire()
		{
			if (reference == nullptr || reference->operator--() > 0)
				return;

			if (locked->load() == true)
				semaphore->release();

			delete reference;
			delete locked;
		};

		/* -----------------------------------------------------------
			LOCKERS
		----------------------------------------------------------- */
		/**
		 * @copydoc Semaphore::acquire()
		 */
		void acquire()
		{
			if (locked->load() == true)
				return;

			semaphore->acquire();
			locked->store(true);
		};

		/**
		 * @copydoc Semaphore::release()
		 */
		void release()
		{
			if (locked->load() == false)
				return;

			semaphore->release();
			locked->store(false);
		};

		/**
		 * @copydoc Semaphore::tryAcquire()
		 */
		 //auto tryAcquire() -> bool;
	};
};
};