#include "session_manager.h"
#include "msg_filter_manager.h"
#include "session_util.h"
#include "nim_sdk_manager\api\nim_cpp_team.h"
#include "..\nim_sdk_manager\api\nim_cpp_user.h"

namespace nim_comp {

	SessionManager::SessionManager(){
	}

	SessionManager::~SessionManager(){
	}

	UnregisterCallback SessionManager::RegSendMsgCb(const nim::Talk::SendMsgAckCallback& callback)
	{
		nim::Talk::SendMsgAckCallback* new_callback = new nim::Talk::SendMsgAckCallback(callback);
		int cb_id = (int)new_callback;
		send_msg_ack_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			send_msg_ack_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegReceiveMsgCb(const nim::Talk::ReceiveMsgCallback& callback)
	{
		nim::Talk::ReceiveMsgCallback* new_callback = new nim::Talk::ReceiveMsgCallback(callback);
		int cb_id = (int)new_callback;
		receive_msg_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			receive_msg_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegMessageStatusChangedCb(const nim::MsgLog::MessageStatusChangedCallback& callback)
	{
		nim::MsgLog::MessageStatusChangedCallback* new_callback = new nim::MsgLog::MessageStatusChangedCallback(callback);
		int cb_id = (int)new_callback;
		message_status_changed_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			message_status_changed_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegReceiveMsgsCb(const nim::Talk::ReceiveMsgsCallback& callback)
	{
		nim::Talk::ReceiveMsgsCallback* new_callback = new nim::Talk::ReceiveMsgsCallback(callback);
		int cb_id = (int)new_callback;
		receive_msgs_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			receive_msgs_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegSendCustomSysmsgCb(const nim::SystemMsg::SendCustomSysmsgCallback& callback)
	{
		nim::SystemMsg::SendCustomSysmsgCallback* new_callback = new nim::SystemMsg::SendCustomSysmsgCallback(callback);
		int cb_id = (int)new_callback;
		send_custom_sysmsg_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			send_custom_sysmsg_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegQueryMsgCb(const nim::MsgLog::QueryMsgCallback& callback)
	{
		nim::MsgLog::QueryMsgCallback* new_callback = new nim::MsgLog::QueryMsgCallback(callback);
		int cb_id = (int)new_callback;
		query_msg_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			query_msg_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegQuerySessionListCb(const nim::Session::QuerySessionListCallabck& callback)
	{
		nim::Session::QuerySessionListCallabck* new_callback = new nim::Session::QuerySessionListCallabck(callback);
		int cb_id = (int)new_callback;
		query_session_list_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			query_session_list_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegRecallMsgsCb(const nim::Talk::RecallMsgsCallback& callback)
	{
		nim::Talk::RecallMsgsCallback* new_callback = new nim::Talk::RecallMsgsCallback(callback);
		int cb_id = (int)new_callback;
		recall_msgs_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			recall_msgs_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegDownloadCb(const SessionDownloadCallback& callback)
	{
		SessionDownloadCallback* new_callback = new SessionDownloadCallback(callback);
		int cb_id = (int)new_callback;
		download_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			download_cb_list_.erase(cb_id);
		});
		return cb;
	}

	UnregisterCallback SessionManager::RegSessionChangeCb(const ChangeCallback& callback)
	{
		ChangeCallback* new_callback = new ChangeCallback(callback);
		int cb_id = (int)new_callback;
		change_cb_list_[cb_id].reset(new_callback);

		auto cb = ToWeakCallback([this, cb_id]() {
			change_cb_list_.erase(cb_id);
		});
		return cb;
	}

	void SessionManager::OnSendMsgCallback(const nim::SendMessageArc& arc)
	{
		for (auto& it : send_msg_ack_cb_list_)
		{
			(*(it.second))(arc);
		}
	}

	void SessionManager::OnReceiveMsgCallback(const nim::IMMessage& message)
	{
		//如果消息没有被拦截，则回调。
		if (!MsgFilterManager::GetInstance()->StartFilter(message))
		{
			for (auto& it : receive_msg_cb_list_)
			{
				(*(it.second))(message);
			}
		}
	}

	void SessionManager::OnReceiveMsgsCallback(const std::list<nim::IMMessage>& messages)
	{
		for (auto& it : receive_msgs_cb_list_)
		{
			(*(it.second))(messages);
		}
	}

	void SessionManager::OnMsgStatusChangedCallback(const nim::MessageStatusChangedResult& res)
	{
		for (auto& it : message_status_changed_cb_list_)
		{
			(*(it.second))(res);
		}
	}

	void SessionManager::OnSendCustomSysmsgCallback(const nim::SendMessageArc& arc)
	{
		for (auto& it : send_custom_sysmsg_cb_list_)
		{
			(*(it.second))(arc);
		}
	}

	void SessionManager::OnQueryMsgCallback(nim::NIMResCode code, const std::string& query_id, nim::NIMSessionType query_type, const nim::QueryMsglogResult& result)
	{
		for (auto& it : query_msg_cb_list_)
		{
			(*(it.second))(code, query_id, query_type, result);
		}
	}

	void SessionManager::OnQuerySessionListCallback(int unread_count, const nim::SessionDataList& session_list)
	{
		nim::SessionDataList result_list = MsgFilterManager::GetInstance()->StartSessionMsgFilter(unread_count, session_list);
		for (auto& it : query_session_list_cb_list_)
		{
			(*(it.second))(unread_count, result_list);
		}
	}

	void SessionManager::OnReceiveRecallMsgCallback(nim::NIMResCode code, const std::list<nim::RecallMsgNotify>& message)
	{
		for (auto& it : recall_msgs_cb_list_)
		{
			(*(it.second))(code, message);
		}
	}

	void SessionManager::OnDownloadCallback(const std::string &cid, bool success)
	{
		for (auto& it : download_cb_list_)
		{
			(*(it.second))(cid, success);
		}
	}

	void SessionManager::SendMsg(const std::string& json_msg, const std::string& json_extension /*= ""*/, nim::Talk::FileUpPrgCallback* pcb /*= nullptr*/)
	{
		nim::Talk::SendMsg(json_msg, json_extension, pcb);
	}

	bool SessionManager::StopSendMsg(const std::string& client_msg_id, const nim::NIMMessageType& type, const std::string& json_extension /*= ""*/)
	{
		return nim::Talk::StopSendMsg(client_msg_id, type, json_extension);
	}

	void SessionManager::RecallMsg(const nim::IMMessage &msg, const std::string &notify, const nim::Talk::RecallMsgsCallback& cb, const std::string& json_extension /*= ""*/)
	{
		nim::Talk::RecallMsg(msg, notify, cb, json_extension);
	}

	std::string SessionManager::CreateTextMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
														const std::string& content, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateTextMessage(receiver_id, session_type, client_msg_id, content, msg_setting, timetag);
	}

	std::string SessionManager::CreateImageMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
														const nim::IMImage& image, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateImageMessage(receiver_id, session_type, client_msg_id, image, file_path, msg_setting, timetag);
	}

	std::string SessionManager::CreateFileMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
														const nim::IMFile& file, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateFileMessage(receiver_id, session_type, client_msg_id, file, file_path, msg_setting, timetag);
	}

	std::string SessionManager::CreateAudioMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
														const nim::IMAudio& audio, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateAudioMessage(receiver_id, session_type, client_msg_id, audio, file_path, msg_setting, timetag);
	}

	std::string SessionManager::CreateVideoMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
														const nim::IMVideo& video, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateVideoMessage(receiver_id, session_type, client_msg_id, video, file_path, msg_setting, timetag);
	}

	std::string SessionManager::CreateLocationMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
														const nim::IMLocation& location, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateLocationMessage(receiver_id, session_type, client_msg_id, location, msg_setting, timetag);
	}

	std::string SessionManager::CreateTipMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id, 
													const std::string& tip_content, const nim::MessageSetting& msg_setting, int64_t timetag /*= 0*/)
	{
		return nim::Talk::CreateTipMessage(receiver_id, session_type, client_msg_id, tip_content, msg_setting, timetag);
	}

	bool SessionManager::ParseIMMessage(const std::string& json_msg, nim::IMMessage& msg)
	{
		return nim::Talk::ParseIMMessage(json_msg, msg);
	}

	bool SessionManager::ParseImageMessageAttach(const nim::IMMessage& msg, nim::IMImage& image)
	{
		return nim::Talk::ParseImageMessageAttach(msg, image);
	}

	bool SessionManager::ParseFileMessageAttach(const nim::IMMessage& msg, nim::IMFile& file)
	{
		return nim::Talk::ParseFileMessageAttach(msg, file);
	}

	void SessionManager::QueryAllRecentSessionAsync()
	{
		nim::Session::QueryAllRecentSessionAsync(nbase::Bind(&SessionManager::OnQuerySessionListCallback, this, std::placeholders::_1, std::placeholders::_2));
	}

	void SessionManager::QueryMsgAsync(const std::string& account_id, nim::NIMSessionType to_type, int limit_count, __int64 anchor_msg_time)
	{
		nim::MsgLog::QueryMsgAsync(account_id, to_type, limit_count, anchor_msg_time, nbase::Bind(&SessionManager::OnQueryMsgCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}

	void SessionManager::SetMsgLogToLocalAsync(const std::string& talk_id, const nim::IMMessage& msg, bool need_update_session, const WriteMsglogCallback& cb, const std::string& json_extension /*= ""*/)
	{
		nim::MsgLog::WriteMsglogToLocalAsync(talk_id, msg, need_update_session, cb);
	}

	bool SessionManager::CheckIfTimeVisible(const long long old_timestamp, const long long new_timestamp)
	{
		return CheckIfShowTime(old_timestamp, new_timestamp);
	}

	bool SessionManager::SetSessionTop(enum nim::NIMSessionType to_type, const std::string &id, bool top, const std::string &json_extension /*= ""*/)
	{
		return nim::Session::SetSessionTop(to_type, id, top, nbase::Bind(&SessionManager::OnSetSessionTopCb,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3), json_extension);
	}

	void SessionManager::OnSetSessionTopCb(nim::NIMResCode code, const nim::SessionData& data, int status)
	{
		for (auto& it : change_cb_list_)
		{
			(*(it.second))(code,data, status);
		}
	}

	bool SessionManager::SetUnreadCountZeroAsync(nim::NIMSessionType to_type, const std::string & id, const std::string & json_extension /*= ""*/)
	{
		//两句话需要一起使用，不是一起使用会出现影响到会话列表的情况。
		bool res = nim::Session::SetUnreadCountZeroAsync(to_type, id, nbase::Bind(&SessionManager::OnSetSessionTopCb, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), json_extension);
		nim::MsgLog::BatchStatusReadAsync(id, to_type, nullptr, "");
		return res;
	}

	void SessionManager::QueryOnlineMsgAsync(const std::string& account_id, nim::NIMSessionType to_type, int limit_count,__int64 from_time,__int64 to_time)
	{
		nim::MsgLog::QueryMsgOnlineAsync(account_id, to_type, limit_count, from_time, to_time, 0, false, true, nbase::Bind(&SessionManager::OnQueryMsgCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),"");
	}

	bool SessionManager::DeleteSession(nim::NIMSessionType to_type, const std::string & id, const std::string & json_extension /*= ""*/)
	{
		return nim::Session::DeleteRecentSession(to_type, id, nbase::Bind(&SessionManager::OnSetSessionTopCb,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3),json_extension);
	}

	bool SessionManager::UpdateTeamInfoAsync(const std::string & tid, const nim::TeamInfo& team_info, TeamEventCallback cb, const std::string & json_extension /*= ""*/)
	{
		return nim::Team::UpdateTeamInfoAsync(tid, team_info, cb,json_extension);
	}

	void SessionManager::GetUserNameCard(std::list< std::string >accids, GetUserNameCardCallback cb,std::string json_extension /*= ""*/)
	{
		nim::User::GetUserNameCard(accids, cb, json_extension);
	}


	bool nim_comp::SessionManager::ParseVideoMessageAttach(const nim::IMMessage& msg, nim::IMVideo& video)
	{
		return nim::Talk::ParseVideoMessageAttach(msg, video);
	}

	bool nim_comp::SessionManager::ParseAudioMessageAttach(const nim::IMMessage& msg, nim::IMAudio& audio)
	{
		return nim::Talk::ParseAudioMessageAttach(msg, audio);
	}

	bool nim_comp::SessionManager::ParseLocationMessageAttach(const nim::IMMessage& msg, nim::IMLocation& location)
	{
		return nim::Talk::ParseLocationMessageAttach(msg, location);
	}


}

