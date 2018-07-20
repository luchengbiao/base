#include "windows_manage.h"
#include <assert.h>
#include <WTypes.h>

WindowsManager::WindowsManager()
{
	stop_register_ = false;
	windows_map_.clear();
}

WindowsManager::~WindowsManager()
{
	windows_map_.clear();
}

bool WindowsManager::RegisterWindow(const std::wstring wnd_class_name, const std::wstring wnd_id, QWidget *wnd)
{
	if (IsStopRegister())
	{
		assert(!stop_register_);
		return false;
	}
	WindowsMap::iterator it = windows_map_.find(wnd_class_name);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, QWidget*>::iterator it2 = it->second.find(wnd_id);
		if (it2 != it->second.end())
		{
			//LOG_INFO(L"The window <class name: {0}, id: {1}> has already registered !") << wnd_class_name << wnd_id;
		}
		it->second[wnd_id] = wnd;
	}
	else
	{
		std::map<std::wstring, QWidget*> id_win;
		id_win[wnd_id] = wnd;
		windows_map_[wnd_class_name] = id_win;
	}
	return true;
}

void WindowsManager::UnRegisterWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id, QWidget *wnd)
{
	WindowsMap::iterator it = windows_map_.find(wnd_class_name);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, QWidget*>::iterator it2 = it->second.find(wnd_id);
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
		}
	}
}

QWidget* WindowsManager::GetWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id)
{
	WindowsMap::iterator it = windows_map_.find(wnd_class_name);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, QWidget*>::iterator it2 = it->second.find(wnd_id);
		if (it2 != it->second.end())
		{
			QWidget* wnd = (QWidget*)(it2->second);

			HWND hwnd = (HWND)wnd->winId();
			if (wnd && ::IsWindow(hwnd))
			{
				return wnd;
			}
		}
	}
	return NULL;
}

WindowList WindowsManager::GetAllWindows()
{
	WindowList list;
	WindowsMap::iterator it = windows_map_.begin();
	for (; it != windows_map_.end(); ++it)
	{
		std::map<std::wstring, QWidget*>::iterator it2 = it->second.begin();
		for (; it2 != it->second.end(); ++it2)
		{
			QWidget *wnd = (QWidget *)(it2->second);
			HWND hwnd = (HWND)wnd->winId();
			if (wnd && ::IsWindow(hwnd))
			{
				list.push_back(wnd);
			}
		}
	}
	return list;
}

WindowList WindowsManager::GetWindowsByClassName(std::wstring classname)
{
	WindowList wnd_list;
	WindowsMap::iterator it = windows_map_.find(classname);
	if (it != windows_map_.end())
	{
		std::map<std::wstring, QWidget*>::iterator it2 = it->second.begin();
		for (; it2 != it->second.end(); it2++)
		{
			QWidget* wnd = (QWidget*)(it2->second);
			HWND hwnd = (HWND)wnd->winId();
			if (wnd && ::IsWindow(hwnd))
			{
				wnd_list.push_back(wnd);
			}
		}
	}
	return wnd_list;
}

void WindowsManager::DestroyAllWindows()
{
	SetStopRegister();

	WindowList lst_wnd = GetAllWindows();
	WindowList::iterator it = lst_wnd.begin();
	for (; it != lst_wnd.end(); ++it)
	{
		QWidget* wnd = (QWidget*)*it;
		HWND hwnd = (HWND)wnd->winId();
		if (wnd && ::IsWindow(hwnd))
		{
			::DestroyWindow(hwnd);
		}
	}
}
