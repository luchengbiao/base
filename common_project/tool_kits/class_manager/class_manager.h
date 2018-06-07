#pragma once
#include "class_manager_global.h"
#include "base\memory\singleton.h"
#include "qobject.h"
#include "base\callback\callback.h"
#include "base\synchronization\lock.h"
#include "..\base_control\painter\paint_data.h"

//上课公共部分
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
	//注册网易回调
	void OnNimControlNotifyCallback(const std::string& session_id, const std::string& info, const std::string& uid);
	void OnNimMemberNotifyCallback(const std::string& session_id, int channel_type, const std::string& uid, int code);
	void OnNimHangupNotifyCallback(const std::string& session_id, const std::string& uid);
	void OnNimRecDataCallback(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data);
	void OnNimConnectNotifyCallback(const std::string& session_id, int channel_type, int code, const std::string& json);

	//注册声网回调
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


	//命令解析回调
	void SignalExecuteStuDrawInfoList(std::list<DrawOpInfo>);
	void SignalExecuteTeaDrawInfoList(std::list<DrawOpInfo>);		//执行所有解析的涂写命令
	void SignalCommandUploadImage(std::string);						//上传图片
	void SignalCommandTeaStartCourse(int64_t, int64_t);				//老师开始上课
	void SignalCommandEndCourse();									//结束上课
	void SignalCommandRefreshBalance();								//刷新账号信息
	void SignalCommandTeaChangePage(One2OneCoursewareType, int);	//老师切换页面（白板，图片，ppt）
	void SignalCommandTeaUploadImageFiles(std::string, int file_type);	//老师上传ppt，图片
	void SignalCommandTeaChangePrice(std::string);					//老师修改价格
	void SignalCommandStartObserve(std::string);					//旁听
	void SignalCommandReceiveObserveData(std::string);				//旁听接收数据
	void SignalCommandExchangeUserInfo(std::string);				//交换用户信息
	void SignalCommandStuChangePage(One2OneCoursewareType, int);	//学生切换页面（白板，图片，ppt）
	void SignalTeaRotateImage(int);									//老师旋转图片
	void SignalCommandStuRecvCourseStart(std::string);				//学生收到开始上课命令
	void SignalCommandExchangeData(int,int);						//13号命令 交互数据信息
	void SignalCommandClassUploadPPT(int, std::string);             //老师班课上传ppt
	void SignalCommandClassChangePPTPage(int, int);                 //老师班课切换ppt
	void SignalCommandClassClearPPT(int);                           //老师班课清除某个ppt
	void SignalCommandClassStartUpload();                           //老师开始上传历史命令
	void SignalCommandClassUploaded(std::string);                   //老师上传历史命令
	void SignalCommandVersion();                                    //版本号

	void SignalCommandWebViewShow(int);								//显示动态ppt
	void SignalCommandLoadPPTUrl(std::string);						//加载动态ppt
	void SignalCommandSwitchPPTPageStep(int, int);					//动态跳转ppt
	void SignalCommandTeaAddVedioCourseware(std::string, std::string);  //增加视频白板
	void SignalCommandTeaPlayVedio(int, int64_t);
	void SignalCommandTeaStopVedio(int64_t);

private:
	void AddCommandBuffer(std::string command);
	void ClearCommandBuffer();
	void SendDataBuffer();

	DrawOpType CommandTypeToDrawOpType(int type);

private:
	nbase::NLock m_send_lock_;  //发送消息锁
	nbase::NLock m_command_lock_;
	std::string cur_buffer_;
	nbase::WeakCallbackFlag m_send_data_timer_;
};
