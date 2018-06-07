#include "session_callback.h"
#include "log_manager\log.h"
#include "manager\session_manager.h"
namespace nim_comp
{
void TalkCallback::OnSendMsgCallback(const nim::SendMessageArc& arc)
{
	QLOG_APP(L"OnSendMsgCallback: id={0} msg_id={1} code={2}") << arc.talk_id_ << arc.msg_id_ << arc.rescode_;
	SessionManager::GetInstance()->OnSendMsgCallback(arc);
}

void TalkCallback::OnReceiveMsgCallback(const nim::IMMessage& message)
{
	//收到信息的回调
	QLOG_PRO(L"OnReceiveMsgCallback: {0} from client:{1}") << message.client_msg_id_ << message.readonly_sender_client_type_;
	SessionManager::GetInstance()->OnReceiveMsgCallback(message);
}

void TalkCallback::OnReceiveMsgsCallback(const std::list<nim::IMMessage>& messages)
{
	SessionManager::GetInstance()->OnReceiveMsgsCallback(messages);
}

void TalkCallback::OnReceiveRecallMsgCallback(nim::NIMResCode code, const std::list<nim::RecallMsgNotify>& message)
{
	SessionManager::GetInstance()->OnReceiveRecallMsgCallback(code, message);
}

void TalkCallback::OnHttpDownloadCallback(nim::NIMResCode code, const std::string& file_path, const std::string& sid, const std::string& cid)
{
	SessionManager::GetInstance()->OnDownloadCallback(cid, code == nim::kNIMResSuccess);
}

void TalkCallback::OnMsgStatusChangedCallback(const nim::MessageStatusChangedResult& res)
{
	QLOG_APP(L"TalkCallback: OnMsgStatusChangedCallback");
	SessionManager::GetInstance()->OnMsgStatusChangedCallback(res);
}

void TalkCallback::OnSendCustomSysmsgCallback(const nim::SendMessageArc& arc)
{
	if (arc.rescode_ != nim::kNIMResSuccess)
	{
		QLOG_ERR(L"OnSendCustomSysmsgCallback: id={0} msg_id={1} code={2}") << arc.talk_id_ << arc.msg_id_ << arc.rescode_;
		SessionManager::GetInstance()->OnSendCustomSysmsgCallback(arc);
	}
}

void TalkCallback::OnQueryMsgCallback(nim::NIMResCode code, const std::string& query_id, nim::NIMSessionType query_type, const nim::QueryMsglogResult& result)
{
	QLOG_APP(L"query end: id={0} type={1} code={2} source={3}") <<query_id <<query_type <<code<< result.source_;

	SessionManager::GetInstance()->OnQueryMsgCallback(code, query_id, query_type, result);
}

void TalkCallback::OnQuerySessionListCallback(int unread_count, const nim::SessionDataList& session_list)
{
	QLOG_PRO(L"local session list: count :{0} - unread :{1}") << session_list.count_ << session_list.unread_count_;

	SessionManager::GetInstance()->OnQuerySessionListCallback(unread_count, session_list);
}

}