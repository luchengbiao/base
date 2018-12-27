#include "combo_box.h"
#include <QLabel>
#include <QListWidget>
#include <QStringList>

GAME_NAMESPACE_BEGIN

UIComboBox::UIComboBox(QWidget* parent)
: QComboBox(parent)
{}

void UIComboBox::SetStringList(const QStringList& str_list)
{
	if (!list_widget_)
	{
		list_widget_ = new QListWidget(this);
		list_widget_->setObjectName("game_lv_common");
		this->setModel(list_widget_->model());
		this->setView(list_widget_);
	}

	list_widget_->clear();

	int index = -1;
	for (const auto& str : str_list)
	{
		index += 1;

		QLabel* lb_item = new QLabel(list_widget_);
		lb_item->setContentsMargins(4, 0, 0, 0);
		lb_item->setText(str);

		QListWidgetItem *list_item = new QListWidgetItem(list_widget_);
		list_item->setSizeHint(QSize(0, 50));
		list_widget_->setItemWidget(list_item, lb_item);

		this->setItemText(index, str);
	}
}

GAME_NAMESPACE_END