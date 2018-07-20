#include "log_file_util.h"
#include <mbstring.h>
#include <memory>
#include "base\file\file_util.h"
#include "base\memory\deleter.h"
#include "base\util\string_util.h"

namespace log_file_util
{
	using FileUniquePtr = std::unique_ptr<FILE, nbase::DeleterFileClose>;

	void WriteLogToFile(const std::wstring& file_path, const std::string& log)
	{
		//setlocale(LC_CTYPE, "");  // setlocale causes crash when called in multi-thread

		//bool exist = nbase::FilePathIsExist(file_path, false);

		FileUniquePtr fp;
		FILE* fp_file = nullptr;
		_wfopen_s(&fp_file, file_path.c_str(), L"a+");
		fp.reset(fp_file);
		if (!fp) { return; }

		//if (!exist)
		//{
		//	const char utf8[3] = { (char)0xEF, (char)0xBB, (char)0xBF };
		//	fwrite(utf8, sizeof(char), 3, fp.get());
		//}

		fwrite(log.c_str(), sizeof(char), log.size(), fp.get());

		fp.reset(nullptr);
	}

	void GetFileContent(const std::wstring& file_path, std::string& content)
	{
		if (nbase::FilePathIsExist(file_path, false))
		{
			std::wstring fn_w;
			nbase::FilePathApartFileName(file_path, fn_w);
			content.append("\n=======").append(nbase::UTF16ToUTF8(fn_w)).append("========\n");

			std::string logStr;
			nbase::ReadFileToString(file_path, logStr);
			content.append(logStr);
		}
	}

	void RemoveHalfIfLongerThan(const std::wstring& file_path, long max)
	{
		//打开文件
		FileUniquePtr fp;
		FILE* fp_file = nullptr;
		_wfopen_s(&fp_file, file_path.c_str(), L"r");
		fp.reset(fp_file);
		if (!fp)
		{
			fp.reset(nullptr);
			return;
		}

		//获取长度
		int ret = fseek(fp.get(), 0L, SEEK_END);
		if (ret != 0)
		{
			return;
		}

		//小于max则直接返回
		long len = ftell(fp.get());
		if (len <= max)
		{
			return;
		}

		//大于max，只留下最后to
		len = max * (-1);
		ret = fseek(fp.get(), len, SEEK_END);
		if (ret != 0)
		{
			return;
		}

		//创建新文件
		nbase::PathString new_file = file_path + L".old";
		FileUniquePtr fp2;
		FILE* fp_file2 = nullptr;
		_wfopen_s(&fp_file2, new_file.c_str(), L"w");
		fp2.reset(fp_file2);
		if (!fp2)
		{
			return;
		}

		//写入新文件
		char cbuf[12 * 1024] = { 0 };
		int cn = sizeof(cbuf), n = 0;
		while (!feof(fp.get()))
		{
			n = fread_s(cbuf, cn, sizeof(char), cn, fp.get());
			if (n > 0)
			{
				fwrite(cbuf, sizeof(char), n, fp2.get());
			}
			else
			{
				break;
			}
		}
		fp.reset(nullptr);
		fp2.reset(nullptr);

		//文件替换
		bool del = nbase::DeleteFileW(file_path);
		if (del)
		{
			::_wrename(new_file.c_str(), file_path.c_str());
		}
		else
		{
			nbase::DeleteFileW(new_file);
		}
	}
}