#pragma once
#include "base\memory\singleton.h"

#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include "websocket\easywsclients.hpp"
#include "websocket\easywsclient.hpp"
#include <list>
#include <functional>
#include "base\synchronization\lock.h"

using WsMessageCb = std::function<void(std::string)>;
using WsConnectCloseCb = std::function<void()>;

typedef std::list<WsMessageCb> WsMessageCbList;
typedef std::list<WsConnectCloseCb> WsConnectCloseCbList;

using easywsclient::WebSocket;
using easywsclient::WebSockets;
//Ã⁄—∂websockets Ω”»Î
class WsService
{
public:
	SINGLETON_DEFINE(WsService);

	WsService();
	~WsService();

	void OnConnect(std::string url, std::string origin = std::string());
	void OnClose();
	void RegisterMessageCallback(WsMessageCb cb);
	void RegisterConnectCloseCallback(WsConnectCloseCb cb);
	void OnSendMessage(std::string message);
	void OnSendPing();
	void OnClearMessageCbList();
	void OnClearConnectCloseCbList();

private:
	void OnReceiveMessage(const std::string & message);
	void OnConnectClose();
	
private:
	WebSocket::pointer ws;
	WebSockets::pointer wss;
	bool b_connecting_;
	bool b_wss_;
	std::string url_;
	WsMessageCbList message_cb_list_;
	WsConnectCloseCbList connect_close_list_;
	nbase::NLock message_cb_list_lock_;
	nbase::NLock connect_close_list_lock_;
	nbase::NLock message_data_lock_;
};