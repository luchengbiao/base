#ifndef __GAME_VIEW_COMBO_BOX_COMBO_BOX_H__
#define __GAME_VIEW_COMBO_BOX_COMBO_BOX_H__
#include <QComboBox>
#include "game/macros.h"

class QStringList;
class QListWidget;

GAME_NAMESPACE_BEGIN

class UIComboBox : public QComboBox
{
	Q_OBJECT

public:
	explicit UIComboBox(QWidget* parent = nullptr);

	void		SetStringList(const QStringList&);

private:
	QListWidget* list_widget_{ nullptr };
};

GAME_NAMESPACE_END

#endif