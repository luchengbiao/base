#pragma once
#include "base\memory\singleton.h"
#include <functional>
#include "nim_sdk_manager\helper\nim_session_helper.h"
#include "nim_sdk_manager\helper\nim_talk_helper.h"
#include "base\util\auto_unregister.h"
#include <memory>
#include <map>
#include <vector>
#include "base\callback\callback.h"

using MsgFilterCallback = std::function<void(nim::IMMessage msg) >;
class MsgFilterManager : public nbase::SupportWeakCallback
{
public:
	SINGLETON_DEFINE(MsgFilterManager);
	MsgFilterManager();
	~MsgFilterManager();
	void AddFilterAccid(std::string accid);
	void DeleteFilterAccid(std::string accid);
	UnregisterCallback RegMsgCallBack(const MsgFilterCallback& msg_cb);
	//TODO:
	bool StartFilter(nim::IMMessage);	//true为被过滤，false为不被过滤
	bool StartFilter(std::string accid);
	//void StartHistoryMsgFilter(nim::QueryMsglogResult &msg_log_result);
	nim::SessionDataList StartSessionMsgFilter(int &unread_count, nim::SessionDataList session_list);


private:
	std::vector<std::string> vec_accid_;
	std::map<int, std::unique_ptr<MsgFilterCallback>>	server_system_msg_list_;
};