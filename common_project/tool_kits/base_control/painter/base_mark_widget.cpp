#include "base_mark_widget.h"

BaseMarkForm::BaseMarkForm(QSize size, QWidget *parent /*= 0*/) :QWidget(parent)
{
	b_accept_event_ = false;
	origin_pt_ = QPoint(-1, -1);
	width_ = 0;
	grid_count_ = 0;
	inside_ = 0;
	this->setFixedSize(size);
	//this->installEventFilter(this);
}

BaseMarkForm::~BaseMarkForm()
{

}

void BaseMarkForm::OnSetSign(MarkIcon icon)
{
	mark_icon_ = icon;
}

void BaseMarkForm::OnSendCommandCallBack(MarkOpCallback cb)
{
	mark_send_cb_ = cb;
}

void BaseMarkForm::OnInitValidPoint()
{

}

void BaseMarkForm::OnMarkSign(QPoint pt)
{
	QPoint index = QPoint(-1, -1);
	int ret = OnGetVirtualPoint(pt, index);

	if (!ret)
	{
		return;
	}
	QPoint des_pt = QPoint(-1, -1);
	OnGetRealPoint(index, des_pt);
	if (des_pt.x() <0 && des_pt.y()<0)
	{
		return;
	}

	MarkOpInfo mark_info;
	mark_info.draw_op_type_ = mark_icon_;
	mark_info.x_ = index.x();
	mark_info.y_ = index.y();
	MarkLabel* mark_icon = OnFindMarkIconByPos(index);
	if (mark_icon)
	{
		if (mark_icon->OnGetMarkIcon() == mark_icon_)
		{
			//如果是相同 则隐藏
			if (mark_icon->isVisible())
			{
				mark_icon->hide();
				mark_info.draw_op_type_ = ICON_DELETE;
			}
			else
			{
				OnUpdateIconLabelPos(mark_icon, des_pt);
				mark_icon->show();
			}
		}
		else
		{
			//更新
			std::string qss = OnGetIconQss((MarkIcon)mark_icon_);
			mark_icon->OnSetMarkIcon(mark_icon_);
			mark_icon->setObjectName(QString::fromStdString(qss));
			mark_icon->setStyle(mark_icon->style());
			OnUpdateIconLabelPos(mark_icon, des_pt);
			mark_icon->show();
		}
	}
	else
	{
		//画图
		MarkLabel* mark_icon = OnCreateIconLabel(mark_icon_,des_pt);
		if (mark_icon)
		{
			int idx = index.x() * 100 + index.y();
			mark_icon_map_.insert(std::map<int, MarkLabel*>::value_type(idx, mark_icon));
		}
	}

	if (mark_send_cb_)
	{
		mark_send_cb_(mark_info);
	}
}

void BaseMarkForm::OnGetRealPoint(QPoint pt, QPoint& des_pt)
{

}

bool BaseMarkForm::OnGetVirtualPoint(QPoint pt, QPoint& des_pt)
{
	return true;
}

bool BaseMarkForm::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == this)
	{
		if (!b_accept_event_)
		{
			return false;
		}
		if (event->type() == QEvent::MouseButtonRelease)
		{
			QPoint end_p;
			end_p =((QMouseEvent*)event)->pos();
			OnMarkSign(end_p);
		}

		return true;
	}

	return QWidget::eventFilter(watched, event);
}

void BaseMarkForm::OnSetParamCalValidPoint(QPoint origin_pt, int width, int gridCount/*=0*/, int inside /*= 0*/)
{
	origin_pt_ = origin_pt;
	width_ = width;
	grid_count_ = gridCount;
	inside_ = inside;

	OnInitValidPoint();
}

MarkLabel* BaseMarkForm::OnCreateIconLabel(MarkIcon icon, QPoint des_pt)
{
	return nullptr;
}

void BaseMarkForm::SetAcceptEvent(bool bAccept)
{
	b_accept_event_ = bAccept;
}

void BaseMarkForm::OnUpdateMarkIcon(int mark_icon, QPoint pt)
{
	if (mark_icon == -1)
	{
		if (pt.x() == -1 && pt.y() == -1)
		{
			//清空所有标记
			OnHideAllMarkIcons();
		}
		else
		{
			MarkLabel* mark_label = OnFindMarkIconByPos(pt);
			if (mark_label)
			{
				mark_label->hide();
			}
		}
	}
	else
	{
		MarkLabel* mark_label = OnFindMarkIconByPos(pt);

		QPoint des_pt = QPoint(-1, -1);
		if (grid_global_position_delegate_)
		{
			des_pt = this->mapFromGlobal(grid_global_position_delegate_(pt.x(), pt.y()));
		}
		else
		{
			OnGetRealPoint(pt, des_pt);
		}

		if (mark_label)
		{
			if (des_pt.x() < 0 && des_pt.y() < 0)
			{
				return;
			}

			//更新
			std::string qss = OnGetIconQss((MarkIcon)mark_icon);
			mark_label->OnSetMarkIcon((MarkIcon)mark_icon);
			mark_label->setObjectName(QString::fromStdString(qss));
			mark_label->setStyle(mark_label->style());
			OnUpdateIconLabelPos(mark_label, des_pt);
			mark_label->show();
		}
		else
		{
			//画图
			mark_label = OnCreateIconLabel((MarkIcon)mark_icon, des_pt);
			if (mark_label)
			{
				int idx = pt.x() * 100 + pt.y();
				mark_icon_map_.insert(std::map<int, MarkLabel*>::value_type(idx, mark_label));
			}
		}
	}
}

MarkLabel* BaseMarkForm::OnFindMarkIconByPos(QPoint pt)
{
	int index = pt.x() * 100 + pt.y();
	auto c_find = mark_icon_map_.find(index);
	if (c_find!=mark_icon_map_.end())
	{
		return c_find->second;
	}

	return nullptr;
}

void BaseMarkForm::OnHideAllMarkIcons()
{
	for (auto iter = mark_icon_map_.begin(); iter != mark_icon_map_.end();iter++)
	{
		if (iter->second)
		{
			iter->second->hide();
		}
	}
}

std::string BaseMarkForm::OnGetIconQss(MarkIcon icon)
{
	return "";
}

void BaseMarkForm::OnMouseReleaseFromParent(QPoint pt)
{
	OnMarkSign(pt);
}

void BaseMarkForm::OnUpdateIconLabelPos(MarkLabel* mark_icon,QPoint des_pt)
{
	
}
