#pragma once
#include "base\memory\singleton.h"
#include "base\callback\callback.h"

class NimInitManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(NimInitManager);
	NimInitManager(){};
	~NimInitManager(){};
public:
	void Init(std::string app_key, std::string app_local_address);
	void Cleanup();

private:
	void RegCallback();
};