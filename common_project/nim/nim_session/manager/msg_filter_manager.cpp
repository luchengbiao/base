#include "msg_filter_manager.h"
#include "nim_sdk_manager\helper\nim_session_helper.h"

MsgFilterManager::MsgFilterManager()
{

}

MsgFilterManager::~MsgFilterManager()
{

}

void MsgFilterManager::AddFilterAccid(std::string accid)
{
	vec_accid_.push_back(accid);
}

void MsgFilterManager::DeleteFilterAccid(std::string accid)
{
	for (auto it = vec_accid_.begin(); it != vec_accid_.end();it++)
	{
		std::string temp = (*it);
		if (temp == accid)
		{
			vec_accid_.erase(it);
		}
	}
}

UnregisterCallback MsgFilterManager::RegMsgCallBack(const MsgFilterCallback& msg_cb)
{
	MsgFilterCallback* new_callback = new MsgFilterCallback(msg_cb);
	int cb_id = (int)new_callback;
	server_system_msg_list_[cb_id].reset(new_callback);

	auto cb = ToWeakCallback([this, cb_id]() {
		server_system_msg_list_.erase(cb_id);
	});
	return cb;
}

bool MsgFilterManager::StartFilter(nim::IMMessage msg)
{
	//带callback的
	std::string accid = msg.sender_accid_;
	std::string compare_temp;
	for (auto it = vec_accid_.begin(); it != vec_accid_.end(); it++)
	{
		compare_temp = (*it);
		if (compare_temp == accid)
		{
			//TODO cb
			for (auto& it : server_system_msg_list_)
			{
				(*(it.second))(msg);
			}
			if (msg.feature_ == nim::kNIMMessageFeatureSyncMsg)
			{
				//放行同步数据
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

bool MsgFilterManager::StartFilter(std::string accid)
{
	//不带callback的
	std::string compare_temp;
	for (auto it = vec_accid_.begin(); it != vec_accid_.end(); it++)
	{
		compare_temp = (*it);
		if (compare_temp == accid)
		{
			return true;
		}
	}
	return false;
}

nim::SessionDataList MsgFilterManager::StartSessionMsgFilter(int &unread_count, nim::SessionDataList session_list)
{
	nim::SessionData temp_data;
	nim::SessionDataList result_list = session_list;
	for (auto it = result_list.sessions_.begin(); it != result_list.sessions_.end();)
	{
		temp_data = (*it);
		if (StartFilter(temp_data.id_))
		{
			unread_count -= temp_data.unread_count_;
			result_list.unread_count_ -= temp_data.unread_count_;
			result_list.count_ -= 1;
			it = result_list.sessions_.erase(it);
		}
		else
		{
			it++;
		}
	}
	return result_list;
}

// void MsgFilterManager::StartHistoryMsgFilter(nim::QueryMsglogResult &query_log_result)
// {
// 	std::list<nim::IMMessage> history_im_msg = query_log_result.msglogs_;
// 	for (auto it = history_im_msg.begin(); it != history_im_msg.end();)
// 	{
// 		nim::IMMessage im_msg = (*it);
// 		if (StartFilter(im_msg))
// 		{
// 			it = history_im_msg.erase(it);
// 			query_log_result.count_--;
// 		}
// 		else
// 		{
// 			it++;
// 		}
// 	}
// 	query_log_result.msglogs_ = history_im_msg;
// 
// }

