#ifndef __COUNTDOWN_LATCH_H__
#define __COUNTDOWN_LATCH_H__
#include <memory>

class CountdownLatch
{
public:
	enum class NotifyReason
	{
		None,
		Normal,
		Reset,
	};

public:
	CountdownLatch();
	~CountdownLatch();

	void			Reset(int count);

	void			Wait();

	bool			Countdown();

	bool			CountIsZero() const;

	NotifyReason	GetNotifyReason() const;

private:
	class CountdownLatchPrivate;
	std::unique_ptr<CountdownLatchPrivate> _data;
};

#endif