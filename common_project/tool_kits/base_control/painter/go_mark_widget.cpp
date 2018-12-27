#include "go_mark_widget.h"
#include "base/macros.h"

GoMarkForm::GoMarkForm(QSize size, QWidget *parent /*= 0*/) :BaseMarkForm(size, parent)
{
	InitPosButtons();
	item_distance_ = 0;
}

GoMarkForm::~GoMarkForm()
{
	ClearPosButtons();
}

void GoMarkForm::OnInitValidPoint()
{
	ClearPosButtons();
	int origin_x = origin_pt_.x() + width_ + 2;
	int origin_y = origin_pt_.y() + width_ + 2;
	if (grid_count_ == 19)
	{
		item_distance_ = (inside_ - grid_count_) / (grid_count_ - 1) + 1;
	}
	else
	{
		item_distance_ = (inside_ - grid_count_) / (grid_count_ - 1);	
	}

	for (int i = 0; i < grid_count_; i++)
	{
		for (int j = 0; j < grid_count_; j++)
		{
			QPoint* pt = new QPoint(-1, -1);
			pt->setX(origin_x);
			pt->setY(origin_y);
			go_coordinate[j][i] = pt;
			origin_x += (item_distance_ + 1);
		}
		origin_x = origin_pt_.x() + width_ + 2;
		origin_y += (item_distance_ + 1);
	}
}

void GoMarkForm::OnGetRealPoint(QPoint pt, QPoint& des_pt)
{
	if (pt.x() <grid_count_ && pt.y() <grid_count_)
	{
		QPoint* pos_button = go_coordinate[pt.x()][pt.y()];
		if (pos_button)
		{
			des_pt.setX(pos_button->x());
			des_pt.setY(pos_button->y());
		}
	}
}

bool GoMarkForm::OnGetVirtualPoint(QPoint pt, QPoint& des_pt)
{
	for (int i = 0; i < grid_count_; i++)
	{
		for (int j = 0; j < grid_count_; j++)
		{
			QPoint* pos_button = go_coordinate[i][j];

			int pos_x = pos_button->x();
			int pox_y = pos_button->y();
			QRect rect(pos_x - item_distance_ / 2, pox_y - item_distance_ / 2, item_distance_, item_distance_);
			if (rect.contains(pt.x(), pt.y()))
			{
				des_pt.setX(i);
				des_pt.setY(j);

				return true;
			}
		}
	}
	return false;
}

MarkLabel* GoMarkForm::OnCreateIconLabel(MarkIcon icon, QPoint des_pt)
{
	std::string qss = OnGetIconQss(icon);
	MarkLabel* mark_icon = new MarkLabel(this);
	mark_icon->OnSetMarkIcon(icon);
	mark_icon->setObjectName(QString::fromStdString(qss));
	mark_icon->setStyle(mark_icon->style());
	mark_icon->setFixedSize(15,15);
	mark_icon->move(des_pt.x()-7,des_pt.y()-7);
	mark_icon->show();
	return mark_icon;
}

void GoMarkForm::ClearPosButtons()
{
	for (int i = 0; i < grid_count_; i++)
	{
		for (int j = 0; j < grid_count_; j++)
		{
			QPoint* pos_button = go_coordinate[i][j];
			SAFE_DELETE(pos_button);
		}
	}
}

std::string GoMarkForm::OnGetIconQss(MarkIcon icon)
{
	std::string qss = "";
	if (icon == ICON_SOLID_CIRCLE)
	{
		qss = "lb_mark_solid_circle_small";
	}
	else if (icon == ICON_CROSS)
	{
		qss = "lb_mark_cross_small";
	}
	else if (icon == ICON_SOLID_RECTANGLE)
	{
		qss = "lb_mark_solid_rectangle_small";
	}
	else if (icon == ICON_SOLID_STAR)
	{
		qss = "lb_mark_solid_star_small";
	}
	else if (icon == ICON_SOLID_TRIANGLE)
	{
		qss = "lb_mark_solid_triangle_small";
	}
	else if (icon == ICON_CIRCLE)
	{
		qss = "lb_mark_circle_small";
	}
	return qss;
}

void GoMarkForm::InitPosButtons()
{
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			go_coordinate[i][j] = nullptr;
		}
	}
}

void GoMarkForm::OnUpdateIconLabelPos(MarkLabel* mark_icon, QPoint des_pt)
{
	if (mark_icon)
	{
		mark_icon->move(des_pt.x() - 7, des_pt.y() - 7);
	}
}
