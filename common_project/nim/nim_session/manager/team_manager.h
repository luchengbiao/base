#pragma once
#include <functional>
#include "nim_sdk_manager\helper\nim_team_helper.h"
#include <string>
#include "base\callback\callback.h"
#include "base\memory\singleton.h"
#include "base\util\auto_unregister.h"
#include "nim_sdk_manager\api\nim_cpp_data_sync.h"
#include <list>
#include <set>
#include <map>
#include "nim_sdk_manager\helper\nim_sysmsg_helper.h"


//tid, tname
typedef std::function<void(const std::string&, const std::string&, nim::NIMTeamType)> OnTeamAdd;
typedef std::function<void(const std::string&)> OnTeamRemove;
typedef std::function<void(const nim::TeamInfo& team_info)> OnTeamNameChange;

//tid,uid
typedef std::function<void(const std::string&, const nim::TeamMemberProperty&)> OnTeamMemberAdd;
typedef std::function<void(const std::string&, const std::string&)> OnTeamMemberRemove;
typedef std::function<void(const std::string&, const std::string&)> OnTeamMemberChange;

//tid,uid,admin
typedef std::function<void(const std::string&, const std::string&, bool)> OnTeamAdminSet;
typedef std::function<void(const std::string&, const std::string&)> OnTeamOwnerChange;

typedef std::function<void(const nim::TeamInfo& team_info)> OnGetTeamInfo;
typedef std::function<void(const nim::TeamEvent& , const std::string&, const nim::TeamInfo& )> OnQueryTeamEventInfo;
typedef std::function<void(const nim::TeamEvent& )> OnTeamEvent;
typedef std::function<void(const nim::SysMessage&)> OnReceiveSysmsg;
typedef std::function<void(int team_count, const std::list<nim::TeamInfo>& team_info_list)> OnQueryTeamInfo;
typedef std::function<void(const std::string& tid, int member_count, const std::list<nim::TeamMemberProperty>& props)> OnQueryTeamMembersCb;
typedef std::function<void(const std::string& tid, const nim::TeamInfo& result)> QueryTeamInfoCallback;

typedef OnTeamAdminSet OnMuteMember;

namespace nim_comp
{
	/** @class TeamService
	* @brief 群组相关服务
	* @copyright (c) 2016, NetEase Inc. All rights reserved

	* @date 2016/09/14
	*/
	class TeamManager : public nbase::SupportWeakCallback
	{
	public:
		SINGLETON_DEFINE(TeamManager);
		TeamManager() {}
	public:

		// 注册
		UnregisterCallback RegAddTeam(OnTeamAdd add);
		UnregisterCallback RegRemoveTeam(OnTeamRemove remove);
		UnregisterCallback RegChangeTeamName(OnTeamNameChange change);
		UnregisterCallback RegAddTeamMember(OnTeamMemberAdd add);
		UnregisterCallback RegRemoveTeamMember(OnTeamMemberRemove remove);
		UnregisterCallback RegChangeTeamMember(OnTeamMemberChange change);
		UnregisterCallback RegSetTeamAdmin(OnTeamAdminSet admin);
		UnregisterCallback RegChangeTeamOwner(OnTeamOwnerChange set_team_owner);
		UnregisterCallback RegMuteMember(OnMuteMember mute);
		UnregisterCallback RegQueryTeamEventInfo(OnQueryTeamEventInfo cb);
		UnregisterCallback RegTeamEvent(OnTeamEvent cb);
		UnregisterCallback RegReceiveSysmsg(OnReceiveSysmsg cb);
		UnregisterCallback RegGetAllTeamInfoCb(OnQueryTeamInfo cb);
		UnregisterCallback RegQueryAllTeamMembers(OnQueryTeamMembersCb cb);



		// callback
		void OnAddTeamCallback(const std::string& tid, const nim::TeamInfo& tinfo);
		void OnRemoveTeamCallback(const std::string& tid);
		void OnChangeTeamNameCallback(const nim::TeamInfo& team_info);
		void OnAddTeamMemberCallback(const std::string& tid, const nim::TeamMemberProperty& team_member);
		void OnRemoveTeamMemberCallback(const std::string& tid, const std::string& uid);
		void OnChangeTeamMemberCallback(const std::string& tid, const std::string& uid, const std::string& team_card);
		void OnChangeTeamAdminCallback(const std::string& tid, const std::string& uid, bool admin);
		void OnSetTeamOwnerCallback(const std::string& tid, const std::string& uid);
		void OnMuteMemberCallback(const std::string& tid, const std::string& uid, bool set_mute);
		void OnTeamDataSyncCallback(nim::NIMDataSyncType sync_type, nim::NIMDataSyncStatus status, const std::string &data_sync_info);
		void OnUITeamEventCallback(const nim::TeamEvent& info, const std::string& tid, const nim::TeamInfo& team_info);
		void OnTeamEventCallback(const nim::TeamEvent& result);
		void OnReceiveSysmsgCallback(const nim::SysMessage& msg);
		void OnQueryAllTeamMembers(const std::string &tid, OnQueryTeamMembersCb cb, const std::string &json_extension = "");

		/*query*/
		void OnAddTeamMember(const std::string& tid, const std::list<std::string>& ids, const std::string& invitation_postscript, const std::string& json_extension = "");
		void OnLeaveTeam(const std::string& tid, const std::string& json_extension = "");
		void OnDismissTeam(const std::string& tid, const std::string& json_extension = "");
		
		/**
		* 获取本地缓存好的所有群的信息
		* @return std::list<nim::TeamInfo> 群信息
		*/
		std::list<nim::TeamInfo> GetCachedTeamsInfo();

		/**
		* 获取群名称
		* @param[in] tid 群组id
		* @return wstring 群名称
		*/
		std::wstring GetTeamName(const std::string& tid);

		/**
		* 获取群头像
		* @param[in] tid 群组id
		* @param[out] icon 群头像
		* @return bool	true 获取成功，false 获取失败
		*/
		bool GetTeamIcon(const std::string& tid, std::string& icon);

		/**
		* 获取群类型
		* @param[in] tid 群组id
		* @return int	0: 普通群, 1: 高级群, -1: 获取不到
		*/
		int GetTeamType(const std::string& tid);

		/**
		* 查询群信息
		* @param[in] tid 群组id
		* @param[in] cb 回调函数
		* @return void	无返回值
		*/
		void QueryTeamInfo(const std::string& tid, OnGetTeamInfo cb);

		/**
		* 从本地数据库获取所有群信息
		* @return void	无返回值
		*/
		void QueryAllTeamInfo();

		/**
		* 获取所有群信息的回调函数
		* @param[in] team_count 群组数量
		* @param[in] team_info_list 群信息列表
		* @return void	无返回值
		*/
		void QueryAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list);

		/**
		* 获取所有群信息的回调函数转发到UI线程
		* @param[in] team_count 群组数量
		* @param[in] team_info_list 群信息列表
		* @return void	无返回值
		*/
		void UIQueryAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list);

		/**
		* 从服务器获取群信息
		* @param[in] tid 群组id
		* @return void	无返回值
		*/
		void GetTeamInfo(const std::string& tid);

		/**
		* 从服务器获取群信息的回调函数
		* @param[in] team_event 群组事件通知
		* @return void	无返回值
		*/
		void GetTeamInfoCb(const nim::TeamEvent& team_event);

		/**
		* 从服务器查询群信息的回调函数
		* @param[in] tid 群组id
		* @param[in] result 群信息
		* @return void	无返回值
		*/
		void GetLocalTeamInfoCb(const std::string& tid, const nim::TeamInfo& result);

		/**
		* 从服务器查询群信息的回调函数转发到UI线程
		* @param[in] tid 群组id
		* @param[in] result 群信息
		* @return void	无返回值
		*/
		


		void UIGetLocalTeamInfoCb(const std::string& tid, const nim::TeamInfo& result);

	private:

		

		
		void GetAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list);
		void GetTeamMembersInfoCb(const std::string& tid, int member_count, const std::list<nim::TeamMemberProperty>& props);

	private:
		std::map<int, std::unique_ptr<OnTeamAdd>>			add_team_cb_;
		std::map<int, std::unique_ptr<OnTeamRemove>>		remove_team_cb_;
		std::map<int, std::unique_ptr<OnTeamMemberAdd>>		add_team_member_cb_;
		std::map<int, std::unique_ptr<OnTeamMemberRemove>>	remove_team_member_cb_;
		std::map<int, std::unique_ptr<OnTeamMemberChange>>	change_team_member_cb_;
		std::map<int, std::unique_ptr<OnTeamNameChange>>	change_team_name_cb_;
		std::map<int, std::unique_ptr<OnTeamAdminSet>>		change_team_admin_cb_;
		std::map<int, std::unique_ptr<OnTeamOwnerChange>>	set_team_owner_cb_;
		std::map<int, std::unique_ptr<OnMuteMember>>		mute_member_cb_;

		std::map<int, std::unique_ptr<OnQueryTeamEventInfo>>		query_team_event_cb_;
		std::map<int, std::unique_ptr<OnTeamEvent>>		team_event_cb_;
		std::map<int, std::unique_ptr<OnReceiveSysmsg>>		receive_sysmsg_cb_;
		std::map<int, std::unique_ptr<OnQueryTeamInfo>>		query_team_info_cb_;
		std::map<int, std::unique_ptr<OnQueryTeamMembersCb>> query_team_members_cb_;

		std::map<std::string, nim::TeamInfo> cached_tinfo_;
		std::set<std::string> on_query_tids_; //正在查询其群信息，但还没返回的群id
	};
}