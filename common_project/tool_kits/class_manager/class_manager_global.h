#pragma once
#include <wtypes.h>
#include <string>
#include <stdint.h> 
#include <vector>

#define CHANNEL_MESSAGE_TIME 180 // Ƶ������Ϣ180ms�ϴ�һ��
#define HEART_BEAT_TIME 15 // �γ����� 30 s/�� 
#define CHECK_START_COURSE_TIME 10 // ���ѧ�����Ƿ��յ���ʼ��� 10s�ȴ�ʱ��
#define HERAT_BEAT_FAIL 4 // �����������
#define COURSE_SECOND_TIMER 1 // ��ʱ��
#define AGORA_UNCONNECT_MSG_TIME 3 // ��⵽�����Ͽ���ÿ��3s����Ƿ��Ѿ�����
#define AGORA_RECONNECT_TIME 60 // �����ϵ�������36�ξ��˳��Ͽ�(3����)
#define AGORA_PER_RECONNECT_TIME 5 // ÿ��5s����һ��
#define AGORA_RECORD_FILE_TIME 3 // ����¼���ļ�ÿ��3sдһ��
#define COUSRE_END_SERVER_ERROR 207 // �������γ̽����ı���
#define EXTEND_THUM_NUM 1 // ����Ӻ���ͼ
#define AGORA_CAMERA_ERROR 1003  // ����������ͷʧ��
#define CHECK_CPU_TIME 3  // ÿ��3���ӣ����һ��
#define CHECK_THUMBNAIL_TIME 2  // ÿ��2s������һ�µ�ǰҳ�������ͼ
#define COUSRE_END_STU_BALANCE_ERROR 1007 //ѧ������

#define REQUEST_DATA_OBJECT_TEA   "tea"
#define REQUEST_DATA_OBJECT_STU   "stu_normal"
#define REQUEST_DATA_OBJECT_OBS   "stu_observe"

enum One2OneCommandType
{
	COMMAND_VERSION = 0,

	// ��ʦ��д����
	TEA_DRAW_START = 1,
	TEA_DRAW_MOVE = 2,
	TEA_DRAW_END = 3,

	TEA_CHANGE_COLOR = 4, // ��ʦ�˻��ʵ���ɫ
	TEA_CHANGE_BOARD_PAGER = 5, // ��ʦ�˰װ廻ҳ

	UPLOAD_IMG = 6, // ѧ������ʦ���ϴ�ͼƬ
	TEA_START_COURSE = 7, //��ʼ�Ͽ�
	END_COURSE = 8, //�����Ͽ�
	TEA_CHANGE_PRICE = 9, //�޸ļ۸�
	STU_CHARGE_BALANCE = 10, //�˻���ֵ
	TEA_CHANGE_PHOTO_PAGE = 11, //�ı���Ƭҳ��
	STU_RECV_COURSE_START = 12, //���ܿ�ʼ�Ͽ�
	EXCHANGE_DATA = 13,  //��������

	TEA_PPT_CHG_PAGE = 14, //��ʦ ppt ��ҳ

	STU_REVOKE = 15, //ѧ���˳���
	TEA_REVOKE = 16, //��ʦ�˳���
	STU_CLEAN = 17, //ѧ�������
	TEA_CLEAN = 18, // ��ʦ�����

	AUDIO_START = 19, //¼����ʼ��ʱ�䣬 ��¼���� xxxx.xx ��
	TIMES_SIGN = 20, //ʱ���־�� ͨ���� 0.xx ��

	TEA_UPLOAD_PPT_FILES = 21, //ppt �ϴ�ͼƬ

	// ѧ����ҳ����
	STU_CHANGE_BOARD_PAGER = 22, // ѧ�����л��װ�ҳ��
	STU_CHANGE_PHOTO_PAGER = 23, // ѧ�����л�ͼƬҳ��
	STU_CHANGE_PPT_PAGER = 24,  // ѧ�����л� ppt ҳ��

	// ѧ����д����
	STU_DRAW_START = 25,
	STU_DRAW_MOVE = 26,
	STU_DRAW_END = 27,

	STU_AUDIO_START = 28,   // ѧ��¼����ʼʱ��
	TEA_RESET_PPT = 29, // ��ʦ����ppt
	COMMAND_START_OBSERVE = 30, //ѧ���������� 
	COMMAND_OBSERVE_DATA = 31,	//ѧ���ϴ��װ����ݷ��ظ�����
	COMMAND_EXCHANGE_USER_INFO = 32,	//�����û���Ϣ

	//���
	TEA_CLASS_UPLOAD_PPT_FILES = 33,  //��Σ���ʦ�ϴ�PPT
	TEA_CLASS_CHANGE_PPT_PAGER = 34,  //��Σ���ʦ�л�ppt���л���ĳ��ppt ��ĳ��ҳ��
	TEA_CLASS_CLEAN_PPT = 35,		  //��Σ���ʦ���ppt
	TEA_CLASS_BEGIN_UPLOAD = 36,	  //������ʦ��ʼ�ϴ�����
	TEA_CLASS_END_UPLOAD = 37,		  //������ʦ�������ϴ�����

	TEA_ROTATE_IMAGE = 38, // ��ʦͼƬ����ת
	TEA_UPLOAD_IMAGE_FILES = 39, // ��ʦ�ϴ����⼯image

	TEA_ERASER_DRAW_DOWN = 43,  // ��ʦ��Ƥ����ʼ
	TEA_ERASER_DRAW_MOVE = 44,  // ��ʦ��Ƥ���ƶ�
	TEA_ERASER_DRAW_UP = 45,    // ��ʦ��Ƥ������

	REPLAY_H5DOC = 46,    // ����h5ppt
	TEA_UPLOAD_H5PPT_FILE = 47, // ��Чppt��ַ
	TEA_CHANGE_H5PPT_PAGE_STEP = 48, // ��ǰ��Чppt��ҳ��ĵڼ�����̬Ч��

	TEA_ADD_VIDEO_BOARD = 49,  // �����Ƶ�װ�
	TEA_VIDEO_PLAY = 50,
	TEA_VIDEO_PAUSE = 51,
};

enum One2OneCoursewareType
{
	COURSEWARE_PPT = 0,
	COURSEWARE_WHITEBOARD,
	COURSEWARE_PICTURE,
	COURSEWARE_CLASS_PPT,
};

enum ExchangeUserInfo
{
	STU_NEED_TEA_UID = 0,   //ѧ�������ȡ��ʦuid

};

struct OrderInfo
{
	int64_t index_;    //����Ψһ��ʶ
	int type;
	int para_int_1;
	int para_int_2;
	std::string para_string_1;
	std::string para_string_2;
	double para_double_1;
	double para_double_2;
	int64_t course_id;
	int64_t start_time;
	OrderInfo()
	{
		index_ = 0;
		type = -1;
		para_int_1 = 0;
		para_int_2 = 0;
		para_string_1 = "";
		para_string_2 = "";
		para_double_1 = 0.0;
		para_double_2 = 0.0;
	}
};

typedef std::vector<OrderInfo> OrderInfoVec;

struct TimestampOrders
{
	int role;			//ѧ��0 ����ʦ1 
	int64_t timestamp;
	OrderInfoVec order_vec;
	TimestampOrders()
	{
		role = 0;
		timestamp = 0;
	}

	bool operator < (const TimestampOrders &a) const
	{
		if (timestamp < a.timestamp)
		{
			return true;
		}
		else if (timestamp == a.timestamp)
		{
			if (role > a.role)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}
};

typedef std::vector<TimestampOrders> TimestampOrdersVec;

