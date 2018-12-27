#ifndef __GAME_PUBLIC_CHESS_MANUAL_HELPER_CHESS_MANUAL_HELPER_H__
#define __GAME_PUBLIC_CHESS_MANUAL_HELPER_CHESS_MANUAL_HELPER_H__
#include <functional>
#include <unordered_map>
#include <string>
#include <QObject>
#include "base/callback/callback.h" // for StdClosure
#include "common/weak_callback/singleton_support_weak_callback.h"
#include "game/model/base/record/game_record.h"

GAME_NAMESPACE_BEGIN

class ChessManualHelper : public QObject, public wcb::SingletonSupportWeakCallback<ChessManualHelper>
{
	Q_OBJECT

	friend class wcb::SingletonSupportWeakCallback<ChessManualHelper>;

	typedef std::function<void(const std::string&, const std::wstring&)> FileDownloadedCallback;
	typedef std::unordered_map<std::string, GameRecordPtr> Url2GameRecordMap;

public:
	void				Download(const std::string& file_url, const FileDownloadedCallback&);

	GameRecordPtr		GameRecordByUrl(const std::string& url) const;

	void				SaveGameRecord(const std::string& url, const GameRecordPtr& game_record);

	void				ClearGameRecords();

signals:
	void				SignalClosure(StdClosure);

private slots:
	void				SlotClosure(StdClosure);

private:
	ChessManualHelper();

private:
	Url2GameRecordMap	url_2_game_record_;
};

#define CHESS_MANAUL_HELPER ChessManualHelper::GetInstance()

GAME_NAMESPACE_END

#endif