#include "team_callback.h"
#include "log_manager\log.h"
#include "nim_sdk_manager\api\nim_cpp_team.h"
#include "base\callback\callback.h"
#include "manager\team_manager.h"


namespace nim_comp
{

void TeamCallback::UITeamEventCallback(const nim::TeamEvent& info, const std::string& tid, const nim::TeamInfo& team_info)
{
	QLOG_APP(L"TeamEvent: notify_id={0} tid={1} code={2}") << info.notification_id_ << tid << info.res_code_;

	TeamManager::GetInstance()->OnUITeamEventCallback(info, tid, team_info);
}

void TeamCallback::OnTeamEventCallback(const nim::TeamEvent& result)
{
	nim::Team::QueryTeamInfoAsync(result.team_id_, nbase::Bind(&TeamCallback::UITeamEventCallback, result, std::placeholders::_1, std::placeholders::_2));
	QLOG_APP(L"OnTeamEventCallback: notify_id={0} code={1} tid={2}") <<result.notification_id_ <<result.res_code_ <<result.team_id_;
}

void TeamCallback::OnReceiveSysmsgCallback(const nim::SysMessage& msg)
{
	QLOG_PRO(L"OnReceiveSysmsgCallback: content: {0} - attach : {1}") << msg.content_ << msg.attach_;
	TeamManager::GetInstance()->OnReceiveSysmsgCallback(msg);
}

}