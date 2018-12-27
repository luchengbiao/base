#ifndef __GAME_MODEL_COMMAND_HANDLER_COURSE_FILE_WRITER_H__
#define __GAME_MODEL_COMMAND_HANDLER_COURSE_FILE_WRITER_H__
#include <string>
#include <QObject>
#include "base/callback/callback.h" // for StdClosure
#include "common/weak_callback/weak_callback.h"
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

class CourseFileWriter : public QObject, public wcb::SupportWeakCallback<CourseFileWriter>
{
	Q_OBJECT

	typedef wcb::WeakCallbackFlagCancelable<CourseFileWriter> MyWeakCallbackFlag;

	friend class CommandHandler;

public:
	static std::shared_ptr<CourseFileWriter> Create();

signals:
	void					SignalClosure(StdClosure);

private slots:
	void					SlotClosure(StdClosure);

private:
	CourseFileWriter();

private:
	MyWeakCallbackFlag		callback_flag_;
};


GAME_NAMESPACE_END

#endif