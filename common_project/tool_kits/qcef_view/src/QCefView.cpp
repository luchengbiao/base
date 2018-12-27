#pragma region qt_headers
#include <QVBoxLayout>
#include <QWindow>
#include <QResizeEvent>
#include <QVariant>

#include <QApplication>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_sandbox_win.h>
#pragma endregion cef_headers

#include "inc/QCefQuery.h"
#include "inc/QCefView.h" 
#include "inc/QCefEvent.h"
#include "CCefWindow.h"
#include "CCefCookie.h"
#include "QCefViewBrowserHandler.h"
#include "cef_manager.h"

//////////////////////////////////////////////////////////////////////////
QCefView::QCefView(const QString url, QWidget* parent)
	: QWidget(parent)
	, pCefWindow_(NULL)
{
	this->setAttribute(Qt::WA_DeleteOnClose);
	CefManager::GetInstance()->AddCefPage(this);

	QGridLayout* layout = new QGridLayout;
	layout->setContentsMargins(0, 0, 0, 0);

	/*
	 * Here we must create a QWidget as a wrapper 
	 * to encapsulate the QWindow
	 */
	pCefWindow_ = new CCefWindow(url, this, window()->windowHandle());
	QWidget* windowContainer = createWindowContainer(pCefWindow_, this);
	if (windowContainer)
	{
		layout->addWidget(windowContainer);
	}

	setLayout(layout);
}

QCefView::~QCefView()
{
	DEBUG_FUNC();
	CefManager::GetInstance()->SubCefPage(this);
	pCefWindow_->Close();
}

void QCefView::Run()
{
	pCefWindow_->Run();
}

QObject* QCefView::getInvoker(const QString& id)
{
	std::map<QString, QObject*>::iterator iter = mmapInvoker.find(id);
	if (iter != mmapInvoker.end())
	{
		return mmapInvoker[id];
	}
	else
	{
		return NULL;
	}
}

void QCefView::registerInvoker(const QString& id, QObject* invoker)
{
	std::map<QString, QObject*>::iterator iter = mmapInvoker.find(id);
	if (iter != mmapInvoker.end())
	{
		mmapInvoker.erase(id);
	}
	mmapInvoker[id] = invoker;
}

void QCefView::getUrlCookie(const QString& url, QList<TCefCookie>& cookieList)
{
	pCefWindow_->GetUrlCookie(url, cookieList);
}

void QCefView::getAllCookie(QList<TCefCookie>& cookieList)
{
	pCefWindow_->GetAllCookie(cookieList);
}

void QCefView::setCookie(const QString& url, const QString& key, const QString& value)
{
	pCefWindow_->SetCookie(url, key, value);
}

void QCefView::runJavaScript(const QString & scriptSource)
{
	pCefWindow_->runJavaScript(scriptSource);
}

void QCefView::processQCefUrlRequest(const QString& url)
{

}

void QCefView::processQCefQueryRequest(const QCefQuery& query)
{

}

void QCefView::navigateToString(const QString& content, const QString& url)
{
	if (pCefWindow_)
	{
		CefString strContent;
		strContent.FromString(content.toLatin1().data());
		CefString strUrl;
		strUrl.FromString(url.toLatin1().data());
		pCefWindow_->cefViewHandler()->GetBrowser()->GetMainFrame()->LoadString(strContent, strUrl);
	}
}

void QCefView::navigateToUrl(const QString& url)
{
	if (pCefWindow_)
	{
		CefString strUrl;
		strUrl.FromString(url.toLatin1().data());
		if (pCefWindow_->cefViewHandler())
		{
			if (pCefWindow_->cefViewHandler()->GetBrowser())
			{
				if (pCefWindow_->cefViewHandler()->GetBrowser()->GetMainFrame())
				{
					pCefWindow_->cefViewHandler()->GetBrowser()->GetMainFrame()->LoadURL(strUrl);
				}
			}
		}
	}
}

bool QCefView::browserCanGoBack()
{
	if (pCefWindow_)
	{
		return pCefWindow_->cefViewHandler()->GetBrowser()->CanGoBack();
	}
	return false;
}

bool QCefView::browserCanGoForward()
{
	if (pCefWindow_)
	{
		return pCefWindow_->cefViewHandler()->GetBrowser()->CanGoForward();
	}
	return false;
}

void QCefView::browserGoBack()
{
	if (pCefWindow_)
	{
		pCefWindow_->cefViewHandler()->GetBrowser()->GoBack();
	}
}

void QCefView::browserGoForward()
{
	if (pCefWindow_)
	{
		pCefWindow_->cefViewHandler()->GetBrowser()->GoForward();
	}
}

bool QCefView::browserIsLoading()
{
	if (pCefWindow_)
	{
		return pCefWindow_->cefViewHandler()->GetBrowser()->IsLoading();
	}
	return false;
}

void QCefView::browserReload()
{
	if (pCefWindow_)
	{
		pCefWindow_->cefViewHandler()->GetBrowser()->Reload();
	}
}

void QCefView::browserStopLoad()
{
	if (pCefWindow_)
	{
		pCefWindow_->cefViewHandler()->GetBrowser()->StopLoad();
	}
}

bool QCefView::triggerEvent(int frameId, const QString& name, const QCefEvent& event)
{
	if (!name.isEmpty())
	{
		if (pCefWindow_)
		{
			auto frame = pCefWindow_->cefViewHandler()->GetBrowser()->GetFrame(frameId);
			if (frame)
			{
				return sendEventNotifyMessage(frameId, name, event);
			}
		}
	}
	return false;
}

bool QCefView::broadcastEvent(const QString& name, const QCefEvent& event)
{
	if (!name.isEmpty())
	{
		if (pCefWindow_)
		{
			return sendEventNotifyMessage(0, name, event);
		}
	}
	return false;
}

void QCefView::onLoadStateChange(bool isLoading)
{

}

void QCefView::onLoadStart()
{

}

void QCefView::onLoadEnd(int httpStatusCode)
{

}

void QCefView::onLoadError(int errorCode,
	const QString& errorMsg,
	const QString& failedUrl)
{

}

void QCefView::onFullScreen(const bool& full)
{
	DEBUG_FUNC();
	if (full)
	{
		this->setWindowFlags(Qt::Window);
		this->showFullScreen();
		QApplication::setActiveWindow(this);
	} 
	else
	{
		this->setWindowFlags(Qt::SubWindow);
		this->showNormal();
		QApplication::setActiveWindow(this);
	}
}

WId QCefView::getCefWinId()
{
	return (WId)(HWND)(*pCefWindow_);
}

void QCefView::notifyMoveOrResizeStarted()
{
	if (pCefWindow_)
	{
		CefRefPtr<QCefViewBrowserHandler> handler = pCefWindow_->cefViewHandler();
		if (handler)
		{
			CefRefPtr<CefBrowser> browser = handler->GetBrowser();
			if (browser)
			{
				CefRefPtr<CefBrowserHost> host = browser->GetHost();
				if (host)
				{
					host->NotifyMoveOrResizeStarted();
				}
			}
		}
	}
}

bool QCefView::sendEventNotifyMessage(int frameId, const QString& name, const QCefEvent& event)
{
	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(TRIGGEREVENT_NOTIFY_MESSAGE);

	CefRefPtr<CefListValue> arguments = msg->GetArgumentList();

	int idx = 0;
	arguments->SetInt(idx++, frameId);

	CefString eventName = name.toLatin1().data();
	arguments->SetString(idx++, eventName);

	CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();

	CefString cefStr;
	cefStr.FromWString(event.objectName().toStdWString());
	dict->SetString("name", cefStr);

	QList<QByteArray> keys = event.dynamicPropertyNames();
	for (QByteArray key : keys)
	{
		QVariant value = event.property(key.data());
		if (value.type() == QMetaType::Bool)
		{
			dict->SetBool(key.data(), value.toBool());
		}
		else if (value.type() == QMetaType::Int || value.type() == QMetaType::UInt)
		{
			dict->SetInt(key.data(), value.toInt());
		}
		else if (value.type() == QMetaType::Double)
		{
			dict->SetDouble(key.data(), value.toDouble());
		}
		else if (value.type() == QMetaType::QString)
		{
			cefStr.FromWString(value.toString().toStdWString());
			dict->SetString(key.data(), cefStr);
		}
		else
		{
			__noop(_T("QCefView"), _T("Unknow Type!"));
		}
	}

	arguments->SetDictionary(idx++, dict);

	return pCefWindow_->cefViewHandler()->TriggerEvent(msg);
}

bool QCefView::responseQCefQuery(int64_t query, 
	bool success, const QString& response, int error)
{
	if (pCefWindow_)
	{
		CefString res;
		res.FromString(response.toLatin1().data());
		return pCefWindow_->cefViewHandler()->ResponseQuery(
			query, success, res, error);
	}
	return false;
}

void QCefView::onUrlChange(const QString& url)
{

}

void QCefView::onKeyEvent(const int& key)
{

}

void QCefView::onTitleChange(const QString& title)
{

}

bool QCefView::excContextMenu(int id)
{
	DEBUG_FUNC();
	return false;
}

bool QCefView::getMenuInfo(int index, int& command, QString& lblName, bool& itemEnable)
{
	command = -1;
	lblName = "";
	itemEnable = false;
	return false;
}

void QCefView::onJsCallback(const QString& fun_name, const QString& param)
{

}
