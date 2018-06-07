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

void NimInitManager::RegCallback()
{
	// ��ʼ����������Ƶ
	bool ret = nim::VChat::Init("");
	assert(ret);

	// ��ʼ������http
	nim_http::Init();

	//ע������ͬ������Ļص�
	//nim::DataSync::RegCompleteCb(nbase::Bind(&nim_comp::DataSyncCallback::SyncCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	/* ����ע��Ļص��������������յ����������͵���Ϣ���¼�ʱִ�еġ������Ҫ�ڳ���ʼʱ��ע��á� */
	//ע�����������ߡ����ߡ�����¼�����ƶ��������ߵĻص�
	nim::Client::RegReloginCb(&nim_comp::LoginCallback::OnReLoginCallback);
	nim::Client::RegKickoutCb(&nim_comp::LoginCallback::OnKickoutCallback);
	nim::Client::RegDisconnectCb(&nim_comp::LoginCallback::OnDisconnectCallback);
	nim::Client::RegMultispotLoginCb(&nim_comp::LoginCallback::OnMultispotLoginCallback);
	nim::Client::RegKickOtherClientCb(&nim_comp::LoginCallback::OnKickoutOtherClientCallback);
	//nim::Client::RegSyncMultiportPushConfigCb(&nim_comp::MultiportPushCallback::OnMultiportPushConfigChange);

	//ע�᷵�ط�����Ϣ����Ļص������յ���Ϣ�Ļص���
	nim::Talk::RegSendMsgCb(nbase::Bind(&nim_comp::TalkCallback::OnSendMsgCallback, std::placeholders::_1));
	nim::Talk::RegReceiveCb(nbase::Bind(&nim_comp::TalkCallback::OnReceiveMsgCallback, std::placeholders::_1));
	nim::Talk::RegReceiveMessagesCb(nbase::Bind(&nim_comp::TalkCallback::OnReceiveMsgsCallback, std::placeholders::_1));
	nim::SystemMsg::RegSendCustomSysmsgCb(nbase::Bind(&nim_comp::TalkCallback::OnSendCustomSysmsgCallback, std::placeholders::_1));
	nim::MsgLog::RegMessageStatusChangedCb(nbase::Bind(&nim_comp::TalkCallback::OnMsgStatusChangedCallback, std::placeholders::_1));

	//ע��Ⱥ�¼��Ļص�
	nim::Team::RegTeamEventCb(nbase::Bind(&nim_comp::TeamCallback::OnTeamEventCallback, std::placeholders::_1));

	//ע�᷵�ط����Զ�����Ϣ�Ľ���Ļص������յ�ϵͳ��Ϣ�������Զ�����Ϣ���Ļص�
	nim::SystemMsg::RegSendCustomSysmsgCb(nbase::Bind(&nim_comp::TalkCallback::OnSendCustomSysmsgCallback, std::placeholders::_1));
	nim::SystemMsg::RegSysmsgCb(nbase::Bind(&nim_comp::TeamCallback::OnReceiveSysmsgCallback, std::placeholders::_1));

	//ע��NOS���ػص�
	nim::NOS::RegDownloadCb(nbase::Bind(&nim_comp::TalkCallback::OnHttpDownloadCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	//ע������Ƶ�ص�
	nim::VChat::SetVideoDataCb(true, nim_comp::VChatCallback::VideoCaptureData);
	nim::VChat::SetVideoDataCb(false, nim_comp::VChatCallback::VideoRecData);
	nim::VChat::SetCbFunc(nim_comp::VChatCallback::VChatCb);

	//ע��װ���Ϣ�ص�
	nim::Rts::SetStartNotifyCb(nbase::Bind(&nim_comp::RtsCallback::StartNotifyCb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	nim::Rts::SetAckNotifyCb(nbase::Bind(&nim_comp::RtsCallback::AckNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetSyncAckNotifyCb(nbase::Bind(&nim_comp::RtsCallback::SyncAckNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetConnectNotifyCb(nbase::Bind(&nim_comp::RtsCallback::ConnectNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetMemberChangeCb(nbase::Bind(&nim_comp::RtsCallback::MemberNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	nim::Rts::SetHangupNotifyCb(nbase::Bind(&nim_comp::RtsCallback::HangupNotifyCallback, std::placeholders::_1, std::placeholders::_2));
	nim::Rts::SetControlNotifyCb(nbase::Bind(&nim_comp::RtsCallback::ControlNotifyCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	nim::Rts::SetRecDataCb(nbase::Bind(&nim_comp::RtsCallback::RecDataCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	//ע���ĵ�ת��֪ͨ
	nim::DocTrans::RegNotifyCb(&nim_comp::DocTransCallback::DocTransNotifyCallback);

	// ���
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
	nim_comp::VChatManager::GetInstance()->SetAudioDataCb(nullptr);
	nim_audio::Audio::Cleanup();
	nim::VChat::Cleanup();
	nim::Client::Cleanup();
}

void NimInitManager::Init(std::string app_key, std::string app_local_address)
{
	nim::SDKConfig config;
	//sdk�������������
	config.database_encrypt_key_ = "Jiayouxueba_db_key_20160314_"; //string��db key���Ŀǰֻ֧�����32���ַ��ļ�����Կ������ʹ��32���ַ���
	bool ret = nim::Client::Init(app_key, app_local_address, "", config); // ��������sdk����ʼ����װĿ¼���û�Ŀ¼

	ret = nim_chatroom::ChatRoom::Init("");

	RegCallback();
}

