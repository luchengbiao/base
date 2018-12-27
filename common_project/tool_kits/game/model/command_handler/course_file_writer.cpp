#include "course_file_writer.h"
#include "qthread_manager/closure.h"
#include "command_handler.h"

using namespace proto;

GAME_NAMESPACE_BEGIN

std::shared_ptr<CourseFileWriter> CourseFileWriter::Create()
{
	std::shared_ptr<CourseFileWriter> ptr(new CourseFileWriter());

	if (ptr)
	{
		ptr->callback_flag_.SetWeakPtr(ptr->GetWeakPtr());
	}

	return (ptr);
}

CourseFileWriter::CourseFileWriter()
{
	qRegisterMetaType<StdClosure>("StdClosure");
	connect(this, SIGNAL(SignalClosure(StdClosure)), this, SLOT(SlotClosure(StdClosure)));
}

void CourseFileWriter::SlotClosure(StdClosure closure)
{
	if (closure) { closure(); }
}

GAME_NAMESPACE_END