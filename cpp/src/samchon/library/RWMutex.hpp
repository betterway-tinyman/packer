#pragma once
#include <samchon/API.hpp>

namespace std
{
	template <typename _Ty> struct atomic;
	class condition_variable;
	class mutex;
};
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
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * <p> Of course, rw_mutex is already defined in linux C. But it is dependent on
	 * the linux OS, so that cannot be compiled in Window having the rw_mutex. There's not
	 * a class like rw_mutex in STL yet. It's the reason why RWMutex is provided. </p>
	 *
	 * <p> As that reason, if STL supports the rw_mutex in near future, the RWMutex can be deprecated. </p>
	 * 
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API RWMutex
	{
	private:
		//Status variables
		std::atomic<size_t> *readingCount;
		std::atomic<bool> *isWriting;
		std::mutex *minusMtx;

		//Lockers
		std::condition_variable *cv;
		std::mutex *readMtx;
		std::mutex *writeMtx;

	public:
		/**
		 * @brief Default Constructor
		 */
		RWMutex();
		~RWMutex();

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
		void readLock() const;

		/**
		 * @brief Unlock of read
		 *
		 * @details
		 * <p> Decreases a reading count. </p>
		 *
		 * <p> When write_lock had done after read_lock, it continues by read_unlock
		 * if the reading count was 1 (read_unlock makes the count to be zero). </p>
		 */
		void readUnlock() const;

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
		void writeLock();

		/**
		 * @brief Unlock on writing
		 */
		void writeUnlock();
	};
};
};

#include <samchon/library/UniqueReadLock.hpp>
#include <samchon/library/UniqueWriteLock.hpp>

#include <samchon/library/SharedReadLock.hpp>
#include <samchon/library/SharedWriteLock.hpp>