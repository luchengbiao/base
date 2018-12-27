#pragma once
#include <map>
#include <mutex>

#pragma region cef_headers
#include <include/wrapper/cef_stream_resource_handler.h>
#include <include/wrapper/cef_message_router.h>
#pragma endregion cef_headers

#include "inc/QCefView.h"
#include "CCefWindow.h"
#include "ICefViewBrowser.h"

class QCefQueryHandler : public CefMessageRouterBrowserSide::Handler, public ICefViewBrowser
{
public:
	QCefQueryHandler(QCefView* host);
	~QCefQueryHandler();

	virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) OVERRIDE;


	virtual void OnQueryCanceled(CefRefPtr<CefBrowser> browser, 
		CefRefPtr<CefFrame> frame, 
		int64 query_id) override;

	bool Response(int64_t query,
		bool success, const CefString& response, int error);

private:
	typedef std::map<int64, CefRefPtr<Callback>> CallbackMap;
	CallbackMap	mapCallback_;
	std::mutex	mtxCallbackMap_;
	QPointer<QCefView> hostWidget_;
};

