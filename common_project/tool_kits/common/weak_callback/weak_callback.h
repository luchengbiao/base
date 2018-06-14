#ifndef __WEAK_CALLBACK_H__
#define __WEAK_CALLBACK_H__
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
		WeakCallback(const WeakClassWeakPtr& weakPtr, const Callee& callee)
			: _weakPtr(weakPtr)
			, _callee(callee)
		{}

		WeakCallback(const WeakClassWeakPtr& weakPtr, Callee&& callee)
			: _weakPtr(weakPtr)
			, _callee(std::move(callee))
		{}

		template<class WeakType>
		WeakCallback(const WeakType& weakCallback)
			: _weakPtr(weakCallback._weakPtr)
			, _callee(weakCallback._callee)
		{}

		template<class... Args>
		auto operator ()(Args && ... args) const->decltype(_callee(std::forward<Args>(args)...))
		{
			WeakClassSharedPtr sharedPtr(_weakPtr.lock());
			if (sharedPtr)
			{
				return _callee(std::forward<Args>(args)...);
			}

			return decltype(_callee(std::forward<Args>(args)...))();
		}

		bool Expired() const
		{
			return _weakPtr.expired();
		}

	private:
		WeakClassWeakPtr	_weakPtr;
		mutable Callee		_callee;
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
		WeakCallbackCancelable(const WeakClassWeakPtr& weakPtr, const CancelFlagWeakPtr& flagWeakPtr, const Callee& callee)
			: _weakPtr(weakPtr)
			, _flagWeakPtr(flagWeakPtr)
			, _callee(callee)
		{}

		WeakCallbackCancelable(const WeakClassWeakPtr& weakPtr, const CancelFlagWeakPtr& flagWeakPtr, Callee&& callee)
			: _weakPtr(weakPtr)
			, _flagWeakPtr(flagWeakPtr)
			, _callee(std::move(callee))
		{}

		template<class WeakType>
		WeakCallbackCancelable(const WeakType& weakCallback)
			: _weakPtr(weakCallback._weakPtr)
			, _flagWeakPtr(weakCallback._flagWeakPtr)
			, _callee(weakCallback._callee)
		{}

		template<class... Args>
		auto operator ()(Args && ... args) const->decltype(_callee(std::forward<Args>(args)...))
		{
			CancelFlagSharedPtr flagSharedPtr(_flagWeakPtr.lock());
			if (flagSharedPtr)
			{
				WeakClassSharedPtr sharedPtr(_weakPtr.lock());
				if (sharedPtr)
				{
					return _callee(std::forward<Args>(args)...);
				}
			}

			return decltype(_callee(std::forward<Args>(args)...))();
		}

		bool Expired() const
		{
			return _weakPtr.expired() || _flagWeakPtr.expired();
		}

	private:
		WeakClassWeakPtr	_weakPtr;
		CancelFlagWeakPtr	_flagWeakPtr;
		mutable Callee		_callee;
	};

	template<typename Owner>
	class WeakCallbackFlagCancelable final
	{
	public:
		void SetWeakPtr(const std::weak_ptr<Owner>& weakPtr) { _weakPtr = weakPtr; }

		template<typename CallbackType>
		auto ToWeakCallback(const CallbackType& callee)->WeakCallbackCancelable<CallbackType, Owner>
		{
			return this->ToWeakCallback(callee, _weakPtr);
		}

		template<typename CallbackType>
		auto ToWeakCallback(const CallbackType& callee, const std::weak_ptr<Owner>& weakPtr)->WeakCallbackCancelable<CallbackType, Owner>
		{
			return WeakCallbackCancelable<CallbackType, Owner>(weakPtr, GetCancelFlagWeakPtr(), callee);
		}

		CancelFlagWeakPtr GetCancelFlagWeakPtr()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (!_cancelFlagPtr)
			{
				_cancelFlagPtr.reset((CancelFlag*)0x01, [](CancelFlag*){/* do nothing with the fake pointer 0x01*/});
			}

			return _cancelFlagPtr;
		}

		void Cancel()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_cancelFlagPtr.reset();
		}

		bool HasUsed()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _cancelFlagPtr.use_count() > 0;
		}

	private:
		std::weak_ptr<Owner>				_weakPtr;
		CancelFlagSharedPtr					_cancelFlagPtr;
		std::mutex							_mutex;
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
		auto weakPtr = ((util::SupportWeakCallback*)p)->GetWeakPtr();
		auto bindObj = std::bind(f, p, args...);
		static_assert(std::is_base_of<wcb::SupportWeakCallback, C>::value, "wcb::SupportWeakCallback should be base of C");
		WeakCallback<decltype(bindObj)> weakCallback(weakPtr, std::move(bindObj));
		return weakCallback;
	}

	// non-const class member function 
	template<class R, class C, class... DArgs, class P, class... Args>
	auto Bind(R(C::*f)(DArgs...), P && p, Args && ... args) ->WeakCallback<decltype(std::bind(f, p, args...)), C>
	{
		auto weakPtr = ((util::SupportWeakCallback*)p)->GetWeakFlag();
		auto bindObj = std::bind(f, p, args...);
		static_assert(std::is_base_of<wcb::SupportWeakCallback, C>::value, "wcb::SupportWeakCallback should be base of C");
		WeakCallback<decltype(bindObj)> weakCallback(weakPtr, std::move(bindObj));
		return weakCallback;
	}
}

#endif  // __WEAK_CALLBACK_H__
