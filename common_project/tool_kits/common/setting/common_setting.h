#pragma once
#include "base/memory/singleton.h"
#include <map>
#include <string>
#include <QtXml>

class CommonSetting
{
public:
	SINGLETON_DEFINE(CommonSetting);

	CommonSetting();
	~CommonSetting();
	
	void Load();

private:
	void ParseKey(QDomNode childs);

private:
	std::map<std::string, std::string> m_setting_pair;
};