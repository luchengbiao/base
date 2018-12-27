#pragma once
#include "include\internal\cef_types_wrappers.h"
#include <map>
#ifdef QCEFVIEW_LIB
#define QCEFVIEW_EXPORT Q_DECL_EXPORT
#else
#define QCEFVIEW_EXPORT Q_DECL_IMPORT
//#pragma comment(lib, "QCefView.lib")
#endif
class QCEFVIEW_EXPORT CefManager
{
public:
	static CefManager* GetInstance();
	CefManager(const CefManager&) = delete;
	CefManager& operator = (const CefManager&) = delete;
	CefManager();
	~CefManager();

	bool Initialize(std::wstring log_dir, bool is_enable_offset_render = true);

	void UnInitialize();
	void CefShutDown();

	bool IsEnableOffsetRender() const;

	// 记录浏览器对象的数量
	void AddBrowserCount();
	void SubBrowserCount();
	int	GetBrowserCount();

	void AddCefPage(QWidget*);
	void SubCefPage(QWidget*);

private:
	void CloseAllCefPage();

private:
	CefSettings cef_settings_;

	CefString browser_sub_process_path;

	CefString resource_directory_path;

	CefString locales_directory_path;

	CefString user_agent;

	bool is_enable_offset_render_;

	int browser_count_;

	std::map<int, QWidget*> m_all_cef_page_;
};

