#pragma once
#include "base\memory\singleton.h"
#include <string>
#include <QObject>

//��ݼ�
class ShortCutManager : public QObject
{
	Q_OBJECT
public:
	SINGLETON_DEFINE(ShortCutManager);
	ShortCutManager();
	~ShortCutManager();

	bool ParseCommand(std::string command_str);

	void OnlineCoursePlayBack(std::string course_id,std::string class_type="online-course");  //�γ̻ط�
	void SeriesCoursePlayBack(std::string user_id,std::string course_id);  //�γ̻ط�

signals:
	void SignalMockHeartStart();   //ģ���Ͽ�������ʼ
	void SignalMockHeartStop();		//ģ���Ͽ���������
	void SignalMockAgoraLogout();	//ģ�������ǳ�
	void SignalMockServerHeartStop();//�Ͽι��̸������������������ӶϿ�
	void SignalMockServerHeartStart();//�Ͽι��̸�������������������

protected:
private:
};