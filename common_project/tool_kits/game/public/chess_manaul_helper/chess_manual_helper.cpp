#include "chess_manual_helper.h"
#include <QThread>
#include "base/file/file_util.h"
#include "base/util/string_util.h"
#include "http_request/file_fetch.h"
#include "game/public/downloaded_files_recorder/downloaded_files_recorder.h"
#include "game/public/public_setting/public_setting.h"

GAME_NAMESPACE_BEGIN

ChessManualHelper::ChessManualHelper()
{
	qRegisterMetaType<StdClosure>("StdClosure");
	connect(this, SIGNAL(SignalClosure(StdClosure)), this, SLOT(SlotClosure(StdClosure)));
}

void ChessManualHelper::Download(const std::string& file_url, const FileDownloadedCallback& callback)
{
	std::wstring file_path;
	if (ExistingFileOfUrl(file_url, &file_path))
	{
		if (callback) { callback(file_url, file_path); }

		return;
	}

	auto cbCompleted = ToWeakCallback([this, callback, file_url](const std::string& file_path){
		if (callback)
		{
			auto file_path_w = nbase::UTF8ToUTF16(file_path);
			if (this->thread() == QThread::currentThread())
			{
				callback(file_url, file_path_w);
			}
			else
			{
				emit SignalClosure([=]{ 
					DOWNLOADED_FILES_RECORDER->Record(file_url, file_path_w);
					callback(file_url, file_path_w);
				});
			}
		}
	});

	auto cbProgress = ToWeakCallback([=](double d1, double d2, double d3, double d4)
	{
		double progress = (d4 * 100 / d3);
	});

	FileFetcher ffetcher;
	ffetcher.AsyncFetchFile(std::string(file_url), nbase::UTF16ToUTF8(DirctoryToStoreChessManual()), cbCompleted, true, cbProgress, "", "");
}

GameRecordPtr ChessManualHelper::GameRecordByUrl(const std::string& url) const
{
	const auto it = url_2_game_record_.find(url);

	return it != url_2_game_record_.cend() ? it->second : GameRecordPtr();
}

void ChessManualHelper::SaveGameRecord(const std::string& url, const GameRecordPtr& game_record)
{
	if (game_record && url_2_game_record_.find(url) == url_2_game_record_.cend())
	{
		url_2_game_record_.emplace(url, game_record->Clone());
	}
}

void ChessManualHelper::ClearGameRecords()
{
	url_2_game_record_.swap(Url2GameRecordMap());
}

void ChessManualHelper::SlotClosure(StdClosure closure)
{
	if (closure) { closure(); }
}

GAME_NAMESPACE_END