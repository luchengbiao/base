#ifndef __GAME_PUBLIC_COURSE_DATA_HELPER_COURSE_DATA_HELPER_H__
#define __GAME_PUBLIC_COURSE_DATA_HELPER_COURSE_DATA_HELPER_H__
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <QObject>
#include "base/callback/callback.h" // for StdClosure
#include "common/weak_callback/singleton_support_weak_callback.h"
#include "game/public/files_fetcher/files_fetcher.h"
#include "game/public/structs/cmd_group_with_executor.h"

GAME_NAMESPACE_BEGIN

class FilesFetcher;

class CourseDataHelper : public QObject, public wcb::SingletonSupportWeakCallback<CourseDataHelper>
{
	Q_OBJECT

	friend class wcb::SingletonSupportWeakCallback<CourseDataHelper>;
	typedef std::vector<CommandGroupSharedPtr> CmdGroups;
	typedef std::unordered_set<FilesFetcherSharedPtr> FilesFetchers;

public:
	CmdExecutor						ExecutorOfFile(const std::wstring& file_path) const;

	CmdGroupWithExecutors			ParseFromFiles(const std::unordered_map<std::wstring, CmdExecutor>& mapFileToExecutor,
										   const proto::CommandFactory& cmd_factory,
										   bool need_to_sort = true) const;

	CmdGroupWithExecutors			ParseFromString(const std::string& str,
													const CmdExecutor executor,
													const proto::CommandFactory& cmd_factory,
													bool need_to_sort = true) const;

	void							SortCmdGroups(CmdGroupWithExecutors& cmd_groups) const;

	void							DownloadResourcesReferedByCmdGroups(const CmdGroups& cmd_groups, const StdClosure& callback);

signals:
	void							SignalClosure(StdClosure);

private slots:
	void							SlotClosure(StdClosure);

private:
	CourseDataHelper();

	FilesFetcherSharedPtr			CreateFetcher();
	void							DeleteFetcher(const FilesFetcherSharedPtr&);

private:
	mutable std::recursive_mutex	mutex_;
	FilesFetchers					files_fetchers_;
};

#define COURSE_DATD_HELPER	CourseDataHelper::GetInstance()

GAME_NAMESPACE_END

#endif