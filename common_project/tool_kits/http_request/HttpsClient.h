#pragma once

#include <thread>
#include <memory>
#include <unordered_map>
#include <list>
#include <string>
#include <stdint.h>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "base/memory/singleton.h"
#include "libyuv/include/libyuv/basic_types.h"
#include "curl/curl.h"

class CHttpRequest;

using CHttpRequestPtr		= std::shared_ptr<CHttpRequest>;
using HttpCallBack			= std::function<void(const CHttpRequestPtr &)>;
using CHttpRequestMap		= std::unordered_map<int64_t, CHttpRequestPtr>;
using CHttpRequestCollect	= std::list<CHttpRequestPtr>;

class CHttpClient
{
public:
	SINGLETON_DEFINE(CHttpClient);

	CHttpClient(void);
	virtual ~CHttpClient(void);

public:
	/**
	* @brief HTTP 异步Post请求
	* @param strUrl 请求的Url地址
	* @param strPost Post数据
	* @param cb 回调函数
	* @return
	*/
	int64_t AsyncPost(const std::string & strUrl, const std::string & strPost, curl_slist* sl_headers, HttpCallBack cb);
	int AsyncGet(const std::string & strUrl, curl_slist* sl_headers, HttpCallBack cb);
	int AsyncPut(const std::string & strUrl, curl_slist* sl_headers, HttpCallBack cb);
	int AsyncDelete(const std::string & strUrl, curl_slist* sl_headers, HttpCallBack cb);

	/**
	* @brief HTTP GET请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/


	int Get(const std::string & strUrl, std::string & strResponse);
	/**
	* @brief HTTP POST请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strPost 输入参数,使用如下格式para1=val&para2=val2&…
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/
	int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);

	void SetDebug(bool bDebug);

	void SetConnectTimeOut(float t){ m_connectTimeOut = t; }
	void SetRequestTimeOut(float t){ m_requestTimeOut = t; }
	void Stop();

private:
	void CreateTheads();

	void CacheAndNotify(CHttpRequestPtr&& request);

	void CurlWorkThreadFunc();
	void CallbackThreadFunc();

	/**
	* @brief HTTPS POST请求,无证书版本
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strPost 输入参数,使用如下格式para1=val&para2=val2&…
	* @param strResponse 输出参数,返回的内容
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @return 返回是否Post成功
	*/
	int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);

	/**
	* @brief HTTPS GET请求,无证书版本
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strResponse 输出参数,返回的内容
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @return 返回是否Post成功
	*/
	int Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);

private:
	bool										m_bDebug{ false };

	float										m_connectTimeOut{ 20.0f };
	float										m_requestTimeOut{ 30.0f };

	std::atomic<bool>							m_bThreadsCreated{ false };

	CHttpRequestCollect							m_NewRequestCollect;
	std::mutex									m_NewRequestMutex;
	std::condition_variable						m_NewRequestCV;

	CHttpRequestCollect							m_FinishRequestCollect;
	std::mutex									m_FinishRequestMutex;
	std::condition_variable						m_FinishRequestCV;

	std::vector<std::shared_ptr<std::thread>>	m_curlWorkThreadPool;
	std::vector<std::shared_ptr<std::thread>>	m_callbackThreadPool;
	std::atomic<bool>							m_bQuit{ false };
};

class CHttpRequest
{
public:
	CHttpRequest(int http_type, curl_slist* sl_headers, const std::string& url, const std::string& postData, HttpCallBack cb);

	~CHttpRequest(){};

	void SetResponse(const std::string& resp)
	{
		m_response = resp;
	}

	void ClearSlHeader()
	{
		if (sl_headers_)
		{
			curl_slist_free_all(sl_headers_);
		}
	}

	int64_t uuid;
	int http_type_;
	std::string m_url;
	std::string m_postData;
	std::string m_response;
	HttpCallBack m_callBack;
	curl_slist* sl_headers_;//增加http header
	CURLcode m_state; //请求结果 返回0成功其他失败
};

