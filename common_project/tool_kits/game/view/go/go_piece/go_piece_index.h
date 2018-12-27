#ifndef __GAME_VIEW_GO_GO_PIECE_GO_PIECE_INDEX_H__
#define __GAME_VIEW_GO_GO_PIECE_GO_PIECE_INDEX_H__
#include <type_traits>
#include <QLabel>
#include <QPointer>
#include <QWidget>
#include "game/macros.h"

GAME_NAMESPACE_BEGIN

template<typename WidgetT>
class GoPieceIndex
{
	typedef QPointer<QLabel> QLabelPtr;

public:
	GoPieceIndex();

	// to enable dynamic_cast<WidgetT*>(this): 
	// base-derived dynamic_cast needs base class has virtual function (at least one virtual function in base class)
	virtual ~GoPieceIndex() = default;
	
	void ShowIndex(int index, const QString& qss_object_name);

	void HideIndex();

protected:
	void LazyCreateIndexLabel();

protected:
	int			index_{ -1 };
	QLabelPtr	label_;
	QString		qss_object_name_;
};

template<typename WidgetT>
GoPieceIndex<WidgetT>::GoPieceIndex()
{
	// ensure that QWidget is base of WidgetT
	static_assert(std::is_base_of<QWidget, WidgetT>::value, "QWidget should be base of WidgetT");

	// ensure the safety of the base-derived conversion: static_cast<WidgetT*>(this)
	static_assert(std::is_base_of<GoPieceIndex<WidgetT>, WidgetT>::value, "GoPieceIndex<WidgetT> should also be base of WidgetT");
}

template<typename WidgetT>
void GoPieceIndex<WidgetT>::ShowIndex(int index, const QString& qss_object_name)
{
	if (!label_.isNull() && index_ == index && qss_object_name_ == qss_object_name)
	{
		label_->show();
		label_->raise();

		return;
	}

	this->LazyCreateIndexLabel();

	if (!label_.isNull())
	{
		label_->show();
		label_->raise();

		if (index_ != index)
		{
			index_ = index;
			label_->setText(QString::number(index_));
		}
		
		if (qss_object_name_ != qss_object_name)
		{
			qss_object_name_ = qss_object_name;
			label_->setObjectName(qss_object_name_);
			label_->setStyle(label_->style());
		}

		label_->adjustSize();
		
		auto parent = label_->parentWidget();
		label_->move(QPoint((parent->width() - label_->width()) / 2, (parent->height() - label_->height()) / 2) + static_cast<WidgetT*>(this)->OffsetFromParentCenterOfIndexLabel());
	}
}

template<typename WidgetT>
void GoPieceIndex<WidgetT>::HideIndex()
{
	if (!label_.isNull())
	{
		label_->hide();
	}
}

template<typename WidgetT>
void GoPieceIndex<WidgetT>::LazyCreateIndexLabel()
{
	if (label_.isNull())
	{
		auto parent_widget = static_cast<WidgetT*>(this)->ParentWidgetOfIndexLabel();
		label_ = new QLabel(parent_widget);

		label_->setAlignment(Qt::AlignCenter);
		label_->hide();
	}
}

GAME_NAMESPACE_END

#endif