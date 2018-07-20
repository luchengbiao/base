#include "class_manager.h"
#include "channel_manager\channel_manager.h"
#include "invite_manager\invite_manager.h"
#include "base\util\string_util.h"
#include "qthread_manager\closure.h"
#include "base\util\string_number_conversions.h"
#include "common\system\system_tool.h"
#include "QtGui\QColor"
#include "log\log.h"

ClassManager::ClassManager()
{
	cur_buffer_ = "";
	OnRegisterCallBack();
}

ClassManager::~ClassManager()
{

}

void ClassManager::OnNimControlNotifyCallback(const std::string& session_id, const std::string& info, const std::string& uid)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel && session_id == data_channel->GetSessionOrChannelID())
	{
		emit SignalNimControlNotifyCallback(session_id, info, uid);
	}
}

void ClassManager::OnNimMemberNotifyCallback(const std::string& session_id, int channel_type, const std::string& uid, int code)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel && session_id == data_channel->GetSessionOrChannelID())
	{
		emit SignalNimMemberNotifyCallback(session_id, channel_type, uid, code);
	}
}

void ClassManager::OnNimHangupNotifyCallback(const std::string& session_id, const std::string& uid)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel && session_id == data_channel->GetSessionOrChannelID())
	{
		emit SignalNimHangupNotifyCallback(session_id, uid);
	}
}

void ClassManager::OnNimRecDataCallback(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel && session_id == data_channel->GetSessionOrChannelID())
	{
		emit SignalNimRecDataCallback(session_id, channel_type, uid, data);
	}
}

void ClassManager::OnNimConnectNotifyCallback(const std::string& session_id, int channel_type, int code, const std::string& json)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel && session_id == data_channel->GetSessionOrChannelID())
	{
		emit SignalNimConnectNotifyCallback(session_id, channel_type, code, json);
	}
}

void ClassManager::OnAgoraMessageChannelReceive(const std::string s_channel_id, const std::string s_account, uint32_t uid, const std::string s_msg)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel&&data_channel->GetSessionOrChannelID() == s_channel_id)
	{
		emit SignalAgoraMessageChannelReceive(s_channel_id, s_account, uid, s_msg);
	}
}

void ClassManager::OnAgoraSignallingChannelJoinCb(std::string s_channel_id)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel &&data_channel->GetSessionOrChannelID() == s_channel_id)
	{
		emit SignalAgoraSignallingChannelJoinCb(s_channel_id);
	}
}

void ClassManager::OnAgoraSignallingChannelLeavedCb(std::string s_channel_id, int code)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel &&data_channel->GetSessionOrChannelID() == s_channel_id)
	{
		emit SignalAgoraSignallingChannelLeavedCb(s_channel_id, code);
	}
}

void ClassManager::OnAgoraSignallingChannelJoinFailed(const std::string s_channel_id, int error)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel&& data_channel->GetSessionOrChannelID() == s_channel_id)
	{
		emit SignalAgoraSignallingChannelJoinFailed(s_channel_id, error);
	}
}

void ClassManager::OnAgoraSignallingChannelInviteEnd(const std::string s_channel_id, const std::string s_account, uint32_t uid, const std::string s_extra)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel &&data_channel->GetSessionOrChannelID() == s_channel_id)
	{
		emit SignalAgoraSignallingChannelInviteEnd(s_channel_id, s_account, uid, s_extra);
	}
}

void ClassManager::OnAgoraSignallingChannelUserLeaved(const std::string s_account, uint32_t uid)
{
	emit SignalAgoraSignallingChannelUserLeaved(s_account, uid);
}

void ClassManager::OnAgoraSignallingLogout(int ecode)
{
	emit SignalAgoraSignallingLogout(ecode);
}

void ClassManager::OnAgoraSignallingMessageInstantReceive(const std::string s_account, uint32_t uid, const std::string s_msg)
{
	emit SignalAgoraSignallingMessageInstantReceive(s_account, uid, s_msg);
}

void ClassManager::OnAgoraMediaChannelLost()
{
	emit SignalAgoraMediaChannelLost();
}

void ClassManager::OnAgoraRejoinMediaChannelSuccess(const std::string s_channel_id, uint32_t uid, int elapsed)
{
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (data_channel &&data_channel->GetSessionOrChannelID() == s_channel_id)
	{
		emit SignalAgoraRejoinMediaChannelSuccess(s_channel_id, uid, elapsed);
	}
}

void ClassManager::OnAgoraMediaChannelError(bool bIgnore, int error)
{
	emit SignalAgoraMediaChannelError(bIgnore, error);
}

void ClassManager::OnAgoraMediaChannelUserOffline(uint32_t uid)
{
	emit SignalAgoraMediaChannelUserOffline(uid);
}

void ClassManager::OnRegisterCallBack()
{
	OnRegisterAgoraCallBack();
	OnRegisterNimCallBack();
}

void ClassManager::OnRegisterAgoraCallBack()
{
	auto message_receive_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraMessageChannelReceive, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	auto signal_join_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingChannelJoinCb, this, std::placeholders::_1));
	auto signal_leave_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingChannelLeavedCb, this, std::placeholders::_1, std::placeholders::_2));
	auto signal_join_fail_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingChannelJoinFailed, this, std::placeholders::_1, std::placeholders::_2));
	//auto end_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingChannelInviteEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	auto user_leave_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingChannelUserLeaved, this, std::placeholders::_1, std::placeholders::_2));
	auto signal_logout_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingLogout, this, std::placeholders::_1));
	auto message_instant_receive_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraSignallingMessageInstantReceive, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	auto chat_connect_lost_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraMediaChannelLost, this));
	auto chat_rejoin_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraRejoinMediaChannelSuccess, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	auto chat_user_offline_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraMediaChannelUserOffline, this, std::placeholders::_1));
	auto data_message_err_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnAgoraDataMsgSendError, this, std::placeholders::_1,std::placeholders::_2));

	InviteManager::GetInstance()->RegAgoraDataMsgChannelRecCb(message_receive_cb);
	InviteManager::GetInstance()->RegAgoraDataMsgInstantRecCb(message_instant_receive_cb);
	InviteManager::GetInstance()->RegAgoraDataChannelJoinCb(signal_join_cb);
	InviteManager::GetInstance()->RegAgoraDataChannelLeavedCb(signal_leave_cb);
	InviteManager::GetInstance()->RegAgoraDataChannelJoinFailCb(signal_join_fail_cb);
	//InviteManager::GetInstance()->RegAgoraInviteEndByPeerCb(end_cb);
	InviteManager::GetInstance()->RegAgoraDataUserLeavedCb(user_leave_cb);
	InviteManager::GetInstance()->RegAgoraLogoutCb(signal_logout_cb);
	InviteManager::GetInstance()->RegAgoraChatConnectLostCb(chat_connect_lost_cb);
	InviteManager::GetInstance()->RegAgoraChatReJoinSuccessCb(chat_rejoin_cb);
	InviteManager::GetInstance()->RegAgoraChatUserOfflineCb(chat_user_offline_cb);
	InviteManager::GetInstance()->RegAgoraDataMsgSendErrorCb(data_message_err_cb);
}

void ClassManager::OnRegisterNimCallBack()
{
	auto control_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnNimControlNotifyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	auto mem_notify_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnNimMemberNotifyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	//auto hangup_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnNimHangupNotifyCallback, this, std::placeholders::_1, std::placeholders::_2));
	auto receive_data_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnNimRecDataCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	auto connect_notify_cb = ToWeakCallback(nbase::Bind(&ClassManager::OnNimConnectNotifyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	InviteManager::GetInstance()->RegNIMControlCb(control_cb);
	//InviteManager::GetInstance()->RegNIMHangupCb(hangup_cb);
	InviteManager::GetInstance()->RegNIMConnectCb(connect_notify_cb);
	InviteManager::GetInstance()->RegNIMMemberCb(mem_notify_cb);
	InviteManager::GetInstance()->RegNIMRecDataCb(receive_data_cb);
}

void ClassManager::StartSendDataService()
{
	ClearCommandBuffer();
	m_send_data_timer_.Cancel();
	auto closure = nbase::Bind(&ClassManager::SendDataBuffer, this);
	auto task = m_send_data_timer_.ToWeakCallback(nbase::Bind(closure));
	qtbase::Post2RepeatedTask(kThreadUIHelper, task, nbase::TimeDelta::FromMilliseconds(CHANNEL_MESSAGE_TIME));
}

void ClassManager::StopSendDataService()
{
	m_send_data_timer_.Cancel();
	ClearCommandBuffer();
}

void ClassManager::SendCommand(One2OneCommandType command_type_, void* param1_ /*= nullptr*/, void* param2_ /*= nullptr*/)
{
	nbase::NAutoLock auto_lock(&m_send_lock_);

	std::string temp_s("");
	switch (command_type_)
	{
	case COMMAND_VERSION:
		break;
	case TEA_DRAW_START:
	case TEA_DRAW_MOVE:
	case TEA_DRAW_END:
	case TEA_ERASER_DRAW_DOWN:
	case TEA_ERASER_DRAW_MOVE:
	case TEA_ERASER_DRAW_UP:
		temp_s = nbase::StringPrintf("%d:%lf,%lf;", command_type_, *((double*)param1_), *((double*)param2_));
		break;
	case TEA_CHANGE_COLOR:
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, (char*)param1_);
		break;
	case TEA_CHANGE_BOARD_PAGER:
		temp_s = nbase::StringPrintf("%d:%d;", command_type_, *((int*)param1_));
		break;
	case UPLOAD_IMG:
	{
		std::string url = std::string((char*)param1_);
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, (char*)url.c_str());
		//LOG_O2O(L"TEA_UPLOAD_IMG: {0}") << temp_s;
		break;
	}
	case TEA_START_COURSE:
		temp_s = nbase::StringPrintf("%d:%I64d,%I64d;", command_type_, *((uint64_t*)param1_), *((uint64_t*)param2_));
		//LOG_O2O(L"TEA_START_COURSE: {0}") << temp_s;
		break;
	case END_COURSE:
		temp_s = nbase::StringPrintf("%d:%I64d;", command_type_, *((uint64_t*)param1_));
		//LOG_O2O(L"TEA_END_COURSE: {0}") << temp_s;
		break;
	case TEA_CHANGE_PRICE:
		temp_s = nbase::StringPrintf("%d:%.1f;", command_type_, *((double*)param1_));
		//LOG_O2O(L"TEA_CHANGE_PRICE: {0}") << temp_s;
		break;
	case STU_CHARGE_BALANCE:
		temp_s = nbase::StringPrintf("%d:;", command_type_);
		break;
	case TEA_CHANGE_PHOTO_PAGE:
		temp_s = nbase::StringPrintf("%d:%d;", command_type_, *((int*)param1_));
		break;
	case STU_RECV_COURSE_START:
		temp_s = nbase::StringPrintf("%d:%I64d;", command_type_, *((uint64_t*)param1_));
		break;
	case EXCHANGE_DATA:
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, (char*)param1_);
		//LOG_O2O(L"STU_EXCHANGE_DATA: {0}") << temp_s;
		break;
	case TEA_PPT_CHG_PAGE:
		temp_s = nbase::StringPrintf("%d:%d;", command_type_, *((int*)param1_));
		break;
	case STU_REVOKE:
	case TEA_REVOKE:
		temp_s = nbase::StringPrintf("%d:;", command_type_);
		break;
	case STU_CLEAN:
	case TEA_CLEAN:
		temp_s = nbase::StringPrintf("%d:;", command_type_);
		break;
	case AUDIO_START:
		temp_s = nbase::StringPrintf("%d:%I64d;", command_type_, *((int64_t*)param1_));
		break;
	case TIMES_SIGN:
		break;
	case TEA_UPLOAD_PPT_FILES:
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, (char*)param1_);
		//LOG_O2O(L"TEA_UPLOAD_PPT_FILES: {0}") << temp_s;
		break;
	case STU_CHANGE_BOARD_PAGER:
	case STU_CHANGE_PHOTO_PAGER:
	case STU_CHANGE_PPT_PAGER:
		temp_s = nbase::StringPrintf("%d:%d;", command_type_, *((int*)param1_));
		break;
	case STU_DRAW_START:
	case STU_DRAW_MOVE:
	case STU_DRAW_END:
		temp_s = nbase::StringPrintf("%d:%lf,%lf;", command_type_, *((double*)param1_), *((double*)param2_));
		break;
	case TEA_RESET_PPT:
		temp_s = nbase::StringPrintf("%d:;", command_type_);
		break;
	case COMMAND_START_OBSERVE:
	{
		std::string data_str = std::string((char*)param1_);
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, data_str.c_str());
		break;
	}
	case COMMAND_OBSERVE_DATA:
	{
		std::string data_str = std::string((char*)param1_);
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, data_str.c_str());
		break;
	}
	case COMMAND_EXCHANGE_USER_INFO:
	{
		std::string data_str = std::string((char*)param1_);
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, data_str.c_str());
		//LOG_O2O(L"COMMAND_EXCHANGE_USER_INFO: {0} ") << temp_s;
		break;
	}
	case TEA_ROTATE_IMAGE:
	{
		temp_s = nbase::StringPrintf("%d:%d;", command_type_, *((int*)param1_));
		//LOG_O2O(L"TEA_ROTATE_IMAGE: {0} ") << temp_s;
		break;
	}
	case TEA_UPLOAD_IMAGE_FILES:
	{
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, (char*)param1_);
		//LOG_O2O(L"TEA_UPLOAD_IMAGE_FILES: {0}") << temp_s;
		break;
	}
	case TEA_CLASS_UPLOAD_PPT_FILES:
	{
		temp_s = nbase::StringPrintf("%d:%d,%s;", command_type_, *((int*)param1_), (char*)param2_);
		break;
	}
	case TEA_CLASS_CHANGE_PPT_PAGER:
	{
		temp_s = nbase::StringPrintf("%d:%d,%d;", command_type_, *((int*)param1_), *((int*)param2_));
		break;
	}
	case TEA_CLASS_CLEAN_PPT:
	{
		temp_s = nbase::StringPrintf("%d:%d,%d;", command_type_, *((int*)param1_), *((int*)param2_));
		break;
	}
	case TEA_CLASS_BEGIN_UPLOAD:
	{
		temp_s = nbase::StringPrintf("%d:;", command_type_);
		break;
	}
	case TEA_CLASS_END_UPLOAD:
	{
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, (char*)param1_);
		break;
	}
	case TEA_ADD_VIDEO_BOARD:
	{
		std::string url = std::string((char*)param1_);
		std::string file_name = std::string((char*)param2_);
		temp_s = nbase::StringPrintf("%d:%s,%s;", command_type_, url.c_str(), file_name.c_str());
		LOG_O2O(L"TEA_ADD_VIDEO_BOARD: {0}") << temp_s;
		break;
	}
	case TEA_VIDEO_PLAY:
	{
		temp_s = nbase::StringPrintf("%d:%I64d,%d;", command_type_, *((int64_t*)param1_), *((int*)param2_));
		LOG_O2O(L"TEA_VIDEO_PLAY: {0}") << temp_s;
		break;
	}
	case TEA_VIDEO_PAUSE:
	{
		temp_s = nbase::StringPrintf("%d:%I64d;", command_type_, *((int64_t*)param1_));
		LOG_O2O(L"TEA_VIDEO_PAUSE: {0}") << temp_s;
		break;
	}case TEA_UPLOAD_H5PPT_FILE:
	{
		std::string url = std::string((char*)param1_);
		temp_s = nbase::StringPrintf("%d:%s;", command_type_, url.c_str());
		LOG_O2O(L"TEA_UPLOAD_H5PPT_FILE: {0}") << temp_s;
		break;
	}
	case REPLAY_H5DOC:
	{
		temp_s = nbase::StringPrintf("%d:%d;", command_type_, *((int*)param1_));
		LOG_O2O(L"REPLAY_H5DOC: {0}") << temp_s;
		break;
	}
	case TEA_CHANGE_H5PPT_PAGE_STEP:
	{
		temp_s = nbase::StringPrintf("%d:%d,%d;", command_type_, *((int*)param1_), *((int*)param2_));
		LOG_O2O(L"TEA_CHANGE_H5PPT_PAGE_STEP: {0}") << temp_s;
		break;
	}

	default:
		break;
	}

	if (!temp_s.empty())
	{
		AddCommandBuffer(temp_s);
	}
}

void ClassManager::AddCommandBuffer(std::string command)
{
	nbase::NAutoLock auto_lock(&m_command_lock_);

	cur_buffer_.append(command);
}

void ClassManager::ClearCommandBuffer()
{
	nbase::NAutoLock auto_lock(&m_command_lock_);

	cur_buffer_.clear();
	cur_buffer_ = "";
}

void ClassManager::SendDataBuffer()
{
	nbase::NAutoLock auto_lock(&m_command_lock_);
	if (cur_buffer_.empty())
	{
		return;
	}
	BaseDataChannel* data_channel = ChannelManager::GetInstance()->GetDataChannel();
	if (!data_channel)
	{
		return;
	}
	std::string channel_id = data_channel->GetSessionOrChannelID();
	data_channel->SendChannelMessage(channel_id, cur_buffer_);
	cur_buffer_.clear();
	cur_buffer_ = "";
}

void ClassManager::OnParseData(std::string command_str)
{
	std::vector<OrderInfo> command_list;
	AnalyzeDataToCommand(command_str, command_list);
	ExecuteCommandList(command_list);
}

void ClassManager::ExecuteCommandList(std::vector<OrderInfo> &command_list)
{
	std::list<DrawOpInfo> tea_info_list;
	std::list<DrawOpInfo> stu_info_list;
	for each (OrderInfo command in command_list)
	{

		if (!(command.type == STU_REVOKE || command.type == STU_CLEAN
			|| command.type == STU_DRAW_START || command.type == STU_DRAW_MOVE || command.type == STU_DRAW_END))
		{
			if (!stu_info_list.empty())
			{
				emit SignalExecuteStuDrawInfoList(stu_info_list);
				stu_info_list.clear();
			}
			
		}

		if (!(command.type == TEA_CHANGE_COLOR || command.type == TEA_REVOKE || command.type == TEA_CLEAN
			|| command.type == TEA_DRAW_START || command.type == TEA_DRAW_MOVE || command.type == TEA_DRAW_END
			|| command.type == TEA_ERASER_DRAW_DOWN || command.type == TEA_ERASER_DRAW_MOVE || command.type == TEA_ERASER_DRAW_UP))
		{
			if (!tea_info_list.empty())
			{
				emit SignalExecuteTeaDrawInfoList(tea_info_list);
				tea_info_list.clear();
			}
		}

		switch (command.type)
		{
		case COMMAND_VERSION:
		{
			emit SignalCommandVersion();
			break;
		}
		case TEA_DRAW_START:
		case TEA_DRAW_MOVE:
		case TEA_DRAW_END:
		{
			DrawOpInfo info;
			info.draw_op_type_ = CommandTypeToDrawOpType(command.type);
			info.x_ = command.para_double_1;
			info.y_ = command.para_double_2;
			tea_info_list.push_back(info);
			break;
		}
		case UPLOAD_IMG:
		{
			std::string url = command.para_string_1;
			emit SignalCommandUploadImage(url);
			break;
		}
		case TEA_START_COURSE:
		{
			int64_t course_id = command.course_id;
			int64_t begin_time = command.start_time;
			emit SignalCommandTeaStartCourse(course_id, begin_time);
			break;
		}
		case END_COURSE:
		{
			emit SignalCommandEndCourse();
			break;
		}
		case STU_CHARGE_BALANCE:
		{
			emit SignalCommandRefreshBalance();
			break;
		}
		case STU_RECV_COURSE_START:
		{
			emit SignalCommandStuRecvCourseStart(command.para_string_1);
			break;
		}
		case EXCHANGE_DATA:
		{
			emit SignalCommandExchangeData(command.para_int_1, command.para_int_2);
			break;
		}
		case TEA_REVOKE:
		{
			DrawOpInfo info;
			info.draw_op_type_ = DrawOpType::DrawOpUndoCb;
			tea_info_list.push_back(info);
			break;
		}
		case TEA_CLEAN:
		{
			DrawOpInfo info;
			info.draw_op_type_ = DrawOpType::DrawOpClearCb;
			tea_info_list.push_back(info);
			break;
		}
		case TEA_CHANGE_BOARD_PAGER:
		{
			int page = command.para_int_1;
			emit SignalCommandTeaChangePage(One2OneCoursewareType::COURSEWARE_WHITEBOARD, page);
			break;
		}
		case TEA_CHANGE_PHOTO_PAGE:
		{
			int page = command.para_int_1;
			emit SignalCommandTeaChangePage(One2OneCoursewareType::COURSEWARE_PICTURE, page);
			break;
		}
		case TEA_PPT_CHG_PAGE:
		{
			int page = command.para_int_1;
			emit SignalCommandTeaChangePage(One2OneCoursewareType::COURSEWARE_PPT, page);
			break;
		}
		case TEA_UPLOAD_PPT_FILES:
		{
			emit SignalCommandTeaUploadImageFiles(command.para_string_1, 0);
			break;
		}
		case TEA_UPLOAD_IMAGE_FILES:
		{
			emit SignalCommandTeaUploadImageFiles(command.para_string_1, 1);
			break;
		}
		case TEA_CHANGE_PRICE:
		{
			emit SignalCommandTeaChangePrice(command.para_string_1);
			break;
		}
		case TEA_CHANGE_COLOR:
		{
			// 老师端换笔的颜色
			std::string color_str = command.para_string_1;
			DrawOpInfo info;
			info.draw_op_type_ = DrawOpType::DrawOpPenColor;
			COLORREF color = systembase::GetPenColor(color_str);
			info.pen_color_ = QColor(GetRValue(color), GetGValue(color), GetBValue(color));
			tea_info_list.push_back(info);
			break;
		}
		case COMMAND_START_OBSERVE:
		{
			std::string sign_str = command.para_string_1;
			emit SignalCommandStartObserve(sign_str);
			
			break;
		}
		case COMMAND_OBSERVE_DATA:
		{
			emit SignalCommandReceiveObserveData(command.para_string_1);
			break;
		}
		case COMMAND_EXCHANGE_USER_INFO:
		{
			emit SignalCommandExchangeUserInfo(command.para_string_1);
			break;
		}
		case STU_REVOKE:
		{
			DrawOpInfo info;
			info.draw_op_type_ = DrawOpType::DrawOpUndoCb;
			stu_info_list.push_back(info);
			break;
		}
		case STU_CLEAN:
		{
			DrawOpInfo info;
			info.draw_op_type_ = DrawOpType::DrawOpClearCb;
			stu_info_list.push_back(info);
			break;
		}
		case  STU_CHANGE_BOARD_PAGER:
		{
			int page = command.para_int_1;
			emit SignalCommandStuChangePage(One2OneCoursewareType::COURSEWARE_WHITEBOARD, page);
			break;
		}
		case STU_CHANGE_PHOTO_PAGER:
		{
			int page = command.para_int_1;
			emit SignalCommandStuChangePage(One2OneCoursewareType::COURSEWARE_PICTURE, page);
			break;
		}
		case STU_CHANGE_PPT_PAGER:
		{
			int page = command.para_int_1;
			emit SignalCommandStuChangePage(One2OneCoursewareType::COURSEWARE_PPT, page);
			break;
		}
		case STU_DRAW_START:
		case STU_DRAW_MOVE:
		case STU_DRAW_END:
		{
			DrawOpInfo info;
			info.draw_op_type_ = CommandTypeToDrawOpType(command.type);
			info.x_ = command.para_double_1;
			info.y_ = command.para_double_2;
			stu_info_list.push_back(info);
			break;
		}
		case TEA_ROTATE_IMAGE:
		{
			emit SignalTeaRotateImage(command.para_int_1);
			break;
		}
		case TEA_ERASER_DRAW_DOWN:
		case TEA_ERASER_DRAW_MOVE:
		case TEA_ERASER_DRAW_UP:
		{
			DrawOpInfo info;
			info.draw_op_type_ = CommandTypeToDrawOpType(command.type);
			info.x_ = command.para_double_1;
			info.y_ = command.para_double_2;
			tea_info_list.push_back(info);
			break;
		}
		case TEA_CLASS_UPLOAD_PPT_FILES:
		{
			emit SignalCommandClassUploadPPT(command.para_int_1, command.para_string_1);
			break;
		}
		case TEA_CLASS_CHANGE_PPT_PAGER:
		{
			emit SignalCommandClassChangePPTPage(command.para_int_1, command.para_int_2);
			break;
		}
		case TEA_CLASS_CLEAN_PPT:
		{
			emit SignalCommandClassClearPPT(command.para_int_1);
			break;
		}
		case TEA_CLASS_BEGIN_UPLOAD:
		{
			emit SignalCommandClassStartUpload();
			break;
		}
		case TEA_CLASS_END_UPLOAD:
		{
			emit SignalCommandClassUploaded(command.para_string_1);
			break;
		}
		case REPLAY_H5DOC:
		{
			emit SignalCommandWebViewShow(command.para_int_1);
			break;
		}
		case TEA_UPLOAD_H5PPT_FILE:
		{
			emit SignalCommandLoadPPTUrl(command.para_string_1);
			break;
		}
		case TEA_CHANGE_H5PPT_PAGE_STEP:
		{
			emit SignalCommandSwitchPPTPageStep(command.para_int_1, command.para_int_2);
			break;
		}
		case TEA_ADD_VIDEO_BOARD:
		{
			emit SignalCommandTeaAddVedioCourseware(command.para_string_1, command.para_string_2);
			break;
		}
		case TEA_VIDEO_PLAY:
		{
			emit SignalCommandTeaPlayVedio(command.para_int_1, command.start_time);
			break;
		}
		case TEA_VIDEO_PAUSE:
		{
			emit SignalCommandTeaStopVedio(command.start_time);
			break;
		}
		}
	}

	if (!tea_info_list.empty())
	{
		emit SignalExecuteTeaDrawInfoList(tea_info_list);
	}

	if (!stu_info_list.empty())
	{
		emit SignalExecuteStuDrawInfoList(stu_info_list);
	}
}

bool ClassManager::AnalyzeDataToCommand(std::string data, std::vector<OrderInfo> &command_list)
{
	std::list<std::string> op_list_ = nbase::StringTokenize(data.c_str(), ";");
	if (op_list_.empty())
	{
		return false;
	}

	for (auto iter = op_list_.begin(); iter != op_list_.end(); iter++)
	{
		std::string& s_ = (*iter);
		int f_ = s_.find(":");
		std::string op_s_ = f_ > 0 ? s_.substr(0, f_) : s_;
		OrderInfo info;
		if (op_s_ == nbase::IntToString(COMMAND_VERSION))
		{
			info.type = COMMAND_VERSION;
		}
		else if (op_s_ == nbase::IntToString(UPLOAD_IMG)) //添加图片
		{
			info.type = UPLOAD_IMG;
			info.para_string_1 = s_.substr(f_ + 1);

		}
		else if (op_s_ == nbase::IntToString(TEA_START_COURSE)) //老师开始上课
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> coordinate = nbase::StringTokenize(subStr.c_str(), ",");
			if (coordinate.size() == 2)
			{
				nbase::StringToInt64(coordinate.front(), &info.course_id);
				coordinate.pop_front();
				nbase::StringToInt64(coordinate.front(), &info.start_time);
				coordinate.pop_front();
				info.type = TEA_START_COURSE;
			}
		}
		else if (op_s_ == nbase::IntToString(END_COURSE))
		{
			info.type = END_COURSE;
		}
		else if (op_s_ == nbase::IntToString(STU_CHARGE_BALANCE))
		{
			info.type = STU_CHARGE_BALANCE;
		}
		else if (op_s_ == nbase::IntToString(STU_RECV_COURSE_START))
		{
			info.type = STU_RECV_COURSE_START;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(EXCHANGE_DATA))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> data_list = nbase::StringTokenize(subStr.c_str(), ",");
			if (data_list.size() == 2)
			{
				nbase::StringToInt(data_list.front(), &info.para_int_1);
				data_list.pop_front();
				nbase::StringToInt(data_list.front(), &info.para_int_2);
				data_list.pop_front();
				info.type = EXCHANGE_DATA;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_REVOKE))
		{
			info.type = TEA_REVOKE;
		}
		else if (op_s_ == nbase::IntToString(TEA_CLEAN))
		{
			info.type = TEA_CLEAN;
		}
		else if (op_s_ == nbase::IntToString(STU_REVOKE))
		{
			info.type = STU_REVOKE;
		}
		else if (op_s_ == nbase::IntToString(STU_CLEAN))
		{
			info.type = STU_CLEAN;
		}
		else if (op_s_ == nbase::IntToString(TEA_CHANGE_PHOTO_PAGE)
			|| op_s_ == nbase::IntToString(TEA_CHANGE_BOARD_PAGER)
			|| op_s_ == nbase::IntToString(TEA_PPT_CHG_PAGE)
			|| op_s_ == nbase::IntToString(STU_CHANGE_BOARD_PAGER)
			|| op_s_ == nbase::IntToString(STU_CHANGE_PHOTO_PAGER)
			|| op_s_ == nbase::IntToString(STU_CHANGE_PPT_PAGER))
		{
			std::string subStr = s_.substr(f_ + 1);
			if (nbase::StringToInt(subStr, &info.para_int_1))
			{
				nbase::StringToInt(op_s_, &info.type);
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_UPLOAD_PPT_FILES))
		{
			info.type = TEA_UPLOAD_PPT_FILES;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(TEA_UPLOAD_IMAGE_FILES))
		{
			info.type = TEA_UPLOAD_IMAGE_FILES;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(TEA_DRAW_START)
			|| op_s_ == nbase::IntToString(TEA_DRAW_MOVE)
			|| op_s_ == nbase::IntToString(TEA_DRAW_END)
			|| op_s_ == nbase::IntToString(TEA_ERASER_DRAW_DOWN)
			|| op_s_ == nbase::IntToString(TEA_ERASER_DRAW_MOVE)
			|| op_s_ == nbase::IntToString(TEA_ERASER_DRAW_UP))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> coordinate = nbase::StringTokenize(subStr.c_str(), ",");
			if (coordinate.size() == 2)
			{
				nbase::StringToDouble(coordinate.front(), &info.para_double_1);
				coordinate.pop_front();
				nbase::StringToDouble(coordinate.front(), &info.para_double_2);
				coordinate.pop_front();
				nbase::StringToInt(op_s_, &info.type);
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_CHANGE_PRICE))
		{
			//老师修改价格
			info.type = TEA_CHANGE_PRICE;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(TEA_CHANGE_COLOR))
		{
			// 老师端换笔的颜色
			info.type = TEA_CHANGE_COLOR;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(STU_DRAW_START)
			|| op_s_ == nbase::IntToString(STU_DRAW_MOVE)
			|| op_s_ == nbase::IntToString(STU_DRAW_END))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> coordinate = nbase::StringTokenize(subStr.c_str(), ",");
			if (coordinate.size() == 2)
			{
				nbase::StringToDouble(coordinate.front(), &info.para_double_1);
				coordinate.pop_front();
				nbase::StringToDouble(coordinate.front(), &info.para_double_2);
				coordinate.pop_front();
				nbase::StringToInt(op_s_, &info.type);
			}
		}
		else if (op_s_ == nbase::IntToString(COMMAND_START_OBSERVE))
		{
			info.type = COMMAND_START_OBSERVE;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(COMMAND_OBSERVE_DATA))
		{
			info.type = COMMAND_OBSERVE_DATA;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(COMMAND_EXCHANGE_USER_INFO))
		{
			info.type = COMMAND_EXCHANGE_USER_INFO;
			info.para_string_1 = s_.substr(f_ + 1);
		}
		else if (op_s_ == nbase::IntToString(TEA_ROTATE_IMAGE))
		{
			if (nbase::StringToInt(s_.substr(f_ + 1), &info.para_int_1))
			{
				info.type = TEA_ROTATE_IMAGE;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_CLASS_UPLOAD_PPT_FILES))
		{ 
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> arr_val = nbase::StringTokenize(subStr.c_str(), ",");
			if (arr_val.size() > 0)
			{
				std::string str = arr_val.front();
				nbase::StringToInt(arr_val.front(), &info.para_int_1);
				arr_val.pop_front();
				info.para_string_1 = subStr.substr(str.length()+1);
				//arr_val.pop_front();
				info.type = TEA_CLASS_UPLOAD_PPT_FILES;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_CLASS_CHANGE_PPT_PAGER))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> arr_val = nbase::StringTokenize(subStr.c_str(), ",");
			if (arr_val.size() == 2)
			{
				nbase::StringToInt(arr_val.front(), &info.para_int_1);
				arr_val.pop_front();
				nbase::StringToInt(arr_val.front(), &info.para_int_2);
				arr_val.pop_front();
				info.type = TEA_CLASS_CHANGE_PPT_PAGER;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_CLASS_CLEAN_PPT))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> arr_val = nbase::StringTokenize(subStr.c_str(), ",");
			if (arr_val.size() == 2)
			{
				nbase::StringToInt(arr_val.front(), &info.para_int_1);
				arr_val.pop_front();
				nbase::StringToInt(arr_val.front(), &info.para_int_2);
				arr_val.pop_front();
				info.type = TEA_CLASS_CLEAN_PPT;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_CLASS_BEGIN_UPLOAD))
		{
			info.type = TEA_CLASS_BEGIN_UPLOAD;
		}
		else if (op_s_ == nbase::IntToString(TEA_CLASS_END_UPLOAD))
		{
			info.para_string_1 = s_.substr(f_ + 1);
			info.type = TEA_CLASS_END_UPLOAD;
		}
		else if (op_s_ == nbase::IntToString(REPLAY_H5DOC))
		{
			std::string op_1 = s_.substr(f_ + 1);
			nbase::StringToInt(op_1, &info.para_int_1);
			info.type = REPLAY_H5DOC;
		}
		else if (op_s_ == nbase::IntToString(TEA_UPLOAD_H5PPT_FILE))
		{
			info.para_string_1 = s_.substr(f_ + 1);
			info.type = TEA_UPLOAD_H5PPT_FILE;
		}
		else if (op_s_ == nbase::IntToString(TEA_CHANGE_H5PPT_PAGE_STEP))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> arr_val = nbase::StringTokenize(subStr.c_str(), ",");
			if (arr_val.size() == 2)
			{
				nbase::StringToInt(arr_val.front(), &info.para_int_1);
				arr_val.pop_front();
				nbase::StringToInt(arr_val.front(), &info.para_int_2);
				arr_val.pop_front();
				info.type = TEA_CHANGE_H5PPT_PAGE_STEP;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_ADD_VIDEO_BOARD))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> arr_val = nbase::StringTokenize(subStr.c_str(), ",");
			if (arr_val.size() == 2)
			{
				info.para_string_1 = arr_val.front();
				arr_val.pop_front();
				info.para_string_2 = arr_val.front();
				arr_val.pop_front();
				info.type = TEA_ADD_VIDEO_BOARD;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_VIDEO_PLAY))
		{
			std::string subStr = s_.substr(f_ + 1);
			std::list<std::string> arr_val = nbase::StringTokenize(subStr.c_str(), ",");
			if (arr_val.size() == 2)
			{
				nbase::StringToInt64(arr_val.front(), &info.start_time);
				arr_val.pop_front();
				nbase::StringToInt(arr_val.front(), &info.para_int_1);
				arr_val.pop_front();
				info.type = TEA_VIDEO_PLAY;
			}
		}
		else if (op_s_ == nbase::IntToString(TEA_VIDEO_PAUSE))
		{
			std::string op_1 = s_.substr(f_ + 1);
			nbase::StringToInt64(op_1, &info.start_time);
			info.type = TEA_VIDEO_PAUSE;
		}

		if (info.type >= 0)
		{
			command_list.push_back(info);
		}
	}
	return true;
}

DrawOpType ClassManager::CommandTypeToDrawOpType(int type)
{
	if (type == One2OneCommandType::TEA_DRAW_START
		|| type == One2OneCommandType::STU_DRAW_START)
	{
		return DrawOpType::DrawOpStart;
	}
	else if (type == One2OneCommandType::TEA_DRAW_MOVE
		|| type == One2OneCommandType::STU_DRAW_MOVE)
	{
		return DrawOpType::DrawOpMove;
	}
	else if (type == One2OneCommandType::TEA_DRAW_END
		|| type == One2OneCommandType::STU_DRAW_END)
	{
		return DrawOpType::DrawOpEnd;
	}
	else if (type == One2OneCommandType::TEA_CLEAN
		|| type == One2OneCommandType::STU_CLEAN)
	{
		return DrawOpType::DrawOpClearCb;
	}
	else if (type == One2OneCommandType::TEA_REVOKE
		|| type == One2OneCommandType::STU_REVOKE)
	{
		return DrawOpType::DrawOpUndoCb;
	}
	else if (type == One2OneCommandType::TEA_CHANGE_COLOR)
	{
		return DrawOpType::DrawOpPenColor;
	}
	else if (type == TEA_ERASER_DRAW_DOWN)
	{
		return EraserOpStart;
	}
	else if (type == TEA_ERASER_DRAW_MOVE)
	{
		return EraserOpMove;
	}
	else if (type == TEA_ERASER_DRAW_UP)
	{
		return EraserOpEnd;
	}

	return DrawOpType::DrawOpNone;
}

void ClassManager::OnAgoraDataMsgSendError(std::string message_id, int ecode)
{
	LOG_O2O(L"ClassManager::OnAgoraDataMsgSendError: message_id: {0}, ecode: {1}") << message_id << ecode;
}
