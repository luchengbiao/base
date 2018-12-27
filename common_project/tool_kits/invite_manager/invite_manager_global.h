#pragma once
#include "channel_manager\channel_manager_global.h"

typedef std::function<void(const std::string s_channel_id, uint32_t uid)> JoinChatChannelSuccessCallBack;

enum One2OneUserType
{
	USER_NORMAL = 0,   // ��ͨѧ��
	USER_OBSERVER = 1, // ������
	USER_AUDITION = 2,	//����ѧ��
};


struct One2OneCallInfo
{
	bool b_creator_;
	NetworkServiceType m_service_type_;   //����ѡ��
	int rts_flag_type_;					  //����״̬
	std::string peer_agora_account_;	  //�����ƶ��˺�
	std::string peer_agora_account_pc_;	  //����PC�˺�

	uint32_t peer_agora_uid_;			  //��ȡ����������к��uid
	uint32_t my_agora_uid_;				  //�����Լ�������uid ����ʹ���˻�
	std::string agora_chatchannel_key_;	  //��������Ƶ����Ƶ��key
	std::string agora_recording_key_;	  //��������Ƶ¼��key


	std::string peer_nim_account_;		  //���׶Է��˺�
	std::string nim_session_id_;		  //��������ʹ��
	std::string s_channel_id_;			  //����Ƶ��Ψһ��ʶ

	One2OneUserType m_user_type_;		  //��ʱ���� ���е�����
	std::string lanuch_from_;			  //������Դ

	uint64_t im_id_;
	std::string extra_info_;			  //Я��������Ϣ

	NetworkServiceType chat_type_;        //����ƵƵ��ѡ��
	NetworkServiceType data_type_;		  //����Ƶ��ѡ��

	std::string nim_rts_channel_id_;      //����Ƶ��¼�Ƶı�ʶ

	int64_t peer_id_;					  //�Է��û�id
	std::string peer_name_;
	std::string peer_head_;
	std::string my_nick_name_;			  //��ʾ�Լ��ǳ�
	std::string stu_grade_name_;
	int tea_subject_id_;				  //��ʦ�γ�����
	int chess_board_type_;				  //����·��(Χ��)
	std::string tea_rank_;				  //��ʦ��λ
	double tea_price_;					  //��ʦ�����۸�
	double stu_balance_;				  //ѧ����Ӧר�����
	int trail_balance_;					  //������ʱ��
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
