#include "chess_mark_widget.h"
static const int NUMBER = 8;
ChessMarkForm::ChessMarkForm(QSize size, QWidget *parent /*= 0*/) :BaseMarkForm(size,parent)
{

}

ChessMarkForm::~ChessMarkForm()
{

}

void ChessMarkForm::OnInitValidPoint()
{

}

void ChessMarkForm::OnGetRealPoint(QPoint pt, QPoint& des_pt)
{
	int x = pt.x();
	int y = pt.y();
	des_pt.setX(x*width_ + origin_pt_.x());
	des_pt.setY(y*width_ + origin_pt_.y());
}

bool ChessMarkForm::OnGetVirtualPoint(QPoint pt, QPoint& des_pt)
{
	int x = pt.x();
	int y = pt.y();
	if (x < origin_pt_.x() || y < origin_pt_.y() || x > width_*NUMBER + origin_pt_.x() || y > width_*NUMBER + origin_pt_.x())
	{
		return false;
	}
	x = (x - origin_pt_.x()) < width_ ? 0 : x;
	y = (y - origin_pt_.y()) < width_ ? 0 : y;
	int hnumber = (x - origin_pt_.x()) / width_;
	int vnumber = (y - origin_pt_.y()) / width_;

	if (hnumber >=8 || vnumber >=8)
	{
		return false;
	}

	des_pt.setX(hnumber);
	des_pt.setY(vnumber);
	return true;
}

MarkLabel* ChessMarkForm::OnCreateIconLabel(MarkIcon icon, QPoint des_pt)
{
	std::string qss = OnGetIconQss(icon);
	MarkLabel* mark_icon = new MarkLabel(this);
	mark_icon->OnSetMarkIcon(icon);
	mark_icon->setObjectName(QString::fromStdString(qss));
	mark_icon->setFixedSize(22,22);
	mark_icon->setStyle(mark_icon->style());
	mark_icon->move(des_pt.x() + (width_ - 22) / 2, des_pt.y() + (width_ - 22) / 2);
	mark_icon->show();
	return mark_icon;
}

std::string ChessMarkForm::OnGetIconQss(MarkIcon icon)
{
	std::string qss = "";
	if (icon == ICON_SOLID_CIRCLE)
	{
		qss = "lb_mark_solid_circle_big";
	}
	else if (icon == ICON_CROSS)
	{
		qss = "lb_mark_cross_big";
	}
	else if (icon == ICON_SOLID_RECTANGLE)
	{
		qss = "lb_mark_solid_rectangle_big";
	}
	else if (icon == ICON_SOLID_STAR)
	{
		qss = "lb_mark_solid_star_big";
	}
	else if (icon == ICON_SOLID_TRIANGLE)
	{
		qss = "lb_mark_solid_triangle_big";
	}
	else if (icon == ICON_CIRCLE)
	{
		qss = "lb_mark_circle_big";
	}
	return qss;
}

void ChessMarkForm::OnUpdateIconLabelPos(MarkLabel* mark_icon, QPoint des_pt)
{
	if (mark_icon)
	{
		mark_icon->move(des_pt.x() + (width_ - 22) / 2, des_pt.y() + (width_ - 22) / 2);
	}
}
