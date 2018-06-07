#pragma once
#include <QWidget>
#include <string>
#include "base\memory\singleton.h"



//map<����������map<����id������ָ��>>, ���ͬһ��ֻ��һ�����ڣ�ʹ��������Ϊid
typedef std::map<std::wstring, std::map<std::wstring, QWidget*>> WindowsMap;
typedef std::list<QWidget *> WindowList;

class WindowsManager
{
public:
	SINGLETON_DEFINE(WindowsManager);
	WindowsManager();
	~WindowsManager();

public:
	/**
	* ���ݴ���������idע�ᴰ��
	* @param[in] wnd_class_name ��������
	* @param[in] wnd_id ����id
	* @param[in] wnd ����ָ��
	* @return bool true �ɹ���false ʧ��
	*/
	bool RegisterWindow(const std::wstring wnd_class_name, const std::wstring wnd_id, QWidget *wnd);

	/**
	* ���ݴ���������idע������
	* @param[in] wnd_class_name ��������
	* @param[in] wnd_id ����id
	* @param[in] wnd ����ָ��
	* @return void �޷���ֵ
	*/
	void UnRegisterWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id, QWidget *wnd);


	/**
	* ���ݴ���������id��ȡ����
	* @param[in] wnd_class_name ��������
	* @param[in] wnd_id ����id
	* @return WindowEx* ����ָ��
	*/
	QWidget* GetWindow(const std::wstring &wnd_class_name, const std::wstring &wnd_id);

	/**
	* ��ȡ���д��ڻ�ȡ����
	* @return WindowList �����б�
	*/
	WindowList GetAllWindows();

	/**
	* ���ݴ���������ȡ��Ӧ�����д���
	* @param[in] classname ��������
	* @return WindowList �����б�
	*/
	WindowList GetWindowsByClassName(std::wstring classname);

	/**
	* �������д���
	* @return void �޷���ֵ
	*/
	void DestroyAllWindows();

	/**
	* ���ý�ֹ���ڴ���
	* @param[in] stop �Ƿ��ֹ���ڴ���
	* @return void �޷���ֵ
	*/
	void SetStopRegister(bool stop = true){ stop_register_ = stop; }

	/**
	* �Ƿ��ֹ���ڴ���
	* @return bool true ��ֹ��false ����ֹ
	*/
	bool IsStopRegister(){ return stop_register_; }

	/**
	* ���ݴ���id����һ��Ψһ���ڵĴ���
	* @param[in] window_id ����id
	* @return WindowType* ����ָ��
	*/
	template<typename WindowType>
	static WindowType* SingletonShow(const std::wstring& window_id)
	{
		WindowType *window = (WindowType*)(WindowsManager::GetInstance()->GetWindow(WindowType::kClassName, window_id));
		if (!window)
		{
			window = new WindowType();
			window->Create();
			window->show();
		}
		else
		{
			window->show();
			window->raise();
			window->activateWindow();
		}

		return window;
	}

protected:

private:
	WindowsMap					windows_map_;	//���д���
	bool						stop_register_;	//��ֹ���ڴ���
};

