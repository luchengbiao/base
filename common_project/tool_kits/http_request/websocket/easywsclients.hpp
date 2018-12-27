#ifndef EASYWSCLIENT_HPP_WSS_MIOFVASDTNUASZDQPLFD
#define EASYWSCLIENT_HPP_WSS_MIOFVASDTNUASZDQPLFD

// This code comes from:
// https://github.com/dhbaird/easywsclient
//
// To get the latest version:
// wget https://raw.github.com/dhbaird/easywsclient/master/easywsclient.hpp
// wget https://raw.github.com/dhbaird/easywsclient/master/easywsclient.cpp
// 支持wss,不支持ws

#include <string>

namespace easywsclient {

	class WebSockets {
	public:
		typedef WebSockets * pointer;
		typedef enum readyStateValues { CLOSING, CLOSED, CONNECTING, OPEN } readyStateValues;

		// Factories:
		static pointer create_dummy();
		static pointer from_url(const std::string& url, const std::string& origin = std::string());
		static pointer from_url_no_mask(const std::string& url, const std::string& origin = std::string());

		// Interfaces:
		virtual ~WebSockets() { }
		virtual void poll(int timeout = 0) = 0; // timeout in milliseconds
		virtual void send(const std::string& message) = 0;
		virtual void sendPing() = 0;
		virtual void close() = 0;
		virtual readyStateValues getReadyState() const = 0;
		template<class Callable>
		void dispatch(Callable callable) { // N.B. this is compatible with both C++11 lambdas, functors and C function pointers
			struct _Callback : public Callback {
				Callable& callable;
				_Callback(Callable& callable) : callable(callable) { }
				void operator()(const std::string& message) { callable(message); }
			};
			_Callback callback(callable);
			_dispatch(callback);
		}

	protected:
		struct Callback { virtual void operator()(const std::string& message) = 0; };
		virtual void _dispatch(Callback& callable) = 0;
	};

} // namespace easywsclient

#endif /* EASYWSCLIENT_HPP_20120819_MIOFVASDTNUASZDQPLFD */