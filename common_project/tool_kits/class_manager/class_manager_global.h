#pragma once
#include <wtypes.h>
#include <string>
#include <stdint.h> 
#include <vector>

#define CHANNEL_MESSAGE_TIME 180 // 频道内消息180ms上传一次
#define HEART_BEAT_TIME 15 // 课程心跳 30 s/次 
#define CHECK_START_COURSE_TIME 10 // 检测学生端是否收到开始命令， 10s等待时间
#define HERAT_BEAT_FAIL 4 // 心跳出错次数
#define COURSE_SECOND_TIMER 1 // 计时器
#define AGORA_UNCONNECT_MSG_TIME 3 // 检测到声网断开，每隔3s检测是否已经连上
#define AGORA_RECONNECT_TIME 60 // 声网断掉，重连36次就退出上课(3分钟)
#define AGORA_PER_RECONNECT_TIME 5 // 每隔5s重连一次
#define AGORA_RECORD_FILE_TIME 3 // 声网录制文件每隔3s写一次
#define COUSRE_END_SERVER_ERROR 207 // 服务器课程结束的报错
#define EXTEND_THUM_NUM 1 // 虚拟加号视图
#define AGORA_CAMERA_ERROR 1003  // 声网打开摄像头失败
#define CHECK_CPU_TIME 3  // 每隔3分钟，检测一下
#define CHECK_THUMBNAIL_TIME 2  // 每隔2s，绘制一下当前页面的缩略图
#define COUSRE_END_STU_BALANCE_ERROR 1007 //学生余额不足

#define REQUEST_DATA_OBJECT_TEA   "tea"
#define REQUEST_DATA_OBJECT_STU   "stu_normal"
#define REQUEST_DATA_OBJECT_OBS   "stu_observe"

enum One2OneCommandType
{
	COMMAND_VERSION = 0,

	// 老师书写命令
	TEA_DRAW_START = 1,
	TEA_DRAW_MOVE = 2,
	TEA_DRAW_END = 3,

	TEA_CHANGE_COLOR = 4, // 老师端换笔的颜色
	TEA_CHANGE_BOARD_PAGER = 5, // 老师端白板换页

	UPLOAD_IMG = 6, // 学生或老师端上传图片
	TEA_START_COURSE = 7, //开始上课
	END_COURSE = 8, //结束上课
	TEA_CHANGE_PRICE = 9, //修改价格
	STU_CHARGE_BALANCE = 10, //账户充值
	TEA_CHANGE_PHOTO_PAGE = 11, //改变照片页码
	STU_RECV_COURSE_START = 12, //接受开始上课
	EXCHANGE_DATA = 13,  //交换数据

	TEA_PPT_CHG_PAGE = 14, //老师 ppt 换页

	STU_REVOKE = 15, //学生端撤销
	TEA_REVOKE = 16, //老师端撤销
	STU_CLEAN = 17, //学生端清除
	TEA_CLEAN = 18, // 老师端清除

	AUDIO_START = 19, //录音开始的时间， 记录的是 xxxx.xx 秒
	TIMES_SIGN = 20, //时间标志， 通常是 0.xx 秒

	TEA_UPLOAD_PPT_FILES = 21, //ppt 上传图片

	// 学生翻页命令
	STU_CHANGE_BOARD_PAGER = 22, // 学生端切换白板页码
	STU_CHANGE_PHOTO_PAGER = 23, // 学生端切换图片页码
	STU_CHANGE_PPT_PAGER = 24,  // 学生端切换 ppt 页码

	// 学生书写命令
	STU_DRAW_START = 25,
	STU_DRAW_MOVE = 26,
	STU_DRAW_END = 27,

	STU_AUDIO_START = 28,   // 学生录音开始时间
	TEA_RESET_PPT = 29, // 老师重置ppt
	COMMAND_START_OBSERVE = 30, //学生进入旁听 
	COMMAND_OBSERVE_DATA = 31,	//学生上传白板数据返回给旁听
	COMMAND_EXCHANGE_USER_INFO = 32,	//交互用户信息

	//班课
	TEA_CLASS_UPLOAD_PPT_FILES = 33,  //班课，老师上传PPT
	TEA_CLASS_CHANGE_PPT_PAGER = 34,  //班课，老师切换ppt，切换到某个ppt 的某张页面
	TEA_CLASS_CLEAN_PPT = 35,		  //班课，老师清除ppt
	TEA_CLASS_BEGIN_UPLOAD = 36,	  //表明老师开始上传数据
	TEA_CLASS_END_UPLOAD = 37,		  //表明老师结束了上传数据

	TEA_ROTATE_IMAGE = 38, // 老师图片的旋转
	TEA_UPLOAD_IMAGE_FILES = 39, // 老师上传错题集image

	TEA_ERASER_DRAW_DOWN = 43,  // 老师橡皮擦开始
	TEA_ERASER_DRAW_MOVE = 44,  // 老师橡皮擦移动
	TEA_ERASER_DRAW_UP = 45,    // 老师橡皮擦结束

	REPLAY_H5DOC = 46,    // 播放h5ppt
	TEA_UPLOAD_H5PPT_FILE = 47, // 动效ppt地址
	TEA_CHANGE_H5PPT_PAGE_STEP = 48, // 当前动效ppt的页码的第几个动态效果

	TEA_ADD_VIDEO_BOARD = 49,  // 添加视频白板
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
	STU_NEED_TEA_UID = 0,   //学生请求获取老师uid

};

struct OrderInfo
{
	int64_t index_;    //命令唯一标识
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
	int role;			//学生0 和老师1 
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

