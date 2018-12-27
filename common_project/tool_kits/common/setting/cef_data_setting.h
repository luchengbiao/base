#pragma once
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include <string>
#include <map>

class CefDataCache : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(CefDataCache);
public:
	CefDataCache();
	~CefDataCache();

	void SetDataDir(std::wstring dir);
	void Set(std::string key, std::string val);
	std::string Get(std::string key);

	void Test(std::string val);

private:
	std::wstring data_dir_;
};