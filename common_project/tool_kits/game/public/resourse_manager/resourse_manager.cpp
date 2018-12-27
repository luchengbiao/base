#include "resourse_manager.h"
#include <QApplication>
#include <QFile>
#include <QtGlobal>

GAME_NAMESPACE_BEGIN

void ResourceManager::LoadResource()
{
	static bool res_loaded = false;
	if (!res_loaded)
	{
		res_loaded = true;

		Q_INIT_RESOURCE(game);

		QFile file(R"(:/qss/res/qss/game.qss)");
		if (file.open(QFile::ReadOnly))
		{
			QApplication* q_app = static_cast<QApplication*>(QCoreApplication::instance());
			q_app->setStyleSheet(q_app->styleSheet() + file.readAll());
			file.close();
		}
	}
}

GAME_NAMESPACE_END