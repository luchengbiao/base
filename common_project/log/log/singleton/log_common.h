#ifndef __LOG_COMMON_H__
#define __LOG_COMMON_H__
#include "../base/log_singleton_with_level.h"
#include "../util/log_output_util.h"

enum class LogCommonLevel
{
	Debug = 0,
	Info,
	Message,
	Warning,
	Error,
	Fatal,
};

class LogCommon : public LogSingletonWithLevel< LogCommon, LogCommonLevel >
{
	virtual void OutputOnDebugWindow(LogCommonLevel lv, const std::wstring& log) override
	{
		WORD attri = DEFAULT_DEBUG_TEXT_COLOR;
		switch (lv)
		{
		case LogCommonLevel::Info:
			attri = GREEN_DEBUG_TEXT_COLOR;
			break;
		case LogCommonLevel::Message:
			attri = BLUE_DEBUG_TEXT_COLOR;
			break;
		case LogCommonLevel::Warning:
			attri = YELLOW_DEBUG_TEXT_COLOR;
			break;
		case LogCommonLevel::Error:
			attri = RED_DEBUG_TEXT_COLOR;
			break;
		case LogCommonLevel::Fatal:
			attri = PURPLE_DEBUG_TEXT_COLOR;
			break;
		default:
			break;
		}

		log_output_util::OutputDebugTextWithColorAttribute(log, attri);
	}

	virtual void GetLogContent(std::string& content) override
	{
		content.append("\n================").append(" common log begin ").append("================\n");

		LogSingletonWithLevel::GetLogContent(content);

		content.append("\n================").append(" common log end ").append("==================\n");
	}
};

class LogCommonItem : public LogItemWithLevel< LogCommonItem, LogCommonLevel >
{
public:
	LogCommonItem(const char* file, long line) : LogItemWithLevel(file, line, LogCommonLevel::Info){}

	static std::wstring StringOfLevel(LogCommonLevel level)
	{
		switch (level)
		{
		case LogCommonLevel::Debug:
			return L"LV_DBG";

		case LogCommonLevel::Info:
			return L"LV_INF";

		case LogCommonLevel::Message:
			return L"LV_MSG";

		case LogCommonLevel::Warning:
			return L"LV_WAR";

		case LogCommonLevel::Error:
			return L"LV_ERR";

		case LogCommonLevel::Fatal:
			return L"LV_FTL";

		default:
			break;
		}

		return L"LV_NONE";
	}

	static void WriteLog(LogCommonLevel level, const std::wstring& log)
	{
		LogCommon::GetInstance()->WriteLog(level, log);
	}
};

#define LOG_DBG(fmt)	LogCommonItem(__FILE__, __LINE__).VLog(LogCommonLevel::Debug,	fmt)
#define LOG_INFO(fmt)	LogCommonItem(__FILE__, __LINE__).VLog(LogCommonLevel::Info,	fmt)
#define LOG_MSG(fmt)	LogCommonItem(__FILE__, __LINE__).VLog(LogCommonLevel::Message,	fmt)
#define LOG_WAR(fmt)	LogCommonItem(__FILE__, __LINE__).VLog(LogCommonLevel::Warning,	fmt)
#define LOG_ERR(fmt)	LogCommonItem(__FILE__, __LINE__).VLog(LogCommonLevel::Error,	fmt)
#define LOG_FLT(fmt)	LogCommonItem(__FILE__, __LINE__).VLog(LogCommonLevel::Fatal,	fmt)

// wrapper: do nothing in non-debug mode
#ifdef _DEBUG
#define DBG_WRAPPER(statement)	statement
#else
#define DBG_WRAPPER(...)
#endif

#define LOG_DBG_STRING_WRAPPER(str)   DBG_WRAPPER(LOG_DBG(str))
#define LOG_INFO_STRING_WRAPPER(str)  DBG_WRAPPER(LOG_INFO(str))
#define LOG_MSG_STRING_WRAPPER(str)   DBG_WRAPPER(LOG_MSG(str))
#define LOG_WAR_STRING_WRAPPER(str)   DBG_WRAPPER(LOG_WAR(str))
#define LOG_ERR_STRING_WRAPPER(str)   DBG_WRAPPER(LOG_ERR(str))
#define LOG_FLT_STRING_WRAPPER(str)   DBG_WRAPPER(LOG_FLT(str))

#endif