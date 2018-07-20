#ifndef __LOG_TEMPLATE_H__
#define __LOG_TEMPLATE_H__
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <type_traits>
#include "base\file\file_util.h"
#include "base\time\time.h"
#include "base\util\string_number_conversions.h"
#include "base\util\string_util.h"
#include "log_singleton_interface.h"
#include "log_file_with_level.h"

template< typename DerivedT, typename LogLevelT >
class LogSingletonWithLevel : public LogSingletonInterface
{
public:
	static DerivedT* GetInstance()
	{
		static DerivedT instance;

		return &instance;
	}

	virtual ~LogSingletonWithLevel() = default;

	inline void			SetLogFilePath(const std::wstring& file_path) { log_file_impl_->SetLogFilePath(file_path); }
	inline std::wstring	GetLogFilePath() const { return log_file_impl_->GetLogFilePath(); }
	inline void			SetLogLevel(LogLevelT lv) { log_file_impl_->SetLogLevel(lv); }
	inline void			RemoveHalfIfLongerThan(long max) { log_file_impl_->RemoveHalfIfLongerThan(max); }

	virtual void WriteLog(LogLevelT lv, const std::wstring &log)
	{
#ifdef _DEBUG
		if (!log_debug_filter_ || log_debug_filter_(lv, log))
		{
			this->OutputOnDebugWindow(lv, log);
		}
#endif

		if (this->CanLevelPass(lv))
		{
			log_file_impl_->WriteLog(nbase::UTF16ToUTF8(log));
		}
	}

	virtual void GetLogContent(std::string& content) override { return log_file_impl_->GetLogContent(content); }

	template<typename FilterT> void SetLogDebugFilter(FilterT&& filter) { log_debug_filter_ = std::forward<FilterT>(filter); }

protected:
	LogSingletonWithLevel() : log_file_impl_(new LogFileWithLevel)
	{
		static_assert(std::is_integral<LogLevelT>::value || std::is_enum<LogLevelT>::value, "LogLevelT must be integral or enum");
	}

	LogSingletonWithLevel(const LogSingletonWithLevel&) = delete;
	LogSingletonWithLevel& operator=(const LogSingletonWithLevel&) = delete;

	virtual bool CanLevelPass(LogLevelT lv) { return (lv >= log_file_impl_->GetLogLevel<LogLevelT>()); }
	virtual void OutputOnDebugWindow(LogLevelT, const std::wstring& log)  { fwprintf_s(stdout, L"%s", log.c_str()); }

protected:
	std::unique_ptr<LogFileWithLevel>						log_file_impl_;

	std::function< bool(LogLevelT, const std::wstring&) >	log_debug_filter_;
};

/**
* HelperT must have 2 static functions:
* 1: std::wstring StringOfLevel(LogLevelT)
* 2��void WriteLog(LogLevelT, const std::wstring&)
*/
template< typename HelperT, typename LogLevelT >
class LogItemWithLevel
{
public:
	LogItemWithLevel(const char* file, long line, LogLevelT lv)
		: index_(0)
		, level_(lv)
	{
		static_assert(std::is_integral<LogLevelT>::value || std::is_enum<LogLevelT>::value, "LogLevelT must be integral or enum");

		nbase::Time::TimeStruct qt = nbase::Time::Now().ToTimeStruct(true);
		time_ = nbase::StringPrintf(L"[%04d-%02d-%02d %02d:%02d:%02d", qt.year_, qt.month_, qt.day_of_month_, qt.hour_, qt.minute_, qt.second_);

		auto str = nbase::UTF8ToUTF16(file);
		std::wstring file_name;
		nbase::FilePathApartFileName(str, file_name);

		file_line_ = nbase::StringPrintf(L" %s:%ld ", file_name.empty() ? str.c_str() : file_name.c_str(), line);
	}

	~LogItemWithLevel()
	{
		std::wstring lv = HelperT::StringOfLevel(level_);

		lv.append(L"] ");

		if (string_.empty())
		{
			string_ = fmt_;
		}
		else if (!fmt_.empty())
		{
			string_.append(fmt_);
		}

		std::wstring log = time_ + file_line_ + lv + string_ + L"\n";

		HelperT::WriteLog(level_, std::move(log));
	}

	LogItemWithLevel& VLog(LogLevelT lv, const std::wstring &fmt)
	{
		level_ = lv;
		fmt_ = fmt;

		return *this;
	}

	LogItemWithLevel& VLog(LogLevelT lv, const std::string &fmt)
	{
		return VLog(lv, nbase::UTF8ToUTF16(fmt));
	}

	LogItemWithLevel& operator<<(const std::wstring &str)
	{
		return this->append(str);
	}

	LogItemWithLevel& operator<<(const std::string &str)
	{
		return this->append(nbase::UTF8ToUTF16(str));
	}

	LogItemWithLevel& operator<<(long long lld)
	{
		return this->append(nbase::Int64ToString16(lld));
	}

	LogItemWithLevel& operator<<(void* ptr)
	{
		std::wstringstream wss;
		wss << ptr;

		return this->append(wss.str());
	}

protected:
	inline LogItemWithLevel& append(const std::wstring& str)
	{
		int len = 0;
		size_t pos = NextArg(len);
		if (pos == fmt_.npos)
		{
			fmt_.append(str);
		}
		else
		{
			string_.append(fmt_.substr(0, pos));
			string_.append(str);

			fmt_.erase(0, pos + len);

			index_ += 1;
		}

		return *this;
	}

	inline size_t NextArg(int &len) // ����"{x}"��������len����"{x}"�ĳ���
	{
		auto pos = fmt_.npos;

		if (! fmt_.empty())
		{
			auto str = nbase::StringPrintf(L"{%d}", index_);

			pos = fmt_.find(str);

			len = str.size();
		}

		return pos;
	}

protected:
	std::wstring	fmt_;
	std::wstring	string_;
	int				index_;

	std::wstring	time_;
	std::wstring	file_line_;
	LogLevelT		level_;
};

#endif