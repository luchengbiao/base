#include "nim_init_manager.h"
#include <assert.h>
#include "nim_sdk_manager/api/nim_cpp_vchat.h"
#include "nim_sdk_manager/api/nim_tools_http_cpp.h"
#include "nim_sdk_manager/api/nim_cpp_data_sync.h"
#include "nim_sdk_manager/api/nim_cpp_client.h"
#include "nim_sdk_manager/api/nim_cpp_talk.h"
#include "nim_sdk_manager/api/nim_cpp_sysmsg.h"
#include "nim_sdk_manager/api/nim_cpp_msglog.h"
#include "nim_sdk_manager/api/nim_cpp_team.h"
#include "nim_sdk_manager/api/nim_cpp_nos.h"
#include "nim_sdk_manager/api/nim_cpp_rts.h"
#include "nim_sdk_manager/api/nim_cpp_doc_trans.h"
#include "nim_sdk_manager/api/nim_audio_cpp.h"
#include "nim_sdk_manager//api/nim_chatroom_cpp.h"
#include "nim_login/callback/login_callback.h"
#include "nim_session/callback/session_callback.h"
#include "nim_session/callback/team_callback.h"
#include "nim_vchat/callback/vchat_callback.h"
#include "nim_rts/callback/rts_callback.h"
#include "nim_doc/callback/doc_callback.h"
#include "nim_chatroom/manager/chatroom_manager.h"
#include "nim_vchat/manager/vchat_manager.h"
#include "iostream"
using namespace std;
void NimInitManager::RegCallback()
{
	// 初始化云信音视频
	bool ret = nim::VChat::Init("");
	assert(ret);

	// 初始化云信http
	nim_http::Init();

	//注册数据同步结果的回调
	//nim::DataSync::RegCompleteCb(nbase::Bind(&nim_comp::DataSyncCallback::SyncCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	/* 以下注册的回调函数，都是在收到服务器推送的消息或事件时执行的。因此需要在程序开始时就注册好。 */
	//注册重连、被踢、掉线、多点登录、把移动端踢下线的回调
	nim::Client::RegReloginCb(&nim_comp::LoginCallback::OnReLoginCallback);
	nim::Client::RegKickoutCb(&nim_comp::LoginCallback::OnKickoutCallback);
	nim::Client::RegDisconnectCb(&nim_comp::LoginCallback::OnDisconnectCallback);
	nim::Client::RegMultispotLoginCb(&nim_comp::LoginCallback::OnMultispotLoginCallback);
	nim::Client::RegKickOtherClientCb(&nim_comp::LoginCallback::OnKickoutOtherClientCallback);
	//nim::Client::RegSyncMultiportPushConfigCb(&nim_comp::MultiportPushCallback::OnMultiportPushConfigChange);

	//注册返回发送消息结果的回调，和收到消息的回调。
	nim::Talk::RegSendMsgCb(nbase::Bind(&nim_comp::TalkCallback::OnSendMsgCallback, std::placeholders::_1));
	nim::Talk::RegReceiveCb(nbase::Bind(&nim_comp::TalkCallback::OnReceiveMsgCallback, std::placeholders::_1));
	nim::Talk::RegReceiveMessagesCb(nbase::Bind(&nim_comp::TalkCallback::OnReceiveMsgsCallback, std::placeholders::_1));
	nim::SystemMsg::RegSendCustomSysmsgCb(nbase::Bind(&nim_comp::TalkCallback::OnSendCustomSysmsgCallback, std::placeholders::_1));
	nim::MsgLog::RegMessageStatusChangedCb(nbase::Bind(&nim_comp::TalkCallback::OnMsgStatusChangedCallback, std::placeholders::_1));

	//注册群事件的回调
	nim::Team::RegTeamEventCb(nbase::Bind(&nim_comp::TeamCallback::OnTeamEventCallback, std::placeholders::_1));

	//注册返回发送自定义消息的结果的回调，和收到系统消息（包括自定义消息）的回调
	nim::SystemMsg::RegSendCustomSysmsgCb(nbase::Bind(&nim_comp::TalkCallback::OnSendCustomSysmsgCallback, std::placeholders::_1));
	nim::SystemMsg::RegSysmsgCb(nbase::Bind(&nim_comp::TeamCallback::OnReceiveSysmsgCallback, std::placeholders::_1));

	//注册NOS下载回调
	nim::NOS::RegDownloadCb(nbase::Bind(&nim_comp::TalkCallback::OnHttpDownloadCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	//注册音视频回调
	nim::VChat::SetVideoDataCb(true, nim_comp::VChatCallback::VideoCaptureData);
	nim::VChat::SetVideoDataCb(false, nim_comp::VChatCallback::VideoRecData);
	nim::VChat::SetCbFunc(nim_comp::VChatCallback::VChatCb);

	//注册白板消息回调
	nim::Rts::SetStartNotifyCb(nbase::Bind(&nim_comp::RtsCallback::StartNotifyCb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	nim::Rts::SetAckNotifyCb(nbase::Bind(&nim_comp::RtsCallback::AckNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetSyncAckNotifyCb(nbase::Bind(&nim_comp::RtsCallback::SyncAckNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetConnectNotifyCb(nbase::Bind(&nim_comp::RtsCallback::ConnectNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetMemberChangeCb(nbase::Bind(&nim_comp::RtsCallback::MemberNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetHangupNotifyCb(nbase::Bind(&nim_comp::RtsCallback::HangupNotifyCallback, std::placeholders::_1, std::placeholders::_2));
	nim::Rts::SetControlNotifyCb(nbase::Bind(&nim_comp::RtsCallback::ControlNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	nim::Rts::SetRecDataCb(nbase::Bind(&nim_comp::RtsCallback::RecDataCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	//注册文档转换通知
	nim::DocTrans::RegNotifyCb(&nim_comp::DocTransCallback::DocTransNotifyCallback);

	// 班课
	/*nim_chatroom::ChatRoom::RegReceiveMsgCb(nbase::Bind(&nim_chatroom::ChatroomCallback::OnReceiveMsgCallback, std::placeholders::_1, std::placeholders::_2));
	nim_chatroom::ChatRoom::RegSendMsgAckCb(nbase::Bind(&nim_chatroom::ChatroomCallback::OnSendMsgCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	nim_chatroom::ChatRoom::RegEnterCb(nbase::Bind(&nim_chatroom::ChatroomCallback::OnEnterCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	nim_chatroom::ChatRoom::RegExitCb(nbase::Bind(&nim_chatroom::ChatroomCallback::OnExitCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	nim_chatroom::ChatRoom::RegNotificationCb(nbase::Bind(&nim_chatroom::ChatroomCallback::OnNotificationCallback, std::placeholders::_1, std::placeholders::_2));
	nim_chatroom::ChatRoom::RegLinkConditionCb(nbase::Bind(&nim_chatroom::ChatroomCallback::OnRegLinkConditionCallback, std::placeholders::_1, std::placeholders::_2));*/

	nim_comp::ChatroomManager::GetInstance();
}

void NimInitManager::Cleanup()
{
	cout << "SetAudioDataCb ing" << endl;
	nim_comp::VChatManager::GetInstance()->SetAudioDataCb(nullptr);
	cout << "SetAudioDataCb end" << endl;
	cout << "Audio clean up ing" << endl; 
	nim_audio::Audio::Cleanup();
	cout << "Audio clean up end" << endl;
	cout << "VChat clean up start" << endl;
	nim::VChat::Cleanup();
	cout << "VChat clean up end" << endl;
	cout << "Client clean up start" << endl;
	nim::Client::Cleanup();
	cout << "Client clean up end" << endl;
}

void NimInitManager::Init(std::string app_key, std::string app_local_address)
{
	nim::SDKConfig config;
	//sdk能力参数（必填）
	config.sync_session_ack_ = true;
	config.database_encrypt_key_ = "XiaoQiShen_db_key_20180608_"; //string（db key必填，目前只支持最多32个字符的加密密钥！建议使用32个字符）
	bool ret = nim::Client::Init(app_key, app_local_address, "", config); // 载入云信sdk，初始化安装目录和用户目录
	ret = nim_chatroom::ChatRoom::Init("");
	RegCallback();
}

