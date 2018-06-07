#include "log_manager\log.h"
#include "base\util\string_util.h"
#include "nim_sdk_manager\api\nim_cpp_team.h"
#include "nim_login\manager\login_manager.h"
#include "qthread_manager\closure.h"
#include "team_manager.h"


namespace nim_comp
{
	UnregisterCallback TeamManager::RegAddTeam(OnTeamAdd add)
	{
		OnTeamAdd* new_add = new OnTeamAdd(add);
		int cb_id = (int)new_add;
		add_team_cb_[cb_id].reset(new_add);
		auto unregister = ToWeakCallback([this, cb_id]() {
			add_team_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnAddTeamCallback(const std::string & tid, const nim::TeamInfo & tinfo)
	{
		QLOG_APP(L"InvokeAddTeam: tid={0} ") << tid;
		cached_tinfo_[tid] = tinfo;
		for (auto& it : add_team_cb_)
		{
			if (tinfo.IsValid() && tinfo.IsMemberValid())
				(*it.second)(tid, tinfo.GetName(), tinfo.GetType());
		}
	}

	UnregisterCallback TeamManager::RegRemoveTeam(OnTeamRemove remove)
	{
		OnTeamRemove* new_cb = new OnTeamRemove(remove);
		int cb_id = (int)new_cb;
		remove_team_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			remove_team_cb_.erase(cb_id);
		});
		return unregister;
	}

	UnregisterCallback TeamManager::RegGetAllTeamInfoCb(OnQueryTeamInfo cb)
	{
		OnQueryTeamInfo* new_add = new OnQueryTeamInfo(cb);
		int cb_id = (int)new_add;
		query_team_info_cb_[cb_id].reset(new_add);
		auto unregister = ToWeakCallback([this, cb_id]() {
			query_team_info_cb_.erase(cb_id);
		});
		return unregister;
	}

	UnregisterCallback TeamManager::RegQueryAllTeamMembers(OnQueryTeamMembersCb cb)
	{
		OnQueryTeamMembersCb* new_add = new OnQueryTeamMembersCb(cb);
		int cb_id = (int)new_add;
		query_team_members_cb_[cb_id].reset(new_add);
		auto unregister = ToWeakCallback([this, cb_id]() {
			query_team_members_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnRemoveTeamCallback(const std::string& tid)
	{
		QLOG_APP(L"InvokeRemoveTeam: tid={0} ") << tid;

		cached_tinfo_.erase(tid);

		for (auto& it : remove_team_cb_)
		{
			(*it.second)(tid);
		}
	}

	UnregisterCallback TeamManager::RegChangeTeamName(OnTeamNameChange change)
	{
		OnTeamNameChange* new_cb = new OnTeamNameChange(change);
		int cb_id = (int)new_cb;
		change_team_name_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			change_team_name_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnChangeTeamNameCallback(const nim::TeamInfo& team_info)
	{
		for (auto& it : change_team_name_cb_)
		{
			(*it.second)(team_info);
		}
	}

	UnregisterCallback TeamManager::RegAddTeamMember(OnTeamMemberAdd add)
	{
		OnTeamMemberAdd* new_cb = new OnTeamMemberAdd(add);
		int cb_id = (int)new_cb;
		add_team_member_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			add_team_member_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnAddTeamMemberCallback(const std::string& tid, const nim::TeamMemberProperty& team_member)
	{
		QLOG_APP(L"invoke add: tid={0} uid={1}") << tid << team_member.GetAccountID();
		for (auto& it : add_team_member_cb_)
		{
			(*it.second)(tid, team_member);
		}
	}

	UnregisterCallback TeamManager::RegRemoveTeamMember(OnTeamMemberRemove remove)
	{
		OnTeamMemberRemove* new_cb = new OnTeamMemberRemove(remove);
		int cb_id = (int)new_cb;
		remove_team_member_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			remove_team_member_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnRemoveTeamMemberCallback(const std::string& tid, const std::string& uid)
	{
		QLOG_APP(L"invoke remove: tid={0} uid={1}") << tid << uid;

		if (LoginManager::GetInstance()->IsEqual(uid))
		{
			cached_tinfo_.erase(tid);
		}

		for (auto& it : remove_team_member_cb_)
		{
			(*it.second)(tid, uid);
		}
	}

	UnregisterCallback TeamManager::RegChangeTeamMember(OnTeamMemberChange change)
	{
		OnTeamMemberChange* new_cb = new OnTeamMemberChange(change);
		int cb_id = (int)new_cb;
		change_team_member_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			change_team_member_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnChangeTeamMemberCallback(const std::string& tid, const std::string& uid, const std::string& team_card)
	{
		QLOG_APP(L"invoke change: tid={0} uid={1} team_card={2}") << tid << uid << team_card;

		for (auto& it : change_team_member_cb_)
		{
			std::string new_team_card = team_card;
			// 如果new_team_card为空，就取User名称

			(*it.second)(tid + "#" + uid, new_team_card);
		}
	}

	UnregisterCallback TeamManager::RegSetTeamAdmin(OnTeamAdminSet admin)
	{
		OnTeamAdminSet* new_cb = new OnTeamAdminSet(admin);
		int cb_id = (int)new_cb;
		change_team_admin_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			change_team_admin_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnChangeTeamAdminCallback(const std::string& tid, const std::string& uid, bool admin)
	{
		QLOG_APP(L"invoke admin: tid={0} uid={1} admin={2}") << tid << uid << admin;
		for (auto& it : change_team_admin_cb_)
		{
			(*it.second)(tid, uid, admin);
		}
	}

	UnregisterCallback TeamManager::RegChangeTeamOwner(OnTeamOwnerChange set_team_owner)
	{
		OnTeamOwnerChange* new_cb = new OnTeamOwnerChange(set_team_owner);
		int cb_id = (int)new_cb;
		set_team_owner_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			set_team_owner_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnSetTeamOwnerCallback(const std::string& tid, const std::string& uid)
	{
		QLOG_APP(L"invoke set team owner: tid={0} uid={1}") << tid << uid;
		for (auto& it : set_team_owner_cb_)
		{
			(*it.second)(tid, uid);
		}
	}

	UnregisterCallback TeamManager::RegMuteMember(OnMuteMember mute)
	{
		OnMuteMember* new_cb = new OnMuteMember(mute);
		int cb_id = (int)new_cb;
		mute_member_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			mute_member_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnMuteMemberCallback(const std::string& tid, const std::string& uid, bool set_mute)
	{
		QLOG_APP(L"invoke mute member: tid={0} uid={1} set_mute={2}") << tid << uid << set_mute;

		for (auto& it : mute_member_cb_)
		{
			(*it.second)(tid, uid, set_mute);
		}
	}

	void TeamManager::OnTeamDataSyncCallback(nim::NIMDataSyncType sync_type, nim::NIMDataSyncStatus status, const std::string &data_sync_info)
	{
		if (sync_type == nim::kNIMDataSyncTypeTeamInfo)
		{
			std::list<nim::TeamInfo> tinfos;
			ParseTeamInfosJson(data_sync_info, tinfos);
			for (auto& tinfo : tinfos)
			{
				UIGetLocalTeamInfoCb(tinfo.GetTeamID(), tinfo);
			}
		}
	}

	std::list<nim::TeamInfo> TeamManager::GetCachedTeamsInfo()
	{
		std::list<nim::TeamInfo> tinfos;
		for (auto& tinfo : cached_tinfo_)
		{
			tinfos.push_back(tinfo.second);
		}
		return tinfos;
	}

	void TeamManager::OnAddTeamMember(const std::string& tid, const std::list<std::string>& ids, const std::string& invitation_postscript, const std::string& json_extension/* = ""*/)
	{
		nim::Team::InviteAsync(tid, ids, invitation_postscript, nbase::Bind(&TeamManager::OnTeamEventCallback, this, std::placeholders::_1),json_extension);
	}


	std::wstring TeamManager::GetTeamName(const std::string& tid)
	{
		std::wstring tname;

		auto it = cached_tinfo_.find(tid);
		if (it == cached_tinfo_.end())
		{
			tname = nbase::UTF8ToUTF16(tid);
			GetTeamInfo(tid);
		}
		else
		{
			tname = nbase::UTF8ToUTF16(it->second.GetName());
		}

		if (tname.empty())
			tname = nbase::UTF8ToUTF16(tid);

		return tname;
	}

	bool TeamManager::GetTeamIcon(const std::string& tid, std::string& icon)
	{
		auto it = cached_tinfo_.find(tid);
		if (it == cached_tinfo_.end())
		{
			GetTeamInfo(tid);
			return false;
		}
		else
		{
			icon = it->second.GetIcon();
		}

		return true;
	}

	int TeamManager::GetTeamType(const std::string& tid)
	{
		int type = -1;
		auto it = cached_tinfo_.find(tid);
		if (it != cached_tinfo_.end())
			type = it->second.GetType();

		return type;
	}

	void TeamManager::QueryTeamInfo(const std::string & tid, OnGetTeamInfo cb)
	{
		nim::Team::QueryTeamInfoAsync(tid, ToWeakCallback([cb](const std::string& tid, const nim::TeamInfo& result)
		{
			cb(result);
		}));
	}

	void TeamManager::QueryAllTeamInfo()
	{
		nim::Team::QueryAllMyTeamsInfoAsync(nbase::Bind(&TeamManager::QueryAllTeamInfoCb, this, std::placeholders::_1, std::placeholders::_2));
	}

	void TeamManager::QueryAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list)
	{
		auto task = nbase::Bind(&TeamManager::UIQueryAllTeamInfoCb, this, team_count, team_info_list);
		qtbase::Post2Task(kThreadUIHelper, task);
	}

	void TeamManager::UIQueryAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list)
	{
		for (auto& team_info : team_info_list)
		{
			cached_tinfo_[team_info.GetTeamID()] = team_info;

			if (!team_info.GetName().empty()) {
				OnChangeTeamNameCallback(team_info);
			}
			if (!team_info.GetIcon().empty()) {
				//PhotoService::GetInstance()->DownloadTeamIcon(team_info);
				// 图片不存在下载图片
			}
		}
		GetAllTeamInfoCb(team_count,team_info_list);

	}

	void TeamManager::OnQueryAllTeamMembers(const std::string &tid, OnQueryTeamMembersCb cb,const std::string &json_extension /*= ""*/)
	{
		nim::Team::QueryTeamMembersAsync(tid, cb, json_extension);
	}

	void TeamManager::GetTeamInfo(const std::string& tid)
	{
		if (on_query_tids_.find(tid) != on_query_tids_.cend())
			return;
		on_query_tids_.insert(tid);

		nim::Team::QueryTeamInfoOnlineAsync(tid, nbase::Bind(&TeamManager::GetTeamInfoCb, this, std::placeholders::_1));
	}

	void TeamManager::GetTeamInfoCb(const nim::TeamEvent& team_event)
	{
		if (team_event.res_code_ == nim::kNIMResSuccess)
		{
			nim::Team::QueryTeamInfoAsync(team_event.team_id_, nbase::Bind(&TeamManager::GetLocalTeamInfoCb, this, std::placeholders::_1, std::placeholders::_2));
		}
	}

	void TeamManager::GetLocalTeamInfoCb(const std::string& tid, const nim::TeamInfo& result)
	{
		auto task = nbase::Bind(&TeamManager::UIGetLocalTeamInfoCb, this, tid, result);
		qtbase::Post2Task(kThreadUIHelper, task);
	}

	void TeamManager::UIGetLocalTeamInfoCb(const std::string& tid, const nim::TeamInfo& result)
	{
		if (cached_tinfo_.find(tid) == cached_tinfo_.end())
		{
			OnAddTeamCallback(tid, result);
		}
		else
		{
			if (!result.GetName().empty()) {
				OnChangeTeamNameCallback(result);
			}
			if (!result.GetIcon().empty()) {
				// 图片不存在下载图片
				//PhotoService::GetInstance()->DownloadTeamIcon(result);
			}
		}
		cached_tinfo_[tid] = result;  
		on_query_tids_.erase(tid); //已经查到，从post_tids_中删除
	}

	UnregisterCallback TeamManager::RegQueryTeamEventInfo(OnQueryTeamEventInfo cb)
	{
		OnQueryTeamEventInfo* new_cb = new OnQueryTeamEventInfo(cb);
		int cb_id = (int)new_cb;
		query_team_event_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			query_team_event_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnUITeamEventCallback(const nim::TeamEvent& info, const std::string& tid, const nim::TeamInfo& team_info)
	{
		for (auto& it : query_team_event_cb_)
		{
			(*it.second)(info, tid, team_info);
		}
	}

	UnregisterCallback TeamManager::RegTeamEvent(OnTeamEvent cb)
	{
		OnTeamEvent* new_cb = new OnTeamEvent(cb);
		int cb_id = (int)new_cb;
		team_event_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			team_event_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnTeamEventCallback(const nim::TeamEvent& result)
	{
		for (auto& it : team_event_cb_)
		{
			(*it.second)(result);
		}
	}

	UnregisterCallback TeamManager::RegReceiveSysmsg(OnReceiveSysmsg cb)
	{
		OnReceiveSysmsg* new_cb = new OnReceiveSysmsg(cb);
		int cb_id = (int)new_cb;
		receive_sysmsg_cb_[cb_id].reset(new_cb);
		auto unregister = ToWeakCallback([this, cb_id]() {
			receive_sysmsg_cb_.erase(cb_id);
		});
		return unregister;
	}

	void TeamManager::OnReceiveSysmsgCallback(const nim::SysMessage& msg)
	{
		for (auto& it : receive_sysmsg_cb_)
		{
			(*it.second)(msg);
		}
	}

	void TeamManager::GetAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list)
	{
		for (auto& it : query_team_info_cb_)
		{
			(*it.second)(team_count, team_info_list);
		}
	}

	void TeamManager::GetTeamMembersInfoCb(const std::string& tid, int member_count, const std::list<nim::TeamMemberProperty>& props)
	{
		for (auto& it : query_team_members_cb_)
		{
			(*it.second)(tid, member_count, props);
		}
	}

	void TeamManager::OnLeaveTeam(const std::string& tid,const std::string& json_extension/* = ""*/)
	{
		nim::Team::LeaveAsync(tid, nbase::Bind(&TeamManager::OnTeamEventCallback,this,std::placeholders::_1), json_extension);
	}

	void TeamManager::OnDismissTeam(const std::string& tid, const std::string& json_extension /*= ""*/)
	{
		nim::Team::DismissAsync(tid, nbase::Bind(&TeamManager::OnTeamEventCallback,this,std::placeholders::_1),json_extension);
	}



}