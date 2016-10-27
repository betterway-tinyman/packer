#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace samchon
{
namespace library
{
	/**
	 * @brief rw_mutex
	 *
	 * @details
	 * <p> A mutex divided into reading and writing. </p>
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/library_critical_section.png)
	 *
	 * @note
	 * <p> Of course, rw_mutex is already defined in linux C. But it is dependent on
	 * the linux OS, so that cannot be compiled in Window having the rw_mutex. There's not
	 * a class like rw_mutex in STL yet. It's the reason why RWMutex is provided. </p>
	 *
	 * <p> As that reason, if STL supports the rw_mutex in near future, the RWMutex can be deprecated. </p>
	 * 
	 * @handbook [Library - Critical Section](https://github.com/samchon/framework/wiki/CPP-Library-Critical_Section)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class RWMutex
	{
	private:
		//Status variables
		size_t reading;
		bool writing;

		//Lockers
		std::condition_variable cv;
		std::mutex read_mtx;
		std::mutex write_mtx;

	public:
		/**
		 * @brief Default Constructor
		 */
		RWMutex()
		{
			reading = 0;
			writing = false;
		};

		/**
		 * @brief Lock on read
		 *
		 * @details
		 * <p> Increases a reading count. </p>
		 * <p> When write_lock is on a progress, wait until write_unlock to be called. </p>
		 *
		 *	\li Reading can be done by multiple sections.
		 *	\li Reading can't be done when writing.
		 *
		 * @warning You've to call read_unlock when the reading work is terminated.
		 */
		void readLock() const
		{
			std::unique_lock<std::mutex> uk((std::mutex&)read_mtx);

			while (writing == true)
				((std::condition_variable&)cv).wait(uk);

			((size_t&)reading)++;
		};

		/**
		 * @brief Unlock of read
		 *
		 * @details
		 * <p> Decreases a reading count. </p>
		 *
		 * <p> When write_lock had done after read_lock, it continues by read_unlock
		 * if the reading count was 1 (read_unlock makes the count to be zero). </p>
		 */
		void readUnlock() const
		{
			std::unique_lock<std::mutex> uk((std::mutex&)read_mtx);

			//차감 전 이미 0 (과도한 readUnlock 수행) 은 안 됨
			if (reading != 0 && --((size_t&)reading) == 0)
				((std::condition_variable&)cv).notify_all();
		};

		/**
		 * @brief Lock on writing
		 *
		 * @details
		 * <p> Changes writing flag to true. </p>
		 *
		 * <p> If another write_lock or read_lock is on a progress, wait until them to be unlocked. </p>
		 *
		 *	\li Writing can be done by only a section at once.
		 *	\li Writing can't be done when reading.
		 *
		 * @note You've to call write_unlock when writing work was terminated.
		 */
		void writeLock()
		{
			std::unique_lock<std::mutex> uk(read_mtx);
			while (reading > 0)
				cv.wait(uk);
			uk.unlock();

			write_mtx.lock();
			writing = true;
		};

		/**
		 * @brief Unlock on writing
		 */
		void writeUnlock()
		{
			writing = false;
			write_mtx.unlock();

			cv.notify_all();
		};
	};
};
};

#include <samchon/library/UniqueReadLock.hpp>
#include <samchon/library/UniqueWriteLock.hpp>

#include <samchon/library/SharedReadLock.hpp>
#include <samchon/library/SharedWriteLock.hpp>