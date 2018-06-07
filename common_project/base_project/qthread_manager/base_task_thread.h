#pragma once
#include "framework_qthread.h"
#include "qbasethread.h"

class BaseTaskThread : public nbase::FrameworkQThread
{
public:
	BaseTaskThread(nbase::QThreadId thread_id, const char *name);
	~BaseTaskThread(void);

private:
	virtual void Init() override;
	virtual void Cleanup() override;

private:
	nbase::QThreadId thread_id_;
};