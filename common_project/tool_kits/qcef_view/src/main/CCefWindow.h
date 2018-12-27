#pragma once
#include <QWindow>
#include <QPointer>

#include <include/cef_app.h>

#include "inc/QCefView.h"
#include "CCefCookie.h"

class QCefViewBrowserHandler;
class QCefViewBrowserApp;

class CCefWindow : public QWindow
{
	Q_OBJECT
#pragma region CEFHWND2QTWIDGET
private:
	typedef std::map<HWND, QPointer<QCefView>> HostWidgetInstanceMap;
	static HostWidgetInstanceMap instance_map_;

public:
	/** Get host Qt widget from native cef browser window handle
	*
	*/
	static QPointer<QCefView> GetHostWidget(HWND h);
#pragma endregion CEFHWND2QTWIDGET

public:
	explicit CCefWindow(const QString& url, QCefView* host, QWindow *parent = 0);
	~CCefWindow();

public:
	/** Get cef view handler associated with current CCefWindow
	 *
	 */
	const CefRefPtr<QCefViewBrowserHandler>& cefViewHandler() const;

	/** Get cef app handler associated with current CCefWindow
	*
	*/
	const CefRefPtr<QCefViewBrowserApp>& cefViewApp() const;

	/** set the global cookie
	*
	*/
	void SetCookie(const QString& url, const QString& key, const QString& value);

	/** get the url of global cookie
	*
	*/
	void GetUrlCookie(const QString& url, QList<TCefCookie>& cookieList);

	/** get all of global cookie
	*
	*/
	void GetAllCookie(QList<TCefCookie>& cookieList);

	void runJavaScript(const QString & scriptSource);

	void InitializeCef();

	void CreateBrowser(const QString& url);

	void Close();
	void Run();
protected:
	void updateCefBrowserWindow();
	virtual void exposeEvent(QExposeEvent *e);
	virtual void resizeEvent(QResizeEvent *e);

public:
	operator HWND();

private:
	HWND								hwndCefBrowser_;		    
	QPointer<QCefView>					pQCefViewWidget_;
	CefRefPtr<QCefViewBrowserHandler>	pQCefViewHandler_;
	CefRefPtr<QCefViewBrowserApp>		pQCefViewApp_;
	CefSettings					        cef_settings_;
};

