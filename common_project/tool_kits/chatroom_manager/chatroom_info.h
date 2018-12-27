#pragma once
#include "base\callback\callback.h"
#include "channel_manager\channel_manager_global.h"


enum MeetingOptType//command
{
	Mot_AllStatus = 1,	//������֪ͨ��Ȩ�޵ĳ�Ա�б�					��1����2��	{"type":10,"data":{"room_id":"123","command":1,"uids":["a","b"]}}
	Mot_GetStatus = 2,	//��Ա��������������Ȩ�޵ĳ�Ա					��1��		{"type":10,"data":{"room_id":"123","command":2}}
	Mot_StatusResponse = 3,	//��Ȩ�޵ĳ�Ա�������߷����Լ���Ȩ�޵�֪ͨ		��2��		{"type":10,"data":{"room_id":"123","command":3,"uids":["myid"]}}
	Mot_SpeekRequest = 10,	//����������������Ȩ��							��2��		{"type":10,"data":{"room_id":"123","command":10}}
	Mot_SpeekAccept = 11,	//������ͬ����������,������ͬʱ����Ⱥ��1��Ϣ	��2��		{"type":10,"data":{"room_id":"123","command":11}}
	Mot_SpeekReject = 12,	//�����˾ܾ���ر�����,������ͬʱ����Ⱥ��1��Ϣ	��2��		{"type":10,"data":{"room_id":"123","command":12}}
	Mot_SpeekRequestCancel = 13,	//ȡ������������������Ȩ��						��2��		{"type":10,"data":{"room_id":"123","command":13}}
	Mot_ChannelCreate = 14,	//����Ƶ��֪ͨ ������
	Mot_AllSilent = 15,	//ȫ������
	Mot_AllSilentCancel = 16,	//ȡ��ȫ������
	Mot_StuSpeakStart = 17,	//ѧ����ʼ˵��
	Mot_StuSpeakEnd = 18,	//ѧ������˵��
	Mot_LessionIdInfo = 19,	//ѧ�����뷿��ʱ������ѧ����ǰ��lessionId		p2p��Ϣ {"data":{"room_id":"123","command":19,"lession_id":929,"uid":1}}
	Mot_StartLession = 20,	//��ʼ�Ͽ� ��������Ϣ
	Mot_EndLession = 21,	//�����Ͽ�
	Mot_MasterVolumn = 22,	//������˵��������{"data":{"room_id":"123","command":19,"volumes":"10,23,32,43,0,0"}}
	Mot_ForbbidenSpeak = 23,	//��ֹ����	p2p
	Mot_ShareScreenRatio = 24,	//����ֱ���	p2p
	Mot_PlayClassOverRing = 25,	//������Ϣ�� �����¿�����
	Mot_CourseConnect = 26,   //�����˿γ���������
	Mot_CourseDisconnect = 27,   //�����˿γ̶Ͽ�
};

enum CustomMsgType
{
	CustomMsgType_Jsb = 1,	//ʯͷ���Ӳ�
	CustomMsgType_SnapChat,	//�ĺ󼴷�
	CustomMsgType_Sticker,	//��ͼ
	CustomMsgType_Rts,		//�װ�ķ��������Ϣ
	CustomMsgType_Meeting = 10,		//���˻������Э��
};

struct ChannelJoinParam
{
	NetworkServiceType data_service_;
	NetworkServiceType chat_service_;
	DataChannelConnectStateCb connect_state_cb_;
	DataChannelMessageCb rev_msg_cb_;
	bool b_tea_;				//����ʱ����ݽ�ɫ���ø���ʽ
	std::string channel_id_;	
	uint32_t agora_uid_;
	std::string channel_key_;

	std::string own_url_;

	ChannelJoinParam()
	{
		data_service_ = SWITCH_NETWORK_OWN;
		chat_service_ = SWITCH_NETWORK_AGORA;
		own_url_ = "";
	}
};

enum ClassroomChannelType
{
	CLASSROOM_CHANNEL_TYPE_DATA = 0,
	CALSSROOM_CHANNEL_TYPE_CHAT = 1,
};

typedef std::function<void(int, int)> ClassroomChannelJoinFailCb;
typedef std::function<void(const std::string s_channel_id, uint32_t uid)> ClassroomChatJoinChannelSuccessCb;