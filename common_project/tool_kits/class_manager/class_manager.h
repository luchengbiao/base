#pragma once
#include "class_manager_global.h"
#include "base\memory\singleton.h"
#include "qobject.h"
#include "base\callback\callback.h"
#include "base\synchronization\lock.h"
#include "..\base_control\painter\paint_data.h"

//�Ͽι�������
class ClassManager : public QObject, public nbase::SupportWeakCallback
{
	Q_OBJECT
public:
	SINGLETON_DEFINE(ClassManager);
	ClassManager();
	~ClassManager();

	void StartSendDataService();
	void StopSendDataService();
	void SendCommand(One2OneCommandType command_type_, void* param1_ = nullptr, void* param2_ = nullptr);
	void OnParseData(std::string command_str);
	bool AnalyzeDataToCommand(std::string data, std::vector<OrderInfo> &command_list);
	void ExecuteCommandList(std::vector<OrderInfo> &command_list);

private:
	//ע�����׻ص�
	void OnNimControlNotifyCallback(const std::string& session_id, const std::string& info, const std::string& uid);
	void OnNimMemberNotifyCallback(const std::string& session_id, int channel_type, const std::string& uid, int code);
	void OnNimHangupNotifyCallback(const std::string& session_id, const std::string& uid);
	void OnNimRecDataCallback(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data);
	void OnNimConnectNotifyCallback(const std::string& session_id, int channel_type, int code, const std::string& json);

	//ע�������ص�
	void OnAgoraMessageChannelReceive(const std::string s_channel_id, const std::string s_account, uint32_t uid, const std::string s_msg);
	void OnAgoraSignallingChannelJoinCb(std::string s_channel_id);
	void OnAgoraSignallingChannelLeavedCb(std::string s_channel_id, int code);
	void OnAgoraSignallingChannelJoinFailed(const std::string s_channel_id, int error);
	void OnAgoraSignallingChannelInviteEnd(const std::string s_channel_id, const std::string s_account, uint32_t uid, const std::string s_extra);
	void OnAgoraSignallingChannelUserLeaved(const std::string s_account, uint32_t uid);
	void OnAgoraSignallingLogout(int ecode);
	void OnAgoraSignallingMessageInstantReceive(const std::string s_account, uint32_t uid, const std::string s_msg);
	void OnAgoraMediaChannelLost();
	void OnAgoraRejoinMediaChannelSuccess(const std::string s_channel_id, uint32_t uid, int elapsed);
	void OnAgoraMediaChannelError(bool bIgnore, int error);
	void OnAgoraMediaChannelUserOffline(uint32_t uid);
	void OnAgoraDataMsgSendError(std::string message_id, int ecode);

	void OnRegisterCallBack();
	void OnRegisterAgoraCallBack();
	void OnRegisterNimCallBack();

signals:
	//NIm CB Operator
	void SignalNimMemberNotifyCallback(std::string, int, std::string, int);
	void SignalNimHangupNotifyCallback(std::string, std::string);
	void SignalNimControlNotifyCallback(std::string, std::string, std::string);
	void SignalNimRecDataCallback(std::string, int, std::string, std::string);
	void SignalNimConnectNotifyCallback(std::string, int, int, std::string);

	//Agora Cb Operator
	void SignalAgoraMessageChannelReceive(std::string, std::string, uint32_t, std::string);
	void SignalAgoraSignallingChannelJoinCb(std::string);
	void SignalAgoraSignallingChannelLeavedCb(std::string, int);
	void SignalAgoraSignallingChannelJoinFailed(std::string, int);
	void SignalAgoraSignallingChannelInviteEnd(std::string, std::string, uint32_t, std::string);
	void SignalAgoraSignallingChannelUserLeaved(std::string, uint32_t);
	void SignalAgoraSignallingLogout(int);
	void SignalAgoraSignallingMessageInstantReceive(std::string, uint32_t, std::string);
	void SignalAgoraMediaChannelLost();
	void SignalAgoraRejoinMediaChannelSuccess(std::string, uint32_t, int);
	void SignalAgoraMediaChannelError(bool, int);
	void SignalAgoraMediaChannelUserOffline(uint32_t);


	//��������ص�
	void SignalExecuteStuDrawInfoList(std::list<DrawOpInfo>);
	void SignalExecuteTeaDrawInfoList(std::list<DrawOpInfo>);		//ִ�����н�����Ϳд����
	void SignalCommandUploadImage(std::string);						//�ϴ�ͼƬ
	void SignalCommandTeaStartCourse(int64_t, int64_t);				//��ʦ��ʼ�Ͽ�
	void SignalCommandEndCourse();									//�����Ͽ�
	void SignalCommandRefreshBalance();								//ˢ���˺���Ϣ
	void SignalCommandTeaChangePage(One2OneCoursewareType, int);	//��ʦ�л�ҳ�棨�װ壬ͼƬ��ppt��
	void SignalCommandTeaUploadImageFiles(std::string, int file_type);	//��ʦ�ϴ�ppt��ͼƬ
	void SignalCommandTeaChangePrice(std::string);					//��ʦ�޸ļ۸�
	void SignalCommandStartObserve(std::string);					//����
	void SignalCommandReceiveObserveData(std::string);				//������������
	void SignalCommandExchangeUserInfo(std::string);				//�����û���Ϣ
	void SignalCommandStuChangePage(One2OneCoursewareType, int);	//ѧ���л�ҳ�棨�װ壬ͼƬ��ppt��
	void SignalTeaRotateImage(int);									//��ʦ��תͼƬ
	void SignalCommandStuRecvCourseStart(std::string);				//ѧ���յ���ʼ�Ͽ�����
	void SignalCommandExchangeData(int,int);						//13������ ����������Ϣ
	void SignalCommandClassUploadPPT(int, std::string);             //��ʦ����ϴ�ppt
	void SignalCommandClassChangePPTPage(int, int);                 //��ʦ����л�ppt
	void SignalCommandClassClearPPT(int);                           //��ʦ������ĳ��ppt
	void SignalCommandClassStartUpload();                           //��ʦ��ʼ�ϴ���ʷ����
	void SignalCommandClassUploaded(std::string);                   //��ʦ�ϴ���ʷ����
	void SignalCommandVersion();                                    //�汾��

	void SignalCommandWebViewShow(int);								//��ʾ��̬ppt
	void SignalCommandLoadPPTUrl(std::string);						//���ض�̬ppt
	void SignalCommandSwitchPPTPageStep(int, int);					//��̬��תppt
	void SignalCommandTeaAddVedioCourseware(std::string, std::string);  //������Ƶ�װ�
	void SignalCommandTeaPlayVedio(int, int64_t);
	void SignalCommandTeaStopVedio(int64_t);

private:
	void AddCommandBuffer(std::string command);
	void ClearCommandBuffer();
	void SendDataBuffer();

	DrawOpType CommandTypeToDrawOpType(int type);

private:
	nbase::NLock m_send_lock_;  //������Ϣ��
	nbase::NLock m_command_lock_;
	std::string cur_buffer_;
	nbase::WeakCallbackFlag m_send_data_timer_;
};
