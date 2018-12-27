#pragma once
#include "base\memory\singleton.h"
#include <string>
#include <QObject>

//快捷键
class ShortCutManager : public QObject
{
	Q_OBJECT
public:
	SINGLETON_DEFINE(ShortCutManager);
	ShortCutManager();
	~ShortCutManager();

	bool ParseCommand(std::string command_str);

signals:
	void SignalMockHeartStart();   //模拟上课心跳开始
	void SignalMockHeartStop();		//模拟上课心跳结束
	void SignalMockAgoraLogout();	//模拟声网登出
	void SignalMockServerHeartStop();//上课过程跟服务器保持心跳连接断开
	void SignalMockServerHeartStart();//上课过程跟服务器保持心跳连接
	void SignalMockOpenClassroom(std::string);

	void SignalO2OPlayback(unsigned long long course_id);
	void SignalO2MPlayback(unsigned long long course_id);
	void SignalReadingSetting(bool);

protected:
private:
};