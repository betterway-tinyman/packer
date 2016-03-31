#pragma once
#include <map>
#include <mutex>

namespace samchon
{
	/**
	 * @brief Global shared pointer\n
	 * @details
	 * <p> SmartPointer is a global shared pointer based on map, map referencing. </p>
	 * 
	 * <p> Manages the storage of a pointer, providing a limited garbage-collection 
	 * facility, possibly sharing that management with other objects. </p>
	 * 
	 * <p> Referenced comments of std::allocator. </p>
	 *	\li http://www.cplusplus.com/reference/memory/shared_ptr/
	 *
	 * @tparam The type of managed object
	 * @author Jeongho Nam
	 */
	template<typename _Ty> 
	class SmartPointer
	{
	private:
		/**
		 * @brief Map of use count of each pointer
		 */
		static std::map<_Ty*, size_t> useCountMap;

		/**
		 * @brief Mutex assigned to useCountMap
		 */
		static std::mutex mtx;

	private:
		/**
		 * @brief A pointer managed by SmartPointer
		 */
		_Ty* ptr;

	public:
		/* --------------------------------------------------------------
			CONSTRUCTORS
		-------------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 * @details The object is empty (owns no pointer, use count of zero)
		 */
		SmartPointer()
		{
			ptr = nullptr;
		};

		/**
		 * @brief Constrct from pointer
		 * @details The object owns ptr, setting the use count to add 1
		 *
		 * @param ptr The pointer to own
		 */
		explicit SmartPointer(const _Ty* ptr)
			: SmartPointer()
		{
			reset(ptr);
		};

		/**
		 * @brief Copy Constructor
		 * @details 
		 * The object shares ownership of smartPointer's asset and increases the use count
		 *
		 * @param smartPointer The object to copy
		 */
		SmartPointer(const SmartPointer &smartPointer)
			: SmartPointer(smartPointer.ptr) {};

		/**
		 * @brief Move constructor
		 * @details 
		 * The object acquires the content managed by smartPointer\n
		 * The ceding object becomes empty and there's no change on use count
		 *
		 * @param smartPointer The object to move
		 */
		SmartPointer(SmartPointer &&smartPointer)
		{
			ptr = smartPointer.ptr;
			smartPointer.ptr = nullptr;
		}

		/**
		 * @brief Destroy SmartPointer
		 *
		 * @details 
		 * Destroys the object. But, before, it may produce 
		 * the following side effects depending on the use_count of member 
		 *
		 *	\li If use_count is greater than 1: 
		 *		The use count is decreased by 1.
		 *	\li If use_count is 1 (i.e., the object is the unique owner of 
		 *		the managed pointer): the object pointed by its owned pointer is 
		 *		deleted.
		 *	\li If use_count is zero (i.e., the object is empty), 
		 *		this destructor has no side effects.
		 */
		~SmartPointer()
		{
			std::lock_guard<std::mutex> lockGuard(mtx);
			destruct(ptr);
		};

	public:
		/* --------------------------------------------------------------
			SETTERS
		-------------------------------------------------------------- */
		/**
		 * @brief Reset pointer
		 *
		 * @details
		 * <p> Reset pointer to manage and shrink use count of previous pointer. </p>
		 *
		 * <p> Additionally, a call to this function has the same side effects as if 
		 * SmartPointer's destructor was called before its value changed 
		 * (including the deletion of the managed object if this SmartPointer was unique). </p>
		 *
		 * @param Pointer whose ownership is taken over by the object.
		 *		  Unlike std::shared_ptr, ptr being managed by another SmartPointer does not cause any problem
		 */
		void reset(const _Ty* ptr)
		{
			std::lock_guard<std::mutex> lockGuard(mtx);
			if (this->ptr == ptr)
				return;

			construct((_Ty*)ptr);
			destruct(this->ptr);

			this->ptr = (_Ty*)ptr;
		};

		/* --------------------------------------------------------------
			GETTERS
		-------------------------------------------------------------- */
		/**
		 * @brief Get pointer
		 *
		 * @details
		 * <p> Returns the stored pointer. </p>
		 *
		 * <p> The stored pointer points to the object the shared_ptr object dereferences to, 
		 * which is generally the same as its owned pointer. </p>
		 *
		 * @return The stored pointer
		 */
		inline auto get() const -> _Ty*
		{
			return ptr;
		};

		/**
		 * @brief Dereference object membr
		 * 
		 * @details
		 * <p> Returns a pointer to the object pointed by the stored pointer in order to access one of its members.
		 * This member function shall not be called if the stored pointer is a null pointer. </p>
		 * 
		 * <p> It returns the same value as get(). </p>
		 *
		 * @return A pointer to be managed by SmartPointer
		 */
		auto operator->() const -> _Ty*
		{
			return get();
		};

		/**
		 * @brief Dereference object
		 *
		 * @details
		 * Returns a reference to the object pointerd by pointer.
		 * It is equivalent to: *get()
		 *
		 * @return A reference to the object pointed
		 */
		auto operator*() const -> _Ty&
		{
			return *get();
		};

		/* --------------------------------------------------------------
			STATISTICS
		-------------------------------------------------------------- */
	private:
		static void construct(_Ty *ptr)
		{
			if (ptr == nullptr)
				return;

			if (useCountMap.find(ptr) != useCountMap.end())
				useCountMap[ptr]++;
			else
				useCountMap[ptr] = 1;
		}
		static void destruct(_Ty *ptr)
		{
			if (ptr == nullptr)
				return;

			if (useCountMap.find(ptr) != useCountMap.end())
				if (--useCountMap[ptr] == 0)
				{
					useCountMap.erase(ptr);
					delete ptr;
				}
		};
	};

	template<typename _Ty> std::map<_Ty*, size_t> SmartPointer<_Ty>::useCountMap;
	template<typename _Ty> std::mutex SmartPointer<_Ty>::mtx;
};