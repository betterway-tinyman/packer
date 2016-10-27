#pragma once

#include <atomic>
#include <mutex>

namespace samchon
{
namespace library
{
	/**
	 * @brief A semaphore
	 *
	 * @details
	 * <p> In computer science, particularly in operating systems, a semaphore is a variable or abstract
	 * data type that is used for controlling access, by multiple processes, to a common resource in a
	 * concurrent system such as a multiprogramming operating system. </p>
	 *
	 * <p> A trivial semaphore is a plain variable that is changed (for example, incremented or
	 * decremented, or toggled) depending on programmer-defined conditions. The variable is then
	 * used as a condition to control access to some system resource. </p>
	 *
	 * <p> A useful way to think of a semaphore as used in the real-world systems is as a record of
	 * how many units of a particular resource are available, coupled with operations to safely
	 * (i.e., without race conditions) adjust that record as units are required or become free, and,
	 * if necessary, wait until a unit of the resource becomes available. Semaphores are a useful tool
	 * in the prevention of race conditions; however, their use is by no means a guarantee that a
	 * program is free from these problems. Semaphores which allow an arbitrary resource count are
	 * called counting semaphores, while semaphores which are restricted to the values 0 and 1
	 * (or locked/unlocked, unavailable/available) are called binary semaphores </p>
	 *	\li Categorized in binary semaphore
	 *
	 * <p> Referenced Wediapedia </p>
	 *	\li https://en.wikipedia.org/wiki/Semaphore_(programming)
	 *
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_critical_section.png)
	 *
	 * @note
	 * <p> Of course, semaphore is already defined in linux C and MFC in Window. But it is dependent on
	 * each operating system, so that cannot be compiled in another OS with those semaphores. There's not
	 * a class like semaphore in STL yet. It's the reason why Semaphore is provided. </p>
	 *
	 * <p> As that reason, if STL supports the semaphore in near future, the Semaphore can be deprecated. </p>
	 *
	 * @handbook [Library - Critical Section](https://github.com/samchon/framework/wiki/CPP-Library-Critical_Section)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class Semaphore
	{
	private:
		/**
		 * @brief The size
		 * @details Permitted size of the semaphore
		 */
		size_t capacity_;

		/* ====================================================
			VARIABLES FOR LOCK
		==================================================== */
		/**
		 * @brief Acquired count
		 */
		size_t acquired_;

		std::mutex count_mtx;

		/**
		 * @brief Locker
		 * @details Manages lock and unlock of the semaphore
		 */
		std::mutex mtx;

	public:
		/**
		 * Constructor.
		 *
		 * @param size The size of the semaphore to permit
		 */
		Semaphore(size_t capacity = 2)
		{
			this->acquired_ = 0;
			this->capacity_ = capacity;
		};

		/**
		 * @brief Set size
		 * @details Set permitted size of the semaphore.
		 */
		void setCapacity(size_t val)
		{
			std::unique_lock<std::mutex> uk(count_mtx);

			this->capacity_ = val;
		};

		/* ====================================================
			GETTERS
		==================================================== */
		/**
		 * @brief Get size
		 * @details Returns size which means the permitted count of the semaphore
		 *
		 * @return The size of semaphore
		 */
		auto capacity() const -> size_t
		{
			return capacity_;
		};

		/**
		 * @brief Get acquired size.
		 */
		auto acquired() const -> size_t
		{
			return acquired_;
		};

		/* ====================================================
			LOCKERS
		==================================================== */
		/**
		 * @brief Acquire admission
		 *
		 * @details
		 * <p> Acquires an admission and increases count of admission by 1. </p>
		 * <p> If the count is over permitted size, wait until other admissions to be released. </p>
		 *	\li Lock on mutex
		 */
		void acquire()
		{
			std::unique_lock<std::mutex> uk(count_mtx);

			if (++acquired_ == capacity_)
			{
				uk.unlock();
				mtx.lock();
			}
		};

		/**
		 * @brief Try to acquire admission
		 *
		 * @details
		 * <p> If admission count is below the permitted size, acquire admission and increase the
		 * count by 1 and return true which means succeded to get admission. </p>
		 *
		 *	\li If the count is matched to the permitted size, lock the mutex
		 *
		 * <p> Else, do not acquire admission and return false which means failed to get admmission. </p>
		 *
		 * @return Whether succeded to acquire an admission or not
		 */
		auto tryAcquire() -> bool
		{
			std::unique_lock<std::mutex> uk(count_mtx);

			if (acquired_ == capacity_)
				return false;
			else
			{
				if (++acquired_ >= capacity_)
					return mtx.try_lock();

				return true;
			}
		};

		/**
		 * @brief Release an admission
		 *
		 * @details
		 * Releases an admission what you've acquired.
		 * If the admission count was over the limited size, unlock the mutex.
		 */
		void release()
		{
			std::unique_lock<std::mutex> uk(count_mtx);

			if (acquired_ != 0 && --acquired_ == capacity_ - 1)
				mtx.unlock();
		};
	};
};
};

#include <samchon/library/UniqueAcquire.hpp>
#include <samchon/library/SharedAcquire.hpp>