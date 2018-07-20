#ifndef __LOG_CLASS_ONLINE_H__
#define __LOG_CLASS_ONLINE_H__
#include "../base/log_singleton_with_level.h"
#include "../util/log_output_util.h"

enum class ClassOnlineLogType
{
	None = 0,
	BanKe,
	O2O,
	Call,
};

class LogClassOnline : public LogSingletonWithLevel< LogClassOnline, ClassOnlineLogType >
{
	virtual bool CanLevelPass(ClassOnlineLogType) override { return true; }

	virtual void OutputOnDebugWindow(ClassOnlineLogType lv, const std::wstring& log) override
	{
		WORD attri = DEFAULT_DEBUG_TEXT_COLOR;
		switch (lv)
		{
		case ClassOnlineLogType::Call:
			attri = GREEN_DEBUG_TEXT_COLOR;
			break;
		case ClassOnlineLogType::O2O:
			attri = LIGHT_BLUE_DEBUG_TEXT_COLOR;
			break;
		case ClassOnlineLogType::BanKe:
			attri = BLUE_DEBUG_TEXT_COLOR;
			break;
		default:
			break;
		}

		log_output_util::OutputDebugTextWithColorAttribute(log, attri);
	}

	virtual void GetLogContent(std::string& content) override
	{
		content.append("\n================").append(" class-online log begin ").append("================\n");

		LogSingletonWithLevel::GetLogContent(content);

		content.append("\n================").append(" class-online log end ").append("==================\n");
	}
};

class LogClassHelper : public LogItemWithLevel< LogClassHelper, ClassOnlineLogType >
{
public:
	LogClassHelper(const char* file, long line) : LogItemWithLevel(file, line, ClassOnlineLogType::None){}

	static std::wstring StringOfLevel(ClassOnlineLogType level)
	{
		switch (level)
		{
		case ClassOnlineLogType::BanKe:
			return L"CLT_BANKE";

		case ClassOnlineLogType::O2O:
			return L"CLT_O2O";

		case ClassOnlineLogType::Call:
			return L"CLT_CALL";

		default:
			break;
		}

		return L"CLT_NONE";
	}

	static void WriteLog(ClassOnlineLogType level, const std::wstring& log)
	{
		LogClassOnline::GetInstance()->WriteLog(level, log);
	}
};

#define LOG_BANKE(fmt)		LogClassHelper(__FILE__, __LINE__).VLog(ClassOnlineLogType::BanKe, fmt)
#define LOG_O2O(fmt)		LogClassHelper(__FILE__, __LINE__).VLog(ClassOnlineLogType::O2O, fmt)
#define LOG_CALL(fmt)		LogClassHelper(__FILE__, __LINE__).VLog(ClassOnlineLogType::Call, fmt)

#endif