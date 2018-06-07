#include "count_down_latch.h"
#include <mutex>
#include <condition_variable>

template<typename V>
class ValueGuard
{
public:
	ValueGuard(V& v) : _v(v) { _v += 1; }
	~ValueGuard() { _v -= 1; }

private:
	V& _v;
};

class CountdownLatch::CountdownLatchPrivate
{
public:
	CountdownLatchPrivate()
		: _count(0)
		, _waiters(0)
		, _notifyReason(NotifyReason::None)
	{}

	inline void	Reset(int count)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_count = count;

		if (_waiters > 0)
		{
			_notifyReason = NotifyReason::Reset;

			_condition.notify_all();
		}
	}

	inline void	Wait()
	{
		std::unique_lock<std::mutex> lock(_mutex);

		ValueGuard<unsigned int> vg(_waiters);

		while (_count > 0)
		{
			_condition.wait(lock);
		}
	}

	inline bool	Countdown()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_count -= 1;

		if (_count == 0)
		{
			if (_waiters > 0)
			{
				_notifyReason = NotifyReason::Normal;

				_condition.notify_all();
			}

			return true;
		}

		return false;
	}

	inline bool	CountIsZero() const
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return (_count == 0);
	}

	inline NotifyReason	GetNotifyReason() const
	{
		return _notifyReason;
	}

private:
	mutable std::mutex		_mutex;
	std::condition_variable	_condition;
	unsigned int			_count;
	unsigned int			_waiters;
	NotifyReason			_notifyReason;
};

CountdownLatch::CountdownLatch()
	: _data(new CountdownLatchPrivate)
{}

CountdownLatch::~CountdownLatch() = default;

void CountdownLatch::Reset(int count)
{
	if (_data)
	{
		_data->Reset(count);
	}
}

void  CountdownLatch::Wait()
{
	if (_data)
	{
		_data->Wait();
	}
}

bool  CountdownLatch::Countdown()
{
	if (_data)
	{
		return _data->Countdown();
	}

	return true;
}

bool CountdownLatch::CountIsZero() const
{
	if (_data)
	{
		return _data->CountIsZero();
	}

	return true;
}

CountdownLatch::NotifyReason CountdownLatch::GetNotifyReason() const
{
	if (_data)
	{
		return _data->GetNotifyReason();
	}

	return CountdownLatch::NotifyReason::None;
}
