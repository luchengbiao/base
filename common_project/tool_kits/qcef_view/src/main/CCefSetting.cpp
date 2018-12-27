#include <QCoreApplication>
#include <QDir>

#include "CCefSetting.h"

void CCefSetting::initializeInstance()
{
	static CCefSetting s_instance;
}

CefString CCefSetting::browser_sub_process_path;

CefString CCefSetting::resource_directory_path;

CefString CCefSetting::locales_directory_path;

CefString CCefSetting::user_agent;

CCefSetting::CCefSetting()
{
	qDebug() << "**************path""""""""""""""""";
	QDir ExeDir = QCoreApplication::applicationDirPath();
	ExeDir.setPath(ExeDir.canonicalPath());
	qDebug() << ExeDir.path();

	QString strExePath = ExeDir.filePath(RENDER_PROCESS_NAME);
	browser_sub_process_path.FromString(QDir::toNativeSeparators(strExePath).toUtf8().data());
	qDebug() << strExePath;

	QString strResPath = ExeDir.filePath(RESOURCE_DIRECTORY_NAME);
	resource_directory_path.FromString(QDir::toNativeSeparators(strResPath).toUtf8().data());
	qDebug() << strResPath;

	QDir ResPath(strResPath);
	locales_directory_path.FromString(QDir::toNativeSeparators(ResPath.filePath(LOCALES_DIRECTORY_NAME)).toUtf8().data());
	qDebug() << ResPath;

	user_agent.FromString(QCEF_USER_AGENT);
	qDebug() << QString::fromStdString(user_agent.ToString());
}

CCefSetting::~CCefSetting()
{

}
