#include "ws_service.h"
#include "base/macros.h"

WsService::WsService()
{
	url_ = "";
	ws = nullptr;
	wss = nullptr;
	b_connecting_ = false;
	b_wss_ = false;
}

WsService::~WsService()
{

}

void WsService::OnConnect(std::string url, std::string origin)
{
	if (url.empty())
	{
		return;
	}
	b_connecting_ = true;
	url_ = url;
#ifdef _WIN32
	INT rc;
	WSADATA wsaData;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc) {
		printf("WSAStartup Failed.\n");
		return;
	}
#endif
	int indx = url.find("wss://");

	if (indx != std::string::npos)
	{
		b_wss_ = true;
		wss = WebSockets::from_url(url);
		while (b_connecting_ && wss &&wss->getReadyState() != WebSockets::CLOSED) {
				Sleep(10);
				nbase::NAutoLock auto_lock(&message_data_lock_);
				wss->poll();
				wss->dispatch(std::bind(&WsService::OnReceiveMessage, this, std::placeholders::_1));
		}
		{
			nbase::NAutoLock auto_lock(&message_data_lock_);
			SAFE_DELETE(wss);
		}
	}
	else
	{
		b_wss_ = false;
		ws = WebSocket::from_url(url);
		while (b_connecting_ && ws &&ws->getReadyState() !=WebSocket::CLOSED) {
				Sleep(10);
				nbase::NAutoLock auto_lock(&message_data_lock_);
				ws->poll();
				ws->dispatch(std::bind(&WsService::OnReceiveMessage, this, std::placeholders::_1));	
		}
		{
			nbase::NAutoLock auto_lock(&message_data_lock_);
			SAFE_DELETE(ws);
		}
	}
	OnConnectClose();
#ifdef _WIN32
	WSACleanup();
#endif
}

void WsService::OnClose()
{
	b_connecting_ = false;
}

void WsService::RegisterMessageCallback(WsMessageCb cb)
{
	nbase::NAutoLock auto_lock(&message_cb_list_lock_);
	message_cb_list_.push_back(cb);
}

void WsService::OnReceiveMessage(const std::string & message)
{
	std::string mss = message;

	nbase::NAutoLock auto_lock(&message_cb_list_lock_);
	for (auto cb : message_cb_list_)
	{
		if (cb)
		{
			cb(mss);
		}
	}
}

void WsService::OnSendMessage(std::string message)
{
	nbase::NAutoLock auto_lock(&message_data_lock_);
	if (b_wss_)
	{
		if (wss)
		{
			wss->send(message);
		}
	}
	else
	{
		if (ws)
		{
			ws->send(message);
		}
	}
}

void WsService::OnClearMessageCbList()
{
	nbase::NAutoLock auto_lock(&message_cb_list_lock_);
	message_cb_list_.clear();
}

void WsService::RegisterConnectCloseCallback(WsConnectCloseCb cb)
{
	nbase::NAutoLock auto_lock(&connect_close_list_lock_);
	connect_close_list_.push_back(cb);
}

void WsService::OnClearConnectCloseCbList()
{
	nbase::NAutoLock auto_lock(&connect_close_list_lock_);
	connect_close_list_.clear();
}

void WsService::OnConnectClose()
{
	nbase::NAutoLock auto_lock(&connect_close_list_lock_);
	for (auto cb : connect_close_list_)
	{
		if (cb)
		{
			cb();
		}
	}
}

void WsService::OnSendPing()
{
	nbase::NAutoLock auto_lock(&message_data_lock_);
	if (b_wss_)
	{
		if (wss)
		{
			wss->sendPing();
		}
	}
	else
	{
		if (ws)
		{
			ws->sendPing();
		}
	}
}
