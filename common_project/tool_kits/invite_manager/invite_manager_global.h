#pragma once
#include "channel_manager\channel_manager_global.h"

typedef std::function<void(const std::string s_channel_id, uint32_t uid)> JoinChatChannelSuccessCallBack;

enum One2OneUserType
{
	USER_NORMAL = 0,   // 普通学生
	USER_OBSERVER = 1, // 旁听者
	USER_AUDITION = 2,	//试听学生
};


struct One2OneCallInfo
{
	bool b_creator_;
	NetworkServiceType m_service_type_;   //信令选择
	int rts_flag_type_;					  //呼叫状态
	std::string peer_agora_account_;	  //声网移动账号
	std::string peer_agora_account_pc_;	  //声网PC账号

	uint32_t peer_agora_uid_;			  //获取声网信令呼叫后的uid
	uint32_t my_agora_uid_;				  //传入自己的声网uid 登入使用账户
	std::string agora_chatchannel_key_;	  //声网音视频加入频道key
	std::string agora_recording_key_;	  //声网音视频录制key


	std::string peer_nim_account_;		  //网易对方账号
	std::string nim_session_id_;		  //网易邀请使用
	std::string s_channel_id_;			  //加入频道唯一标识

	One2OneUserType m_user_type_;		  //暂时保留 呼叫的类型
	std::string lanuch_from_;			  //呼叫来源

	uint64_t im_id_;
	std::string extra_info_;			  //携带额外信息

	NetworkServiceType chat_type_;        //音视频频道选择
	NetworkServiceType data_type_;		  //数据频道选择

	std::string nim_rts_channel_id_;      //网易频道录制的标识

	int64_t peer_id_;					  //对方用户id
	std::string peer_name_;
	std::string peer_head_;
	std::string my_nick_name_;			  //显示自己昵称
	std::string stu_grade_name_;
	int tea_subject_id_;				  //老师课程类型
	int chess_board_type_;				  //棋盘路数(围棋)
	std::string tea_rank_;				  //老师段位
	double tea_price_;					  //老师辅导价格
	double stu_balance_;				  //学生对应专属余额
	int trail_balance_;					  //试听课时长
	One2OneCallInfo()
	{
		m_user_type_ = USER_NORMAL;
		b_creator_ = true;
		peer_id_ = 0;
		im_id_ = 0;
		my_agora_uid_ = 0;
		peer_agora_uid_ = 0;
		m_service_type_ = SWITCH_NETWORK_AGORA;
		chat_type_ = SWITCH_NETWORK_AGORA;
		data_type_ = SWITCH_NETWORK_AGORA;
		tea_price_ = 0.0;
		stu_balance_ = 0.0;
		tea_subject_id_ = 0;
		chess_board_type_ = 0;
		trail_balance_ = 0;
	}
};
