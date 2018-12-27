#include "HttpsClient.h"
#include <stdio.h>
#include "log\log.h"
#include "..\common\system\system_tool.h"
#include "base\system\api_setting.h"
#include "base\macros.h"
#include <string>
#include "base\util\string_number_conversions.h"

#if (defined(WIN32) || defined(WIN64)) && defined(_DEBUG)
#include <windows.h>  
#include <process.h> 
#include <sstream>
#define DEBUG_CURL_THREAD
#define LOG_THREAD_WAKE_UP
#endif

#ifdef _DEBUG
#pragma comment(lib, "my_libcurld.lib")
#else
#pragma comment(lib, "my_libcurl.lib")

#endif

#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )

using namespace std;

#define  HTTP_TYPE_GET				0
#define  HTTP_TYPE_POST				1
#define  HTTP_TYPE_PUT				2
#define  HTTP_TYPE_DELETE			3

#define  CURL_SELECT_ERR			(-1)

#define  MILLISECONDS_TO_WAIT_CURL	50

#ifdef DEBUG_CURL_THREAD
const DWORD kVCThreadNameException = 0x406D1388;

typedef struct tagTHREADNAME_INFO {
	DWORD dwType;  // Must be 0x1000.  
	LPCSTR szName;  // Pointer to name (in user addr space).  
	DWORD dwThreadID;  // Thread ID (-1=caller thread).  
	DWORD dwFlags;  // Reserved for future use, must be zero.  
} THREADNAME_INFO;
#endif


CHttpClient::CHttpClient(void)
{
	curl_global_init(CURL_GLOBAL_ALL);

	this->CreateTheads();
}

CHttpClient::~CHttpClient(void)
{
	if (!m_bQuit)
	{
		this->Stop();
	}

	curl_global_cleanup();
}

void CHttpClient::CreateTheads()
{
	bool flag = false;
	if (m_bThreadsCreated.compare_exchange_strong(flag, true))
	{
		int cpuCount = 1;

#ifndef __linux__
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		cpuCount = sysInfo.dwNumberOfProcessors;
#else
		cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif

		int curlWorkThreadCount = cpuCount;
		while (curlWorkThreadCount-- > 0)
		{
			m_curlWorkThreadPool.emplace_back(std::make_shared<std::thread>(std::bind(&CHttpClient::CurlWorkThreadFunc, this)));
		}

		int callbackThreadCount = cpuCount;
		while (callbackThreadCount-- > 0)
		{
			m_callbackThreadPool.emplace_back(std::make_shared<std::thread>(std::bind(&CHttpClient::CallbackThreadFunc, this)));
		}
	}
}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if (itype == CURLINFO_TEXT) {
	}
	else if (itype == CURLINFO_HEADER_IN) {
		DBG_WRAPPER(LOG_DBG(L"[HEADER_IN]{0}") << pData);
	}
	else if (itype == CURLINFO_HEADER_OUT) {
		DBG_WRAPPER(LOG_DBG(L"[HEADER_OUT]{0}") << pData);
	}
	else if (itype == CURLINFO_DATA_IN) {
		DBG_WRAPPER(LOG_DBG(L"[DATA_IN]{0}") << pData);
	}
	else if (itype == CURLINFO_DATA_OUT) {
		DBG_WRAPPER(LOG_DBG(L"[DATA_OUT]{0}") << pData);
	}
	return 0;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = static_cast<std::string*> (lpVoid);
	char* pData = (char*)buffer;
	if (NULL == str || NULL == pData) {
		return 0;
	}

	str->append(pData, size * nmemb);
	return nmemb * size;
}

static size_t OnAsyncWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	// CHttpRequestCollect::iterator* pit = static_cast<CHttpRequestCollect::iterator*> (lpVoid);
	string* pStr = static_cast<std::string*>(lpVoid);
	char* pData = (char*)buffer;
	if (NULL == pStr || NULL == pData) {
		return 0;
	}

	pStr->append(pData, size * nmemb);
	return nmemb * size;
}

static size_t DefaultApiHeaderCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	return nSize * nMemByte;
}

static size_t put_read_callback(char *ptr, size_t size, size_t nmemb, void *stream)
{
	return 0;
}

int CHttpClient::Get(const std::string & strUrl, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if (NULL == curl) {
		return CURLE_FAILED_INIT;
	}
	if (m_bDebug) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	/**
	* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_connectTimeOut);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_requestTimeOut);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_CAINFO, "all.pem");    //设置一个证书文件  

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int CHttpClient::AsyncGet(const std::string & strUrl, curl_slist* sl_headers, HttpCallBack cb)
{
	this->CacheAndNotify(std::make_shared<CHttpRequest>(HTTP_TYPE_GET, sl_headers, strUrl, string(""), cb));

	return 0;
}

int CHttpClient::AsyncPut(const std::string & strUrl, curl_slist* sl_headers, HttpCallBack cb)
{
	this->CacheAndNotify(std::make_shared<CHttpRequest>(HTTP_TYPE_PUT, sl_headers, strUrl, string(""), cb));

	return 0;
}

int CHttpClient::AsyncDelete(const std::string & strUrl, curl_slist* sl_headers, HttpCallBack cb)
{
	this->CacheAndNotify(std::make_shared<CHttpRequest>(HTTP_TYPE_DELETE, sl_headers, strUrl, string(""), cb));

	return 0;
}

int64_t CHttpClient::AsyncPost(const std::string & strUrl, const std::string & strPost, curl_slist* sl_headers, HttpCallBack cb)
{
	CHttpRequestPtr ptr = std::make_shared<CHttpRequest>(HTTP_TYPE_POST, sl_headers, strUrl, strPost, cb);

	auto uuid = ptr->uuid;

	this->CacheAndNotify(std::move(ptr));

	return uuid;
}

void  CHttpClient::CacheAndNotify(CHttpRequestPtr&& request)
{
	std::lock_guard<std::mutex> lock(m_NewRequestMutex);

	m_NewRequestCollect.emplace_back(std::move(request));

	m_NewRequestCV.notify_one();
}

int CHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if (NULL == curl) {
		return CURLE_FAILED_INIT;
	}
	if (m_bDebug) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, m_connectTimeOut);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_requestTimeOut);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_CAINFO, "all.pem");    //设置一个证书文件

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}
///////////////////////////////////////////////////////////////////////////////////////////////

void CHttpClient::SetDebug(bool bDebug)
{
	m_bDebug = bDebug;
}

void CHttpClient::Stop()
{
	m_bQuit = true;
	
	m_NewRequestCV.notify_all();
	m_FinishRequestCV.notify_all();

	for (auto& threadPtr : m_curlWorkThreadPool)
	{
		if (threadPtr && threadPtr->joinable())
		{
			threadPtr->join();
		}
	}
	m_curlWorkThreadPool.clear();

	for (auto& threadPtr : m_callbackThreadPool)
	{
		if (threadPtr && threadPtr->joinable())
		{
			threadPtr->join();
		}
	}
	m_callbackThreadPool.clear();

	m_bThreadsCreated = false;
}

static int curl_multi_select(CURLM * curl_m)
{
	int ret = 0;

	struct timeval timeout_tv;
	fd_set fd_read;
	fd_set fd_write;
	fd_set fd_except;
	int max_fd = -1;

	// 注意这里一定要清空fdset,curl_multi_fdset不会执行fdset的清空操作  //
	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);
	FD_ZERO(&fd_except);

	// 设置select超时时间  //
	timeout_tv.tv_sec = 0;
	timeout_tv.tv_usec = 50000;

	// 获取multi curl需要监听的文件描述符集合 fd_set //
	CURLMcode mc = curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);

	if (mc != CURLM_OK) 
	{
		fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
		return CURL_SELECT_ERR;
	}

	/**
	* When max_fd returns with -1,
	* you need to wait a while and then proceed and call curl_multi_perform anyway.
	* How long to wait? I would suggest 100 milliseconds at least,
	* but you may want to test it out in your own particular conditions to find a suitable value.
	*/
	if (-1 == max_fd) 
	{
		return CURL_SELECT_ERR;
	}

	/**
	* 执行监听，当文件描述符状态发生改变的时候返回
	* 返回0，程序调用curl_multi_perform通知curl执行相应操作
	* 返回-1，表示select错误
	* 注意：即使select超时也需要返回0，具体可以去官网看文档说明
	*/
	int ret_code = ::select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);
	switch (ret_code) 
	{
	case -1:
		ret = CURL_SELECT_ERR;
		break;
	case 0:
	default:
		ret = 0;
		break;
	}

	return ret;
}

static CURL* curl_easy_handler(int http_type, std::string & sUrl, std::string & strPost, CHttpRequestCollect::iterator it, uint32 requestTimeOut, uint32 connectTimeOut)
{
	CHttpRequestPtr request_ptr = (*it);

	std::string ua_("");

	std::string reg_dir = "SOFTWARE\\";
	std::string full_path = reg_dir.append(ApiSetting::register_dir_);
	std::string source = nbase::UTF16ToUTF8(systembase::GetRegValue(nbase::UTF8ToUTF16(reg_dir), L"app_source"));

	ua_.append("os/").append(systembase::GetOSVersionString());
	ua_.append(" da_version/").append(ApiSetting::app_inner_version_);
	ua_.append(" os_version/").append(nbase::IntToString(systembase::GetOSVersion()));
	ua_.append(" app_version/").append(ApiSetting::app_version_);
	ua_.append(" device_code/").append(systembase::GetMac());
	ua_.append(" device_model/").append("pc");
	ua_.append(" api_version/").append(ApiSetting::api_server_version_);
	ua_.append(" channel/").append(source);
	ua_.append(" browser/").append("PCWebView");

	CURL * curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, requestTimeOut*1000.0f);

	if (http_type == HTTP_TYPE_POST)
	{
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strPost.length());
	}
	else if (http_type == HTTP_TYPE_PUT)
	{
		curl_easy_setopt(curl, CURLOPT_PROXY, "");
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, put_read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, "");
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, 0);	//size为0时不会调用read callback
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
	}
	else if (http_type == HTTP_TYPE_DELETE)
	{
		curl_easy_setopt(curl, CURLOPT_PROXY, "");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, DefaultApiHeaderCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &(request_ptr->m_response));
	}
	
	// write function //
	curl_slist* sl_headers = request_ptr->sl_headers_;
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, sl_headers);// 改协议头
	
	curl_easy_setopt(curl, CURLOPT_USERAGENT, ua_.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(request_ptr->m_response));
	curl_easy_setopt(curl, CURLOPT_PRIVATE, it);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, connectTimeOut*1000.0f);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_CAINFO, "all.pem");    //设置一个证书文件  

	return curl;
}

#ifdef DEBUG_CURL_THREAD
static void SetThreadName(const std::string& name, DWORD threadId)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name.c_str();
	info.dwThreadID = threadId;
	info.dwFlags = 0;

	__try
	{
		RaiseException(kVCThreadNameException, 0, sizeof(info) / sizeof(DWORD), reinterpret_cast<DWORD_PTR*>(&info));
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
#endif

void CHttpClient::CurlWorkThreadFunc()
{
#ifdef DEBUG_CURL_THREAD
	DWORD threadId = GetCurrentThreadId();
	std::stringstream ss;
	ss << "CHttpClient::CurlWorkThreadFunc_" << threadId;

	SetThreadName(ss.str(), threadId);
#endif

	CURLM* handleMultiCurl = curl_multi_init();

	CHttpRequestMap doingRequestCollect;

	while (m_bQuit == false)
	{
		/*
		* 调用curl_multi_perform函数执行curl请求
		* url_multi_perform返回CURLM_CALL_MULTI_PERFORM时，表示需要继续调用该函数直到返回值不是CURLM_CALL_MULTI_PERFORM为止
		* running_handles变量返回正在处理的easy curl数量，running_handles为0表示当前没有正在执行的curl请求
		*/
		int running_handles = 0;
		while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(handleMultiCurl, &running_handles));

		if ((running_handles == 0) || (CURL_SELECT_ERR !=  curl_multi_select(handleMultiCurl)))
		{
			int msgs_left = 0;
			CURLMsg* msg = NULL;

			while (msg = curl_multi_info_read(handleMultiCurl, &msgs_left))
			{
				if (CURLMSG_DONE == msg->msg)
				{
					CHttpRequestMap::iterator itd = doingRequestCollect.find((int64_t)msg->easy_handle);
					if (itd != doingRequestCollect.end())
					{
						(itd->second)->m_state = msg->data.result;

						{
							std::lock_guard<std::mutex> lock(m_FinishRequestMutex);
							m_FinishRequestCollect.push_back(itd->second);

							m_FinishRequestCV.notify_one();
						}

						doingRequestCollect.erase(itd);

						curl_multi_remove_handle(handleMultiCurl, msg->easy_handle);
						curl_easy_cleanup(msg->easy_handle);
					}
				}
				else
				{
					LOG_ERR(L"bad request, result:") << msg->msg;
				}
			}
		}

		CHttpRequestCollect tmp;

		{
			std::unique_lock<std::mutex> lock(m_NewRequestMutex);

			if (running_handles > 0 || !doingRequestCollect.empty())
			{
				m_NewRequestCV.wait_for(lock, std::chrono::milliseconds(MILLISECONDS_TO_WAIT_CURL));
			}
			else
			{
				m_NewRequestCV.wait(lock, [this]{ return !m_NewRequestCollect.empty() || m_bQuit; });

#ifdef LOG_THREAD_WAKE_UP
				LOG_INFO(L"CHttpClient::CurlWorkThreadFunc_{0} Wakeup") << threadId;
#endif
			}

			if (m_bQuit)
			{
				break;
			}

			tmp.swap(m_NewRequestCollect);
		}

		if (!tmp.empty())
		{
			CHttpRequestCollect::iterator it = tmp.begin();
			CHttpRequestCollect::iterator itend = tmp.end();
			for (; it != itend; ++it)
			{
				const auto& request = *it;

				CURL* hUrl = curl_easy_handler(request->http_type_, request->m_url, request->m_postData, it, m_requestTimeOut, m_connectTimeOut);

				if (hUrl == NULL)
				{
					continue;
				}

				doingRequestCollect.emplace((int64_t)hUrl, request);

				curl_multi_add_handle(handleMultiCurl, hUrl);
			}
		}
	}

	curl_multi_cleanup(handleMultiCurl);
}

void CHttpClient::CallbackThreadFunc()
{
#ifdef DEBUG_CURL_THREAD
	DWORD threadId = GetCurrentThreadId();
	std::stringstream ss;
	ss << "CHttpClient::CallbackThreadFunc_" << threadId;

	SetThreadName(ss.str(), threadId);
#endif

	while (m_bQuit == false)
	{
		CHttpRequestCollect tmp;

		{		
			std::unique_lock<std::mutex> lock(m_FinishRequestMutex);

			m_FinishRequestCV.wait(lock, [this]{ return !m_FinishRequestCollect.empty() || m_bQuit; });

#ifdef LOG_THREAD_WAKE_UP
			LOG_INFO(L"CHttpClient::CallbackThreadFunc_{0} Wakeup") << threadId;
#endif

			if (m_bQuit) 
			{
				break;
			}

			tmp.swap(m_FinishRequestCollect);
		}

		if (! tmp.empty())
		{
			for (const auto& request : tmp)
			{
				request->m_callBack(request);
			}
		}
	}
}
//------------------------------------------------------------------------------------

CHttpRequest::CHttpRequest(int http_type, curl_slist* sl_headers, const std::string& url, const std::string& postData, HttpCallBack cb) : 
				http_type_(http_type), sl_headers_(sl_headers), m_url(url), m_postData(postData), m_callBack(cb), m_state(CURL_LAST)
{
	static std::atomic_int64_t httpRequestUUID(0);

	uuid = httpRequestUUID++;
}