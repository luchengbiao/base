#ifndef __LOG_OUTPUT_UTIL_H__
#define __LOG_OUTPUT_UTIL_H__
#include <string>
#include <Windows.h>

namespace log_output_util
{
	void	OutputDebugTextWithColorAttribute(const std::wstring& txt, WORD attribute);

	void    ClearConsole();

	void	HandleConsoleCommand(const std::wstring& command);
}

#define DEFAULT_DEBUG_TEXT_COLOR		(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define RED_DEBUG_TEXT_COLOR			(FOREGROUND_INTENSITY | FOREGROUND_RED)
#define GREEN_DEBUG_TEXT_COLOR			(FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define BLUE_DEBUG_TEXT_COLOR			(FOREGROUND_INTENSITY | FOREGROUND_BLUE)
#define YELLOW_DEBUG_TEXT_COLOR			(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
#define PURPLE_DEBUG_TEXT_COLOR			(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
#define LIGHT_BLUE_DEBUG_TEXT_COLOR		(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);

#endif