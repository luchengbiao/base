#ifndef __COMMON_WEAK_CALLBACK_WEAK_CALLBACK_H__
#define __COMMON_WEAK_CALLBACK_WEAK_CALLBACK_H__
#include <atomic>
#include <memory>
#include <functional>
#include <mutex>

namespace wcb
{
	template<typename Callee, typename WeakClass>
	class WeakCallback
	{
		typedef std::weak_ptr<WeakClass>	WeakClassWeakPtr;
		typedef std::shared_ptr<WeakClass>	WeakClassSharedPtr;

	public:
		WeakCallback(const WeakClassWeakPtr& weak_ptr, const Callee& callee)
			: weak_ptr_(weak_ptr)
			, callee_(callee)
		{}

		WeakCallback(const WeakClassWeakPtr& weak_ptr, Callee&& callee)
			: weak_ptr_(weak_ptr)
			, callee_(std::move(callee))
		{}

		template<class WeakType>
		WeakCallback(const WeakType& weak_cb)
			: weak_ptr_(weak_cb.weak_ptr_)
			, callee_(weak_cb.callee_)
		{}

		template<class... Args>
		auto operator ()(Args && ... args) const->decltype(callee_(std::forward<Args>(args)...))
		{
			WeakClassSharedPtr shared_ptr(weak_ptr_.lock());
			if (shared_ptr)
			{
				return callee_(std::forward<Args>(args)...);
			}

			return decltype(callee_(std::forward<Args>(args)...))();
		}

		bool Expired() const
		{
			return weak_ptr_.expired();
		}

	private:
		WeakClassWeakPtr	weak_ptr_;
		mutable Callee		callee_;
	};

	template<typename Derived>
	class SupportWeakCallback : public std::enable_shared_from_this<Derived>
	{
	public:
		virtual ~SupportWeakCallback(){};

		template<typename CallbackType>
		auto ToWeakCallback(const CallbackType& callee)->WeakCallback<CallbackType, Derived>
		{
			return WeakCallback<CallbackType, Derived>(GetWeakPtr(), callee);
		}

		std::weak_ptr<Derived> GetWeakPtr()
		{
			return shared_from_this();
		}

		std::shared_ptr<Derived> GetSharedPtr()
		{
			return shared_from_this();
		}
	};

	class CancelFlag
	{};

	typedef std::weak_ptr<CancelFlag>	CancelFlagWeakPtr;
	typedef std::shared_ptr<CancelFlag>	CancelFlagSharedPtr;

	template<typename Callee, typename WeakClass>
	class WeakCallbackCancelable
	{
		typedef std::weak_ptr<WeakClass>	WeakClassWeakPtr;
		typedef std::shared_ptr<WeakClass>	WeakClassSharedPtr;

	public:
		WeakCallbackCancelable(const WeakClassWeakPtr& weak_ptr, const CancelFlagWeakPtr& flag_weak_ptr, const Callee& callee)
			: weak_ptr_(weak_ptr)
			, flag_weak_ptr_(flag_weak_ptr)
			, callee_(callee)
		{}

		WeakCallbackCancelable(const WeakClassWeakPtr& weak_ptr, const CancelFlagWeakPtr& flag_weak_ptr, Callee&& callee)
			: weak_ptr_(weak_ptr)
			, flag_weak_ptr_(flag_weak_ptr)
			, callee_(std::move(callee))
		{}

		template<class WeakType>
		WeakCallbackCancelable(const WeakType& weak_cb)
			: weak_ptr_(weak_cb.weak_ptr_)
			, flag_weak_ptr_(weak_cb.flag_weak_ptr_)
			, callee_(weak_cb.callee_)
		{}

		template<class... Args>
		auto operator ()(Args && ... args) const->decltype(callee_(std::forward<Args>(args)...))
		{
			CancelFlagSharedPtr flag_shared_ptr(flag_weak_ptr_.lock());
			if (flag_shared_ptr)
			{
				WeakClassSharedPtr shared_ptr(weak_ptr_.lock());
				if (shared_ptr)
				{
					return callee_(std::forward<Args>(args)...);
				}
			}

			return decltype(callee_(std::forward<Args>(args)...))();
		}

		bool Expired() const
		{
			return weak_ptr_.expired() || flag_weak_ptr_.expired();
		}

	private:
		WeakClassWeakPtr	weak_ptr_;
		CancelFlagWeakPtr	flag_weak_ptr_;
		mutable Callee		callee_;
	};

	template<typename Owner>
	class WeakCallbackFlagCancelable final
	{
	public:
		void SetWeakPtr(const std::weak_ptr<Owner>& weak_ptr) { weak_ptr_ = weak_ptr; }

		template<typename CallbackType>
		auto ToWeakCallback(const CallbackType& callee)->WeakCallbackCancelable<CallbackType, Owner>
		{
			return this->ToWeakCallback(callee, weak_ptr_);
		}

		template<typename CallbackType>
		auto ToWeakCallback(const CallbackType& callee, const std::weak_ptr<Owner>& weak_ptr)->WeakCallbackCancelable<CallbackType, Owner>
		{
			return WeakCallbackCancelable<CallbackType, Owner>(weak_ptr, GetCancelFlagWeakPtr(), callee);
		}

		CancelFlagWeakPtr GetCancelFlagWeakPtr()
		{
			auto cancel_flag_ptr = std::atomic_load(&cancel_flag_ptr_);
			if (!cancel_flag_ptr)
			{
				CancelFlagSharedPtr cancel_flag_ptr_null;
				cancel_flag_ptr.reset((CancelFlag*)0x01, [](CancelFlag*){/* do nothing with the fake pointer 0x01*/});

				while (!std::atomic_compare_exchange_weak(&cancel_flag_ptr_, &cancel_flag_ptr_null, cancel_flag_ptr));
			}

			return cancel_flag_ptr;
		}

		void Cancel()
		{
			std::atomic_store(&cancel_flag_ptr_, CancelFlagSharedPtr());
		}

		bool IsInUse()
		{
			auto cancel_flag_ptr = std::atomic_load(&cancel_flag_ptr_);

			return cancel_flag_ptr.use_count() > 0;
		}

	private:
		std::weak_ptr<Owner>	weak_ptr_;
		CancelFlagSharedPtr		cancel_flag_ptr_;
	};


	// global function 
	template<class F, class... Args, class = std::enable_if_t<!std::is_member_function_pointer<F>::value>>
	auto Bind(F && f, Args && ... args)
		->decltype(std::bind(f, args...))
	{
		return std::bind(f, args...);
	}

	// const class member function 
	template<class R, class C, class... DArgs, class P, class... Args>
	auto Bind(R(C::*f)(DArgs...) const, P && p, Args && ... args)->WeakCallback<decltype(std::bind(f, p, args...)), C>
	{
		auto weak_ptr = ((util::SupportWeakCallback*)p)->GetWeakPtr();
		auto bindObj = std::bind(f, p, args...);
		static_assert(std::is_base_of<wcb::SupportWeakCallback, C>::value, "wcb::SupportWeakCallback should be base of C");
		WeakCallback<decltype(bindObj)> weak_cb(weak_ptr, std::move(bindObj));
		return weak_cb;
	}

	// non-const class member function 
	template<class R, class C, class... DArgs, class P, class... Args>
	auto Bind(R(C::*f)(DArgs...), P && p, Args && ... args) ->WeakCallback<decltype(std::bind(f, p, args...)), C>
	{
		auto weak_ptr = ((util::SupportWeakCallback*)p)->GetWeakFlag();
		auto bindObj = std::bind(f, p, args...);
		static_assert(std::is_base_of<wcb::SupportWeakCallback, C>::value, "wcb::SupportWeakCallback should be base of C");
		WeakCallback<decltype(bindObj)> weak_cb(weak_ptr, std::move(bindObj));
		return weak_cb;
	}
}

#endif  // __WEAK_CALLBACK_H__
