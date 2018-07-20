#include "log_output_util.h"
#include "base/synchronization/lock.h"


namespace log_output_util
{
	static HANDLE& ConsoleHandleRef()
	{
		static HANDLE s_handle_console = GetStdHandle(STD_OUTPUT_HANDLE);

		return s_handle_console;
	}

	static nbase::NLock* LockPtr()
	{
		static nbase::NLock s_lock;
		
		return &s_lock;
	}

	static std::wstring& FilterString()
	{
		static std::wstring s_filter_str;

		return s_filter_str;
	}

	void OutputDebugTextWithColorAttribute(const std::wstring& txt, WORD attribute)
	{
		if (!FilterString().empty() && txt.find(FilterString()) == txt.npos)
		{
			return;
		}

		nbase::NAutoLock auto_lock(LockPtr());

		HANDLE console_handle = ConsoleHandleRef();

		if (console_handle == NULL)
		{
			return;
		}

		SetConsoleTextAttribute(console_handle, attribute);

		WriteConsoleW(console_handle, &txt[0], txt.length(), nullptr, nullptr);

		SetConsoleTextAttribute(console_handle, DEFAULT_DEBUG_TEXT_COLOR);
	}

	void AllocNewConsole()
	{
		nbase::NAutoLock auto_lock(LockPtr());

		::FreeConsole();
		::AllocConsole();

		ConsoleHandleRef() = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void ClearConsole()
	{
		nbase::NAutoLock auto_lock(LockPtr());

		HANDLE console_handle = ConsoleHandleRef();

		if (console_handle == NULL)
		{
			return;
		}

		COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */
		BOOL bSuccess;
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
		DWORD dwConSize;                 /* number of character cells in  the current buffer */

		/* get the number of character cells in the current buffer */

		bSuccess = GetConsoleScreenBufferInfo(console_handle, &csbi);
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		/* fill the entire screen with blanks */

		bSuccess = FillConsoleOutputCharacter(console_handle, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);

		/* get the current text attribute */

		bSuccess = GetConsoleScreenBufferInfo(console_handle, &csbi);

		/* now set the buffer's attributes accordingly */

		bSuccess = FillConsoleOutputAttribute(console_handle, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

		/* put the cursor at (0, 0) */
		bSuccess = SetConsoleCursorPosition(console_handle, coordScreen);
	}

	void HandleConsoleCommand(const std::wstring& command)
	{
		if (command == L"c" || command == L"C")
		{
			
		}
		else
		{
			FilterString() = command;
		}

		ClearConsole();
	}
}