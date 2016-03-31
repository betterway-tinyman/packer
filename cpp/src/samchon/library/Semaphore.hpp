#pragma once
#include <samchon/API.hpp>

namespace std
{
	template <typename _Ty> struct atomic;
	class mutex;
};
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
	 * @image html cpp/library_critical_section.png
	 * @image latex cpp/library_critical_section.png
	 *
	 * @note
	 * <p> Of course, semaphore is already defined in linux C and MFC in Window. But it is dependent on
	 * each operating system, so that cannot be compiled in another OS with those semaphores. There's not
	 * a class like semaphore in STL yet. It's the reason why Semaphore is provided. </p>
	 *
	 * <p> As that reason, if STL supports the semaphore in near future, the Semaphore can be deprecated. </p>
	 *
	 * @see samchon::library
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API Semaphore
	{
	private:
		/**
		 * @brief The size
		 * @details Permitted size of the semaphore
		 */
		size_t size_;

		/* ====================================================
			VARIABLES FOR LOCK
		==================================================== */
		/**
		 * @brief Acquired count
		 */
		size_t acquired;
		std::mutex *countMtx;

		/**
		 * @brief Locker
		 * @details Manages lock and unlock of the semaphore
		 */
		std::mutex *mtx;

	public:
		/**
		 * Constructor.
		 *
		 * @param size The size of the semaphore to permit
		 */
		Semaphore(size_t = 2);
		~Semaphore();

		/**
		 * @brief Set size
		 * @details Set permitted size of the semaphore.
		 */
		void setSize(size_t);

		/* ====================================================
			GETTERS
		==================================================== */
		/**
		 * @brief Get size
		 * @details Returns size which means the permitted count of the semaphore
		 *
		 * @return The size of semaphore
		 */
		auto size() const->size_t;

		/**
		 * @brief Get acquired size.
		 */
		auto acquiredSize() const->size_t;

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
		void acquire();

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
		auto tryAcquire() -> bool;

		/**
		 * @brief Release an admission
		 *
		 * @details
		 * Releases an admission what you've acquired.
		 * If the admission count was over the limited size, unlock the mutex.
		 */
		void release();
	};
};
};

#include <samchon/library/UniqueAcquire.hpp>
#include <samchon/library/SharedAcquire.hpp>