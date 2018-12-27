#ifndef __GAME_VIEW_GO_GO_PIECE_GO_PIECE_FLAG_H__
#define __GAME_VIEW_GO_GO_PIECE_GO_PIECE_FLAG_H__
#include <type_traits>
#include <QPointer>
#include <QWidget>
#include "game/macros.h"
#include "game/view/widget_with_pixmap/widget_with_pixmap.h"

GAME_NAMESPACE_BEGIN

template<typename WidgetT>
class GoPieceFlag
{
	typedef WidgetWithPixmap FlagWidget;
	typedef QPointer<FlagWidget> FlagWidgetPtr;

public:
	GoPieceFlag();

	// to enable dynamic_cast<WidgetT*>(this): 
	// base-derived dynamic_cast needs base class has virtual function (at least one virtual function in base class)
	virtual ~GoPieceFlag() = default;

	void			ShowFlag();

	void			HideFlag();

	void			SetFlagVisible(bool visible);

protected:
	void			LazyCreateFlag();

protected:
	FlagWidgetPtr	flag_;
};

template<typename WidgetT>
GoPieceFlag<WidgetT>::GoPieceFlag()
{
	// ensure that QWidget is base of WidgetT
	static_assert(std::is_base_of<QWidget, WidgetT>::value, "QWidget should be base of WidgetT");

	// ensure the safety of the base-derived conversion: static_cast<WidgetT*>(this)
	static_assert(std::is_base_of<GoPieceFlag<WidgetT>, WidgetT>::value, "GoPieceFlag<WidgetT> should also be base of WidgetT");
}

template<typename WidgetT>
void GoPieceFlag<WidgetT>::ShowFlag()
{
	this->LazyCreateFlag();

	if (flag_)
	{
		flag_->show();
		flag_->raise();
	}
}

template<typename WidgetT>
void GoPieceFlag<WidgetT>::HideFlag()
{
	if (!flag_.isNull())
	{
		flag_->hide();
	}
}

template<typename WidgetT>
void GoPieceFlag<WidgetT>::SetFlagVisible(bool visible)
{
	visible ? ShowFlag() : HideFlag();
}

template<typename WidgetT>
void GoPieceFlag<WidgetT>::LazyCreateFlag()
{
	if (flag_.isNull())
	{
		auto parent = static_cast<WidgetT*>(this)->ParentWidgetOfFlagWidget();

		flag_ = new FlagWidget(parent);

		QPixmap pixmap(R"(:go/res/images/go/last_flag.png)");
		if (!pixmap.isNull())
		{
			flag_->setFixedSize(pixmap.size());
			flag_->setPixmap(pixmap);
		}

		flag_->move(QPoint((parent->width() - flag_->width()) / 2, (parent->height() - flag_->height()) / 2) + static_cast<WidgetT*>(this)->OffsetFromParentCenterOfFlagWidget());

		flag_->hide();
	}
}

GAME_NAMESPACE_END

#endif