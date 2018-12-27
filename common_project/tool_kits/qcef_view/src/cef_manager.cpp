#include "cef_manager.h"
#include "QCoreApplication"
#include "QDir"
#include "include\internal\cef_win.h"
#include "QCefViewBrowserApp.h"
#include <map>
#include <QWidget>
#include "base\util\string_util.h"

CefManager* CefManager::GetInstance()
{
	static CefManager cef_manager;
	return &cef_manager;
}

CefManager::CefManager()
{
	browser_count_ = 0;
	is_enable_offset_render_ = true;
}

CefManager::~CefManager()
{
}

bool CefManager::Initialize(std::wstring log_dir, bool is_enable_offset_render /*= true*/)
{
	is_enable_offset_render_ = is_enable_offset_render;

	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	CefMainArgs main_args(hInstance);

	void* sandboxInfo = NULL;
	CefRefPtr<QCefViewBrowserApp> app = new QCefViewBrowserApp();

	// 如果是在子进程中调用，会堵塞直到子进程退出，并且exit_code返回大于等于0
	// 如果在Browser进程中调用，则立即返回-1
	int exit_code = CefExecuteProcess(main_args, app, sandboxInfo);
	if (exit_code >= 0)
		return false;

	QDir ExeDir = QCoreApplication::applicationDirPath();
	ExeDir.setPath(ExeDir.canonicalPath());

	QString strExePath = ExeDir.filePath(RENDER_PROCESS_NAME);
	browser_sub_process_path.FromString(QDir::toNativeSeparators(strExePath).toUtf8().data());

	QString strResPath = ExeDir.filePath(RESOURCE_DIRECTORY_NAME);
	resource_directory_path.FromString(QDir::toNativeSeparators(strResPath).toUtf8().data());

	QDir ResPath(strResPath);
	locales_directory_path.FromString(QDir::toNativeSeparators(ResPath.filePath(LOCALES_DIRECTORY_NAME)).toUtf8().data());

	user_agent.FromString(QCEF_USER_AGENT);

	CefString(&cef_settings_.browser_subprocess_path) = browser_sub_process_path;
	CefString(&cef_settings_.resources_dir_path) = resource_directory_path;
	CefString(&cef_settings_.locales_dir_path) = locales_directory_path;
	CefString(&cef_settings_.user_agent) = user_agent;

	CefString(&cef_settings_.cache_path) = log_dir + L"CefLocalStorage";
	// 设置debug log文件位置
	CefString(&cef_settings_.log_file) = nbase::UTF16ToUTF8(log_dir+L"cef.log");
	// cef2623、2526版本debug模式:在使用multi_threaded_message_loop时退出程序会触发中断
	// 加入disable-extensions参数可以修复这个问题，但是会导致一些页面打开时报错
	// 开启Cef多线程消息循环，兼容nbase库消息循环

	cef_settings_.single_process = false;
	cef_settings_.multi_threaded_message_loop = true;
	cef_settings_.log_severity = LOGSEVERITY_DEFAULT;
	cef_settings_.remote_debugging_port = 7777;
	// cef2623、2526版本debug模式:在使用multi_threaded_message_loop时退出程序会触发中断
	// 加入disable-extensions参数可以修复这个问题，但是会导致一些页面打开时报错
	// 开启Cef多线程消息循环，兼容nbase库消息循环

	// 开启离屏渲染
	cef_settings_.windowless_rendering_enabled = is_enable_offset_render;

#if !defined(SUPPORT_CEF_FLASH)
	cef_settings_.no_sandbox = true;
#endif

	// Initialize CEF.
	if (CefInitialize(main_args, cef_settings_, app, sandboxInfo))
	{
		qDebug() << __FUNCTION__ << "**********info : CefInitialize successfully !";
		return true;
	}
	else
	{
		qDebug() << __FUNCTION__ << "***********CefInitialize error";
		return false;
	}
}

void CefManager::UnInitialize()
{
	CloseAllCefPage();
}

void CefManager::CefShutDown()
{
	CefClearSchemeHandlerFactories();
	CefShutdown();
}

bool CefManager::IsEnableOffsetRender() const
{
	return is_enable_offset_render_;
}

void CefManager::AddBrowserCount()
{
	browser_count_++;
	qDebug() << __FUNCTION__ << browser_count_;
}

void CefManager::SubBrowserCount()
{
	browser_count_--;
	qDebug() << __FUNCTION__ << browser_count_;
}

int CefManager::GetBrowserCount()
{
	return browser_count_;
	qDebug() << __FUNCTION__ << browser_count_;
}

void CefManager::AddCefPage(QWidget* cef_widget)
{
	m_all_cef_page_[(int)(cef_widget)] = cef_widget;
}

void CefManager::SubCefPage(QWidget* cef_page)
{
	m_all_cef_page_.erase((int)(cef_page));
}

void CefManager::CloseAllCefPage()
{
	for (auto iter = m_all_cef_page_.begin(); iter != m_all_cef_page_.end(); ++iter)
	{
		iter->second->close();
	}
	m_all_cef_page_.clear();
}
