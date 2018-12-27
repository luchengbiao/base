#ifndef __COMMON_WEAK_CALLBACK_SINGLETON_SUPPORT_WEAK_CALLBACK_H__
#define __COMMON_WEAK_CALLBACK_SINGLETON_SUPPORT_WEAK_CALLBACK_H__
#include "weak_callback.h"

namespace wcb
{
	template<typename DerivedT>
	class SingletonSupportWeakCallback : public SupportWeakCallback<DerivedT>
	{
	public:
		static const std::shared_ptr<DerivedT>& GetInstance()
		{
			if (!instance_)
			{
				std::call_once(once_flag_, []{
					instance_.reset(new DerivedT); // if DerivedT's constructor is private or protected,  SingletonSupportWeakCallback<DerivedT> must be friend class of DerivedT
					instance_->Init();
				});
			}

			return instance_;
		}

		static bool IsInstantiated()
		{
			return instance_? true : false;
		}

		SingletonSupportWeakCallback(const SingletonSupportWeakCallback&) = delete;
		SingletonSupportWeakCallback& operator=(const SingletonSupportWeakCallback&) = delete;

	protected:
		SingletonSupportWeakCallback() = default;

		virtual void Init() {}

	private:
		static std::shared_ptr<DerivedT> instance_;
		static std::once_flag once_flag_;
	};

	template<typename DerivedT>
	std::shared_ptr<DerivedT> SingletonSupportWeakCallback<DerivedT>::instance_;

	template<typename DerivedT>
	std::once_flag SingletonSupportWeakCallback<DerivedT>::once_flag_;
}

#endif