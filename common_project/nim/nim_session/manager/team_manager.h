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
	* @brief Ⱥ����ط���
	* @copyright (c) 2016, NetEase Inc. All rights reserved

	* @date 2016/09/14
	*/
	class TeamManager : public nbase::SupportWeakCallback
	{
	public:
		SINGLETON_DEFINE(TeamManager);
		TeamManager() {}
	public:

		// ע��
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
		* ��ȡ���ػ���õ�����Ⱥ����Ϣ
		* @return std::list<nim::TeamInfo> Ⱥ��Ϣ
		*/
		std::list<nim::TeamInfo> GetCachedTeamsInfo();

		/**
		* ��ȡȺ����
		* @param[in] tid Ⱥ��id
		* @return wstring Ⱥ����
		*/
		std::wstring GetTeamName(const std::string& tid);

		/**
		* ��ȡȺͷ��
		* @param[in] tid Ⱥ��id
		* @param[out] icon Ⱥͷ��
		* @return bool	true ��ȡ�ɹ���false ��ȡʧ��
		*/
		bool GetTeamIcon(const std::string& tid, std::string& icon);

		/**
		* ��ȡȺ����
		* @param[in] tid Ⱥ��id
		* @return int	0: ��ͨȺ, 1: �߼�Ⱥ, -1: ��ȡ����
		*/
		int GetTeamType(const std::string& tid);

		/**
		* ��ѯȺ��Ϣ
		* @param[in] tid Ⱥ��id
		* @param[in] cb �ص�����
		* @return void	�޷���ֵ
		*/
		void QueryTeamInfo(const std::string& tid, OnGetTeamInfo cb);

		/**
		* �ӱ������ݿ��ȡ����Ⱥ��Ϣ
		* @return void	�޷���ֵ
		*/
		void QueryAllTeamInfo();

		/**
		* ��ȡ����Ⱥ��Ϣ�Ļص�����
		* @param[in] team_count Ⱥ������
		* @param[in] team_info_list Ⱥ��Ϣ�б�
		* @return void	�޷���ֵ
		*/
		void QueryAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list);

		/**
		* ��ȡ����Ⱥ��Ϣ�Ļص�����ת����UI�߳�
		* @param[in] team_count Ⱥ������
		* @param[in] team_info_list Ⱥ��Ϣ�б�
		* @return void	�޷���ֵ
		*/
		void UIQueryAllTeamInfoCb(int team_count, const std::list<nim::TeamInfo>& team_info_list);

		/**
		* �ӷ�������ȡȺ��Ϣ
		* @param[in] tid Ⱥ��id
		* @return void	�޷���ֵ
		*/
		void GetTeamInfo(const std::string& tid);

		/**
		* �ӷ�������ȡȺ��Ϣ�Ļص�����
		* @param[in] team_event Ⱥ���¼�֪ͨ
		* @return void	�޷���ֵ
		*/
		void GetTeamInfoCb(const nim::TeamEvent& team_event);

		/**
		* �ӷ�������ѯȺ��Ϣ�Ļص�����
		* @param[in] tid Ⱥ��id
		* @param[in] result Ⱥ��Ϣ
		* @return void	�޷���ֵ
		*/
		void GetLocalTeamInfoCb(const std::string& tid, const nim::TeamInfo& result);

		/**
		* �ӷ�������ѯȺ��Ϣ�Ļص�����ת����UI�߳�
		* @param[in] tid Ⱥ��id
		* @param[in] result Ⱥ��Ϣ
		* @return void	�޷���ֵ
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
		std::set<std::string> on_query_tids_; //���ڲ�ѯ��Ⱥ��Ϣ������û���ص�Ⱥid
	};
}