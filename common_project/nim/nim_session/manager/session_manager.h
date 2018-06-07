#pragma once
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include <map>
#include "nim_sdk_manager\api\nim_cpp_talk.h"
#include "nim_sdk_manager\api\nim_cpp_msglog.h"
#include "nim_sdk_manager\api\nim_cpp_sysmsg.h"
#include "nim_sdk_manager\api\nim_cpp_session.h"
#include "base\util\auto_unregister.h"
#include "nim_sdk_manager\helper\nim_team_helper.h"

typedef std::function<void(const std::string &, bool)> SessionDownloadCallback;
typedef std::function<void(nim::NIMResCode res_code, const std::string& msg_id)> ModifySingleMsglogCallback; /**< 修改（单条）消息历史回调模板 */
typedef std::function<void(nim::NIMResCode, const nim::SessionData&, int)>ChangeCallback;
typedef ModifySingleMsglogCallback WriteMsglogCallback;	/**< 保存消息历史回调模板 */
typedef std::function<void(const nim::TeamEvent& team_event)> TeamEventCallback;
typedef std::function<void(const std::list<nim::UserNameCard>)> GetUserNameCardCallback;
namespace nim_comp
{
	class SessionManager : public nbase::SupportWeakCallback
	{
	public:
		SINGLETON_DEFINE(SessionManager);
		SessionManager();
		~SessionManager();

		// reg cb
		UnregisterCallback RegSendMsgCb(const nim::Talk::SendMsgAckCallback& callback);
		UnregisterCallback RegReceiveMsgCb(const nim::Talk::ReceiveMsgCallback& callback);
		UnregisterCallback RegMessageStatusChangedCb(const nim::MsgLog::MessageStatusChangedCallback& callback);
		UnregisterCallback RegReceiveMsgsCb(const nim::Talk::ReceiveMsgsCallback& callback);
		UnregisterCallback RegSendCustomSysmsgCb(const nim::SystemMsg::SendCustomSysmsgCallback& callback);
		UnregisterCallback RegQueryMsgCb(const nim::MsgLog::QueryMsgCallback& callback);
		UnregisterCallback RegQuerySessionListCb(const nim::Session::QuerySessionListCallabck& callback);
		UnregisterCallback RegRecallMsgsCb(const nim::Talk::RecallMsgsCallback& callback);
		UnregisterCallback RegDownloadCb(const SessionDownloadCallback& callback);
		UnregisterCallback RegSessionChangeCb(const ChangeCallback& callback);

		// cb
		void OnSendMsgCallback(const nim::SendMessageArc& arc);
		void OnReceiveMsgCallback(const nim::IMMessage& message);
		void OnReceiveMsgsCallback(const std::list<nim::IMMessage>& messages);
		void OnMsgStatusChangedCallback(const nim::MessageStatusChangedResult& res);
		void OnSendCustomSysmsgCallback(const nim::SendMessageArc& arc);
		void OnQueryMsgCallback(nim::NIMResCode code, const std::string& query_id, nim::NIMSessionType query_type, const nim::QueryMsglogResult& result);
		void OnQuerySessionListCallback(int unread_count, const nim::SessionDataList& session_list);
		void OnReceiveRecallMsgCallback(nim::NIMResCode code, const std::list<nim::RecallMsgNotify>& message);
		void OnDownloadCallback(const std::string &cid, bool success);
		void SetMsgLogToLocalAsync(const std::string& talk_id, const nim::IMMessage& msg, bool need_update_session, const WriteMsglogCallback& cb, const std::string& json_extension = "");
		void OnSetSessionTopCb(nim::NIMResCode, const nim::SessionData&, int);
		

		void QueryAllRecentSessionAsync();			//消息列表
		void QueryMsgAsync(const std::string& account_id, nim::NIMSessionType to_type, int limit_count, __int64 anchor_msg_time);						//历史消息
		void QueryOnlineMsgAsync(const std::string& account_id, nim::NIMSessionType to_type, int limit_count, __int64 from_time, __int64 to_time);	//online 历史消息
		void SendMsg(const std::string& json_msg, const std::string& json_extension = "", nim::Talk::FileUpPrgCallback* pcb = nullptr);
		bool StopSendMsg(const std::string& client_msg_id, const nim::NIMMessageType& type, const std::string& json_extension = "");
		void RecallMsg(const nim::IMMessage &msg, const std::string &notify, const nim::Talk::RecallMsgsCallback& cb, const std::string& json_extension = "");

		std::string CreateTextMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id
											, const std::string& content, const nim::MessageSetting& msg_setting, int64_t timetag = 0);
		std::string CreateImageMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id,
											const nim::IMImage& image, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag = 0);
		std::string CreateFileMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id
												, const nim::IMFile& file, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag = 0);
		std::string CreateAudioMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id
												, const nim::IMAudio& audio, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag = 0);
		std::string CreateVideoMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id
												, const nim::IMVideo& video, const std::string& file_path, const nim::MessageSetting& msg_setting, int64_t timetag = 0);
		std::string CreateLocationMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id
												, const nim::IMLocation& location, const nim::MessageSetting& msg_setting, int64_t timetag = 0);
		std::string CreateTipMessage(const std::string& receiver_id, const nim::NIMSessionType session_type, const std::string& client_msg_id
												, const std::string& tip_content, const nim::MessageSetting& msg_setting, int64_t timetag = 0);

		bool ParseIMMessage(const std::string& json_msg, nim::IMMessage& msg);
		bool ParseImageMessageAttach(const nim::IMMessage& msg, nim::IMImage& image);
		bool ParseFileMessageAttach(const nim::IMMessage& msg, nim::IMFile& file);
		bool ParseAudioMessageAttach(const nim::IMMessage& msg, nim::IMAudio& audio);
		bool ParseVideoMessageAttach(const nim::IMMessage& msg, nim::IMVideo& video);
		bool ParseLocationMessageAttach(const nim::IMMessage& msg, nim::IMLocation& location);

		bool CheckIfTimeVisible(const long long old_timestamp, const long long new_timestamp);
		bool SetSessionTop(enum nim::NIMSessionType to_type, const std::string &id, bool top, const std::string &json_extension = "");
		bool SetUnreadCountZeroAsync(nim::NIMSessionType to_type, const std::string & id, const std::string & json_extension = "");
		bool DeleteSession(nim::NIMSessionType to_type, const std::string & id, const std::string & json_extension = "");
		bool UpdateTeamInfoAsync(const std::string & tid, const nim::TeamInfo& team_info,TeamEventCallback cb, const std::string & json_extension = "");

		void GetUserNameCard(std::list< std::string > accids,GetUserNameCardCallback cb,std::string json_extension = "");

	private:
		std::map<int, std::unique_ptr<nim::Talk::SendMsgAckCallback>> send_msg_ack_cb_list_;
		std::map<int, std::unique_ptr<nim::Talk::ReceiveMsgCallback>> receive_msg_cb_list_;
		std::map<int, std::unique_ptr<nim::MsgLog::MessageStatusChangedCallback>> message_status_changed_cb_list_;
		std::map<int, std::unique_ptr<nim::Talk::ReceiveMsgsCallback>> receive_msgs_cb_list_;
		std::map<int, std::unique_ptr<nim::SystemMsg::SendCustomSysmsgCallback>> send_custom_sysmsg_cb_list_;
		std::map<int, std::unique_ptr<nim::MsgLog::QueryMsgCallback>> query_msg_cb_list_;
		std::map<int, std::unique_ptr<nim::Session::QuerySessionListCallabck>> query_session_list_cb_list_;
		std::map<int, std::unique_ptr<nim::Talk::RecallMsgsCallback>> recall_msgs_cb_list_;
		std::map<int, std::unique_ptr<SessionDownloadCallback>> download_cb_list_;
		std::map<int, std::unique_ptr<ChangeCallback>> change_cb_list_;
	};
}