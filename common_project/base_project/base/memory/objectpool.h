#ifndef BASE_MEMORY_OBJECT_POOL_H_
#define BASE_MEMORY_OBJECT_POOL_H_
#include <algorithm>
#include <mutex>
#include <stddef.h> // for std::size_t
#include <type_traits>
#include <unordered_map>

namespace nbase
{
#define HAS_MEMBER_FUNCTION(member) \
	template<typename T, typename ... Args> \
	struct has_member_##member \
	{ \
	private: \
	template<typename U> \
	static auto test(void*) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	\
	template<typename U>\
	static std::false_type test(...); \
	\
	public:\
	static const bool value = decltype(test<T>(nullptr))::value; \
};

HAS_MEMBER_FUNCTION(resetWithConstructorArgs)

	template<typename Object>
	class ObjectPool
	{
		typedef std::pair<Object*, bool> PairType; // false: idle, true: taken out
		typedef std::unordered_map<Object*, bool> PoolType;
		typedef std::recursive_mutex MutexType;

	public:
		// constructor
		explicit ObjectPool(std::size_t increased_per_time, bool thread_safe)
			: pool_(new PoolType)
			, increased_per_time_(increased_per_time)
			, mutex_(thread_safe ? new MutexType : nullptr)
		{
		}

		// destructor
		~ObjectPool()
		{
			for (const auto& pair : *pool_)
			{
				delete pair.first;
			}

			delete pool_;

			delete mutex_;
		}

		// delete copy constructor
		ObjectPool(const ObjectPool&) = delete;

		// delete assign operator
		ObjectPool& operator=(const ObjectPool&) = delete;

		// take out an object from pool
		template<typename... Args>
		Object* takeOut(Args&&... args)
		{
			Object* obj = nullptr;

			{
				std::lock_guard<ObjectPool> lg(*this);

				auto it = std::find_if(pool_->begin(), pool_->end(), [](const PairType& pair) { return !pair.second; });
				if (it != pool_->end())
				{
					it->second = true;

					obj = it->first;

					this->resetObject(obj, std::forward<Args>(args)...);
				}
				else
				{
					obj = increaseWithoutLock(increased_per_time_, std::forward<Args>(args)...);

					if (obj)
					{
						(*pool_)[obj] = true;
					}
				}
			}

			return obj;
		}

		// give back an object to pool
		bool giveBack(Object* obj)
		{
			if (obj)
			{
				std::lock_guard<ObjectPool> lg(*this);

				auto it = pool_->find(obj);
				if (it != pool_->end())
				{
					it->second = false;

					return true;
				}
			}

			return false;
		}

	private:
		void lock()
		{
			if (mutex_) { mutex_->lock(); }
		}

		void unlock()
		{
			if (mutex_) { mutex_->unlock(); }
		}

		template<typename... Args>
		Object* increaseWithoutLock(std::size_t count, Args&&... args)
		{
			Object* ret = nullptr;

			while (count > 0)
			{
				count -= 1;

				auto obj = new (std::nothrow) Object(std::forward<Args>(args)...);

				if (obj)
				{
					ret = obj;
					pool_->emplace(obj, false);
				}
			}

			return ret;
		}

		template<typename T, typename... Args, typename = std::enable_if_t<has_member_resetWithConstructorArgs<T, Args...>::value>>
		void resetObject(T* obj, Args&&... args)
		{
			obj->resetWithConstructorArgs(std::forward<Args>(args)...);
		}

		void resetObject(...) {}

	private:
		PoolType*			pool_;
		std::size_t			increased_per_time_;
		mutable MutexType*	mutex_;

		friend class std::lock_guard<ObjectPool>;
	};
}

#endif