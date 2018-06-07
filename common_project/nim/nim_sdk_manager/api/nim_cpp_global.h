﻿/** @file nim_cpp_global.h
  * @brief NIM SDK提供的一些全局接口
  * @copyright (c) 2015-2016, NetEase Inc. All rights reserved
  * @author towik, Oleg
  * @date 2015/2/1
  */

#ifndef _NIM_SDK_CPP_GLOBAL_H_
#define _NIM_SDK_CPP_GLOBAL_H_

#include <functional>
#include <string>
#include "nim\nim_c_sdk\util\nim_base_types.h"

/**
* @namespace nim
* @brief namespace nim
*/
namespace nim
{
#include "nim\nim_c_sdk\include\nim_global_def.h"


///** @brief 网络探测回调信息 */
//struct NetDetectCbInfo
//{
//	int32_t res_code_;					/**< 返回的错误码 */
//	int32_t loss_;
//	int32_t rtt_max_;
//	int32_t rtt_min_;
//	int32_t rtt_avg_;
//	int32_t rtt_mdev_;
//	std::string expand_info_;
//
//	NetDetectCbInfo()
//	{
//		res_code_ = 0;
//		loss_ = 0;
//		rtt_max_ = 0;
//		rtt_min_ = 0;
//		rtt_avg_ = 0;
//		rtt_mdev_ = 0;
//	}
//};
/** @class Global
  * @brief NIM SDK提供的一些全局接口
  */
class Global
{
public:
	typedef std::function<void(int log_level,const std::string& log)> SDKLogCallback; 
	//typedef std::function<void(int, NetDetectCbInfo)> NetDetectCallback;	/**< 网络探测回调模板 */

public:
	/** @fn void FreeStrBuf(char *str)
	* 释放SDK内部分配的内存
	* @param[in] str 由SDK内部分配内存的字符串
	* @return void 无返回值
	*/
	static void FreeStrBuf(char *str);

	/** @fn  void FreeBuf(void *data)
	* 释放SDK内部分配的内存
	* @param[in] data 由SDK内部分配的内存
	* @return void 无返回值
	*/
	static void FreeBuf(void *data);
	
	/** @fn void SetProxy(NIMProxyType type, const std::string& host, int port, const std::string& user, const std::string& password)
    * 设置SDK统一的网络代理。不需要代理时，type设置为kNIMProxyNone，其余参数都传空字符串（端口设为0）。有些代理不需要用户名和密码，相应参数也传空字符串。   
    * @param[in] type 代理类型，见NIMProxyType定义
    * @param[in] host 代理地址
    * @param[in] port 代理端口
    * @param[in] user 代理用户名
    * @param[in] password 代理密码
    * @return void 无返回值
    */
	static void SetProxy(NIMProxyType type, const std::string& host, int port, const std::string& user, const std::string& password);

	///** @fn void NetDetect(NIMNetDetectType type, const std::string& app_key, NetDetectCallback cb)
	//* 网络探测
	//* @param[in] type 探测类型NIMNetDetectType
	//* @param[in] app_key 应用标识 APP key
	//* @param[in] cb 操作结果的回调函数
	//* @return void 无返回值
	//* @note 错误码	200:成功
	//*				0:流程错误
	//*				400:非法请求格式
	//*				417:请求数据不对
	//*				606:ip为内网ip
	//*				607:频率超限
	//*				20001:探测类型错误
	//*				20002:ip错误
	//*				20003:sock错误
	//*/
	//static void NetDetect(NIMNetDetectType type, const std::string& app_key, NetDetectCallback cb);

	/** @fn void nim_client_reg_kickout_other_client_cb(const char *json_extension, nim_json_transport_cb_func cb, const void *user_data)
	* 注册输出sdk log回调
	* @param[in] json_extension json扩展参数（备用，目前不需要）
	* @param[in] cb 输出sdk log的回调函数， nim_sdk_log_cb_func回调函数定义见nim_global_def.h
	* @param[in] user_data APP的自定义用户数据，SDK只负责传回给回调函数cb，不做任何处理！
	* @return void 无返回值
	*/
	static void SetSDKLogCallback(const std::string&json_extension,const SDKLogCallback& callback);
};

} 

#endif //_NIM_SDK_CPP_GLOBAL_H_