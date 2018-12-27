#include "draw_board.h"
#include "paint_image.h"
#include "paint_widget.h"
#include "QBoxLayout"
#include "QPainter"
#include "base\file\file_util.h"
#define ERROR_RANGE 20

DrawBoard::DrawBoard(PaintBaseInfo info, QWidget *parent /*= 0*/) : BaseBoard(info, parent)
{
	PaintBaseInfo paint_info = info;
	paint_info.width -= paint_info.offset_x_ * 2;
	paint_info.height -= paint_info.offset_y_ * 2;
	draw_board_ = new PaintWidget(paint_info, this);
	draw_board_->setFixedSize(paint_info.width, paint_info.height);
	QHBoxLayout* layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setContentsMargins(info.offset_x_, info.offset_y_, info.offset_x_, info.offset_y_);
	layout->addWidget(draw_board_);
	this->setLayout(layout);
	this->setFixedSize(info.width, info.height);
	b_visible_ = false;
}

DrawBoard::~DrawBoard()
{

}

QImage DrawBoard::GetImage()
{
	QImage image = GetBackgroundImage();
	QImage draw_image;
	if (draw_board_)
	{
		draw_image = draw_board_->GetImage();
	}
	if (!draw_image.isNull() && !image.isNull())
	{
		QPainter painter(&image);
		painter.drawImage(0, 0, draw_image);
	}
	return image;
}

QImage DrawBoard::GetImage(bool withBackgroudImage)
{
	return withBackgroudImage ? GetImage() : draw_board_->GetImage();
}

QImage DrawBoard::GetImageByForce()
{
	QImage image = GetBackgroundImageByForce();
	QImage draw_image;
	if (draw_board_)
	{
		draw_image = draw_board_->GetImageByForce();
	}
	if (!draw_image.isNull() && !image.isNull())
	{
		QPainter painter(&image);
		painter.drawImage(0, 0, draw_image);
	}
	return image;
}

void DrawBoard::SetTeaPenColor(QColor color)
{
	if (draw_board_)
	{
		draw_board_->SetTeaPenColor(color);
	}
}

void DrawBoard::SetStuPenColor(QColor color)
{
	if (draw_board_)
	{
		draw_board_->SetStuPenColor(color);
	}
}

void DrawBoard::SetTeaShape(Shape shape)
{
	if (draw_board_)
	{
		draw_board_->SetTeaShape(shape);
	}
}

void DrawBoard::SetStuShape(Shape shape)
{
	if (draw_board_)
	{
		draw_board_->SetTeaShape(shape);
	}
}

void DrawBoard::SetPenIcon(std::string color)
{
	if (draw_board_)
	{
		draw_board_->SetPenIcon(color);
	}
}

void DrawBoard::ResizeBoard(int width, int height)
{
	QSize board_size = this->GetImageSize();
	if (draw_board_
		&& (abs(width - board_size.width()) >= ERROR_RANGE
		|| abs(height - board_size.height()) >= ERROR_RANGE))
	{
		this->setFixedSize(width, height);
		ResizeImage(width, height);
		draw_board_->ResizeDrawBoard(width, height);
	}
}

void DrawBoard::OnStuDrawInfos(const std::list<DrawOpInfo>& info_list, bool b_paint)
{
	if (draw_board_)
	{
		draw_board_->OnStuDrawInfos(info_list, b_paint);
	}
}

void DrawBoard::OnTeaDrawInfos(const std::list<DrawOpInfo>& info_list, bool b_paint)
{
	if (draw_board_)
	{
		draw_board_->OnTeaDrawInfos(info_list, b_paint);
	}
}

void DrawBoard::SetDrawCb(DrawOpCallback cb)
{
	if (draw_board_)
	{
		draw_board_->SetDrawCb(cb);
	}
}

void DrawBoard::SetAcceptEvent(bool bAccept)
{
	if (draw_board_)
	{
		draw_board_->SetAcceptEvent(bAccept);
	}
}

bool  DrawBoard::IsEventAccepted() const
{
	return draw_board_ ? draw_board_->IsEventAccepted() : false;
}

bool DrawBoard::IsModified()
{
	bool draw_modify = false;
	if (draw_board_)
	{
		draw_modify = draw_board_->IsModified();
	}

	return (IsBackgroundModified() || draw_modify);
}

int DrawBoard::GetCoursewareType()
{
	if (draw_board_)
	{
		return draw_board_->GetCoursewareType();
	}
	return 0;
}

int DrawBoard::GetPage()
{
	if (draw_board_)
	{
		return draw_board_->GetPage();
	}
	return 0;
}

QImage DrawBoard::GetLocalDrawImage()
{
	std::wstring local_image = GetLocalImage();
	QImage image;
	if (nbase::FilePathIsExist(local_image, false)
		&& image.load(QString::fromStdWString(local_image)))
	{
		int angle = GetAngle();
		QMatrix matrix;
		matrix.rotate(angle);
		image = image.transformed(matrix);
		std::list<DrawRecord> list_draw_units;
		if (draw_board_) {
			draw_board_->GetDrawUnits(list_draw_units);
		}

		for (auto item : list_draw_units)
		{
			if (item.m_draw_unit_
				&& (item.m_sign_ & DrawRecordSign_StuClear) == 0
				&& (item.m_sign_ & DrawRecordSign_TeaClear) == 0)
			{
				item.m_draw_unit_->Render(image);
			}
		}
	}
	return image;
}

void DrawBoard::SetVisibleEx(bool bvisible)
{
	//if (b_visible_ != bvisible)
	{
		b_visible_ = bvisible;
		__super::SetVisibleEx(bvisible);
		if (draw_board_) {
			draw_board_->SetVisibleEx(bvisible);
		}
	}
}

void DrawBoard::OnLButtonDown(QPoint pt)
{
	if (draw_board_) {
		draw_board_->OnLButtonDown(pt);
	}
}

void DrawBoard::OnMouseMove(QPoint pt)
{
	if (draw_board_) {
		draw_board_->OnMouseMove(pt);
	}
}

void DrawBoard::OnLButtonUp(QPoint pt)
{
	if (draw_board_) {
		draw_board_->OnLButtonUp(pt);
	}
}

void DrawBoard::ClearDrawedUnits()
{
	if (draw_board_) {
		draw_board_->ClearDrawedUnits();
	}
}

