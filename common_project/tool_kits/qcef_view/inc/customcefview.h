#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include "QCefView.h"
#include <functional>

#ifdef QCEFVIEW_LIB
#define QCEFVIEW_EXPORT Q_DECL_EXPORT
#else
#define QCEFVIEW_EXPORT Q_DECL_IMPORT
//#pragma comment(lib, "QCefView.lib")
#endif

enum EContextMenu
{
	ECM_Back,
	ECM_Forward,
	ECM_Reload
};

using UrlChangeCb = std::function<void(std::string)>;
using OnLoadEndCb = std::function<void(int)>;
using OnTitleChangeCb = std::function<void(std::string)>;
using OnJsCallBackCb = std::function<void(std::string, std::string)>;

class QCEFVIEW_EXPORT CustomCefView : public QCefView
{
	Q_OBJECT

public:
	CustomCefView(const QString& url, QWidget *parent = 0);
	~CustomCefView();

	void changeColor();
	void RegisterUrlChangeCb(UrlChangeCb);
	void RegisterLoadEndCb(OnLoadEndCb);
	void RegisterTitleChangeCb(OnTitleChangeCb);
	void RegisterJsCallBackCb(OnJsCallBackCb cb);
protected:
	virtual void processQCefUrlRequest(const QString& url) override;

	virtual void processQCefQueryRequest(const QCefQuery& query) override;

	virtual void onLoadStateChange(bool isLoading) override;

	virtual void onLoadStart() override;

	virtual void onLoadEnd(int httpStatusCode) override;

	virtual void onLoadError(int errorCode, const QString& errorMsg, const QString& failedUrl) override;

	virtual void dragEnterEvent(QDragEnterEvent *) override;

	virtual void onUrlChange(const QString& url) override;

	virtual bool excContextMenu(int id) override;

	virtual bool getMenuInfo(int index, int& command, QString& lblName, bool& itemEnable) override;

	virtual void onKeyEvent(const int& key);

	virtual void onTitleChange(const QString& title) override;

	virtual void  onJsCallback(const QString& fun_name, const QString& param);
private:

	UrlChangeCb m_url_change_cb_;
	OnLoadEndCb m_on_load_end_cb_;
	OnTitleChangeCb m_on_title_change_cb_;
	OnJsCallBackCb m_on_js_callback_cb_;

};
#endif // CUSTOMCEFVIEW_H