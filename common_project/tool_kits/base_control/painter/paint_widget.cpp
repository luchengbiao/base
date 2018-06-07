#include "paint_widget.h"
#include <QPainter>
#include <math.h>
#include "qlogging.h"
#include "QDebug"
#include <iostream>
#define BLACK_PEN_ICON "res\\images\\image_pen\\pen_black.png"
#define RED_PEN_ICON "res\\images\\image_pen\\pen_red.png"
#define BLUE_PEN_ICON "res\\images\\image_pen\\pen_blue.png"
#define GREEN_PEN_ICON "res\\images\\image_pen\\pen_black.png"
#define GREEN_ERASE_ICON "res\\images\\image_pen\\erase.png"

PaintWidget::PaintWidget(PaintBaseInfo info, QWidget *parent) :QWidget(parent)
{
	image_size_ = QSize(info.width, info.height);
	this->setFixedSize(image_size_);
	draw_rect_ = QRect(0, 0, info.width, info.height);
	b_modified_ = false;
	penStyle_ = Qt::SolidLine;
	tea_shape_ = PaintShape_None;
	stu_shape_ = PaintShape_None;
	m_type_ = info.m_type_;
	page_ = info.page_;
	b_stu_draw_ = info.b_stu_;
	b_accept_event_ = info.b_accept_event_;
	current_stu_draw_unit_ = NULL;
	current_tea_draw_unit_ = NULL;
	list_draw_units_.clear();
	vec_tea_waiting_draw_units_.clear();
	vec_stu_waiting_draw_units_.clear();
	this->installEventFilter(this);
	this->setStyleSheet("QWidget{margin:0px;padding-top:0px;padding-left:0px;}");
	this->setMouseTracking(true);
	//this->setCursor(Qt::BlankCursor);
	map_pen_icon_["black"] = BLACK_PEN_ICON;
	map_pen_icon_["red"] = RED_PEN_ICON;
	map_pen_icon_["blue"] = BLUE_PEN_ICON;
	map_pen_icon_["green"] = GREEN_PEN_ICON;
	map_pen_icon_["erase"] = GREEN_ERASE_ICON;
	teaPenColor_ = QColor(0, 0, 0);
	stuPenColor_ = QColor(0x06, 0xe6, 0x13); 
	pen_point_ = QPoint(-1, -1);
	pen_icon_.load(QString::fromStdString(map_pen_icon_["black"]));
}

PaintWidget::~PaintWidget()
{
	ReleaseAllDrawUnits();
}

bool PaintWidget::IsModified()
{
	return b_modified_;
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.scale(1, 1);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawImage(0, 0, image_);

	if (pen_point_.x() >= 0 || pen_point_.y() >= 0)
	{
		painter.drawImage(pen_point_.x(), pen_point_.y(), pen_icon_);
	}
}

void PaintWidget::MousePress(QMouseEvent *event)
{
	if (event && event->button() == Qt::LeftButton)
	{
		QPoint start;
		start = event->pos();
		OnLButtonDown(start);
	}
}

void PaintWidget::MovePenIcon(QPoint pt)
{
	QPoint move = pt;
	if (move != pen_point_)
	{
		QPoint tmp = pen_point_;
		pen_point_ = move;
		if (!b_stu_draw_ && tea_shape_ == PaintShape_Erase)
		{
			pen_point_.setX(pen_point_.x() - pen_icon_.width() / 2);
			pen_point_.setY(pen_point_.y() - pen_icon_.height() / 2);
		}
		else if (!b_stu_draw_ && tea_shape_ == PaintShape_None)
		{
			pen_point_.setY(pen_point_.y() - pen_icon_.height());
		}
		else if (b_stu_draw_ && stu_shape_ == PaintShape_Erase)
		{
			pen_point_.setX(pen_point_.x() - pen_icon_.width() / 2);
			pen_point_.setY(pen_point_.y() - pen_icon_.height() / 2);
		}
		else if (b_stu_draw_ && stu_shape_ == PaintShape_None)
		{
			pen_point_.setY(pen_point_.y() - pen_icon_.height());
		}
			
		QPoint topLeft = QPoint(min(tmp.x(), pen_point_.x()), min(tmp.y(), pen_point_.y()));
		QPoint bottomRight = QPoint(max(tmp.x(), pen_point_.x()), max(tmp.y(), pen_point_.y()));
		bottomRight.setX(bottomRight.x() + pen_icon_.width());
		bottomRight.setY(bottomRight.y() + pen_icon_.height());
		update(QRect(topLeft, bottomRight));
	}
}

void PaintWidget::MouseMove(QMouseEvent *event)
{
	if (event) {
		MovePenIcon(event->pos());
	}

	if (event && event->buttons() == Qt::LeftButton)
	{
		QPoint move;
		move = event->pos();
		if (tea_shape_ == PaintShape_None || tea_shape_ == PaintShape_Erase)
		{
			QRect rcItem(0, 0, image_size_.width(), image_size_.height());
			if (rcItem.contains(move))
			{
				OnDrawUnitProcess(move);
			}
			PaintContent(image_);
		}
	}
}

void PaintWidget::MouseRelease(QMouseEvent *event)
{
	if (event && event->button() == Qt::LeftButton)
	{
		QPoint end_p;
		end_p = event->pos();
		OnLButtonUp(end_p);
	}
}

void PaintWidget::MouseLeave(QMouseEvent *event)
{
	if (event && !b_stu_draw_ && current_tea_draw_unit_)
	{
		QPoint pt = current_tea_draw_unit_->GetEndPoint();
		OnLButtonUp(pt);
	}
	else if (event && b_stu_draw_ && current_stu_draw_unit_)
	{
		QPoint pt = current_stu_draw_unit_->GetEndPoint();
		OnLButtonUp(pt);
	}
}

bool PaintWidget::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == this)
	{
		if (!b_accept_event_)
		{
			return false;
		}

		if (event->type() == QEvent::MouseButtonPress)
		{
			MousePress((QMouseEvent*)event);
			return true;
		}
		else if (event->type() == QEvent::MouseMove)
		{
			MouseMove((QMouseEvent*)event);
			return true;
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			MouseRelease((QMouseEvent*)event);
			return true;
		}
		else if (event->type() == QEvent::Leave)
		{
			MouseLeave((QMouseEvent*)event);
			return true;
		}
	}

	return QWidget::eventFilter(watched, event);
}

void PaintWidget::OnDrawUnitStart(const QPoint &point)
{
	if (b_stu_draw_) {
		CreateStuDrawUnit(point);
	} else {
		CreateTeaDrawUnit(point);
	}

	if (draw_op_cb_)
	{
		DrawOpInfo info;
		info.draw_op_type_ = DrawOpStart;
		if (!b_stu_draw_ && tea_shape_ == PaintShape_Erase) {
			info.draw_op_type_ = EraserOpStart;
		}
		else if (b_stu_draw_ && stu_shape_ == PaintShape_Erase) {
			info.draw_op_type_ = EraserOpStart;
		}
		info.x_ = point.x() * 1.0 / image_size_.width();
		info.y_ = point.y() * 1.0 / image_size_.height();
		draw_op_cb_(info);
	}
}

void PaintWidget::CreateStuDrawUnit(const QPoint& point)
{
	if (current_stu_draw_unit_)
	{
		delete current_stu_draw_unit_;
		current_stu_draw_unit_ = NULL;
	}

	QRect rc(0, 0, image_size_.width(), image_size_.height());

	if (stu_shape_ == PaintShape_None)
	{
		current_stu_draw_unit_ = new DrawUnitPen(point, rc);
	}
	else if (stu_shape_ == PaintShape_Erase)
	{
		current_stu_draw_unit_ = new DrawUnitErase(point, rc);
	}
	current_stu_draw_unit_->SetPenColor(stuPenColor_);
}

void PaintWidget::CreateTeaDrawUnit(const QPoint& point)
{
	if (current_tea_draw_unit_)
	{
		delete current_tea_draw_unit_;
		current_tea_draw_unit_ = NULL;
	}
	QRect rc(0, 0, image_size_.width(), image_size_.height());

	if (tea_shape_ == PaintShape_None)
	{
		current_tea_draw_unit_ = new DrawUnitPen(point, rc);
	}
	else if (tea_shape_ == PaintShape_Erase)
	{
		current_tea_draw_unit_ = new DrawUnitErase(point, rc);
	}
	current_tea_draw_unit_->SetPenColor(teaPenColor_);
}

void PaintWidget::OnDrawUnitProcess(const QPoint &point)
{
	if (!b_stu_draw_ && current_tea_draw_unit_
		&& current_tea_draw_unit_->SetEndPoint(point))
	{
		DoDrawUnitProcess(point);
	}
	else if (b_stu_draw_ && current_stu_draw_unit_
		&& current_stu_draw_unit_->SetEndPoint(point))
	{
		DoDrawUnitProcess(point);
	}
}

void PaintWidget::DoDrawUnitProcess(const QPoint &point)
{
	// 鼠标移动过程中，需要立即重绘
	PaintContent(image_);
	if (draw_op_cb_)
	{
		DrawOpInfo info;
		info.draw_op_type_ = DrawOpMove;
		if (!b_stu_draw_ && tea_shape_ == PaintShape_Erase) {
			info.draw_op_type_ = EraserOpMove;
		}
		else if (b_stu_draw_ && stu_shape_ == PaintShape_Erase) {
			info.draw_op_type_ = EraserOpMove;
		}
		info.x_ = point.x() * 1.0 / image_size_.width();
		info.y_ = point.y() * 1.0 / image_size_.height();
		draw_op_cb_(info);
	}
}

void PaintWidget::OnDrawUnitEnd(const QPoint &point)
{
	if ((!b_stu_draw_ && !current_tea_draw_unit_)
		|| (b_stu_draw_ && !current_stu_draw_unit_))
	{
		PaintContent(image_);
		return;
	}

	if (!b_stu_draw_) {
		current_tea_draw_unit_->SetEndPoint(point);
	} else {
		current_stu_draw_unit_->SetEndPoint(point);
	}
	SubmitDrawUnit();
	if (draw_op_cb_)
	{
		DrawOpInfo info;
		info.draw_op_type_ = DrawOpEnd;
		if (!b_stu_draw_ && tea_shape_ == PaintShape_Erase) {
			info.draw_op_type_ = EraserOpEnd;
		}
		else if (b_stu_draw_ && stu_shape_ == PaintShape_Erase) {
			info.draw_op_type_ = EraserOpEnd;
		}
		info.x_ = point.x() * 1.0 / image_size_.width();
		info.y_ = point.y() * 1.0 / image_size_.height();
		draw_op_cb_(info);
	}
}

void PaintWidget::SubmitDrawUnit()
{
	if (!b_stu_draw_)
	{
		vec_tea_waiting_draw_units_.push_back(current_tea_draw_unit_);
		current_tea_draw_unit_ = NULL; //置空,vector自己管理内存
	}
	else if (b_stu_draw_)
	{
		vec_stu_waiting_draw_units_.push_back(current_stu_draw_unit_);
		current_stu_draw_unit_ = NULL; //置空,vector自己管理内存
	}
	PaintContent(image_);
}

void PaintWidget::PaintContent(QImage &Cimage, bool redraw)
{
	bool bfirst = true;
	bool bErase = false;

	if (redraw)
	{
		Cimage = QImage(image_size_, QImage::Format_ARGB32);
		Cimage.fill(qRgba(255, 255, 255, 0));
		for (auto item : list_draw_units_)
		{
			if (item.m_draw_unit_
				&& (item.m_sign_ & DrawRecordSign_StuClear) == 0
				&& (item.m_sign_ & DrawRecordSign_TeaClear) == 0)
			{
				item.m_draw_unit_->Render(Cimage);
				CalcDrawRect(item.m_draw_unit_->GetDrawRect(), bfirst);
				bErase = (item.m_draw_unit_->GetType() == DrawUnit_Erase);
			}
		}
	}

	for (auto it : vec_tea_waiting_draw_units_)
	{
		if (it)
		{
			it->Render(Cimage);
			CalcDrawRect(it->GetDrawRect(), bfirst);
			bErase = (it->GetType() == DrawUnit_Erase);
			DrawRecord item;
			item.m_draw_unit_ = it;
			item.m_sign_ = DrawRecordSign_TeaDraw;
			list_draw_units_.push_back(item);
		}
	}
	vec_tea_waiting_draw_units_.clear();

	if (current_tea_draw_unit_)
	{
		current_tea_draw_unit_->Render(Cimage, true);
		CalcDrawRect(current_tea_draw_unit_->GetDrawRect(), bfirst);
		bErase = (current_tea_draw_unit_->GetType() == DrawUnit_Erase);
	}

	for (auto it : vec_stu_waiting_draw_units_)
	{
		if (it)
		{
			it->Render(Cimage);
			CalcDrawRect(it->GetDrawRect(), bfirst);
			bErase = (it->GetType() == DrawUnit_Erase);
			DrawRecord item;
			item.m_draw_unit_ = it;
			item.m_sign_ = DrawRecordSign_StuDraw;
			list_draw_units_.push_back(item);
		}
	}
	vec_stu_waiting_draw_units_.clear();

	if (current_stu_draw_unit_)
	{
		current_stu_draw_unit_->Render(Cimage, true);
		CalcDrawRect(current_stu_draw_unit_->GetDrawRect(), bfirst);
		bErase = (current_stu_draw_unit_->GetType() == DrawUnit_Erase);
	}
	if (bErase || redraw)
	{
		ReplaceEraseColorPixel(redraw);
	}
	b_modified_ = true;

	if (redraw)
	{
		update();
	}
	else
	{
		update(draw_rect_);
	}
}

QImage PaintWidget::GetImage()
{
	b_modified_ = false;
	return image_;
}

void PaintWidget::SetPenStyle(Qt::PenStyle style)
{
	penStyle_ = style;
}

void PaintWidget::SetTeaShape(Shape shape)
{
	if (tea_shape_ != shape)
	{
		tea_shape_ = shape;

		pen_point_ = QPoint(-1, -1);
		update();
	}
}

void PaintWidget::SetStuShape(Shape shape)
{
	if (stu_shape_ != shape)
	{
		stu_shape_ = shape;

		pen_point_ = QPoint(-1, -1);
		update();
	}
}

void PaintWidget::SetPenIcon(std::string color)
{
	pen_icon_.load(QString::fromStdString(map_pen_icon_[color]));

	if (color == "erase")
	{
		int icon_size = image_size_.height() * 0.1;
		pen_icon_ = pen_icon_.scaled(icon_size, icon_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
}

void PaintWidget::ResizeDrawBoard(int width, int height)
{
	if (image_.isNull())
	{
		image_size_ = QSize(width, height);
	}
	else if (width != image_size_.width()
		|| height != image_size_.height())
	{
		image_size_ = QSize(width, height);
		image_ = QImage(image_size_, QImage::Format_ARGB32);
		image_.fill(qRgba(255, 255, 255, 0));
		this->setFixedSize(image_size_);

		PaintContent(image_, true);

		QImage erase_icon;
		if (((!b_stu_draw_ && tea_shape_ == PaintShape_Erase)
			|| (b_stu_draw_ && stu_shape_ == PaintShape_Erase))
			&& erase_icon.load(QString::fromStdString(map_pen_icon_["erase"])))
		{
			int icon_size = image_size_.height() * 0.1;
			pen_icon_ = erase_icon.scaled(icon_size, icon_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		}
	}
}

void PaintWidget::SetVisibleEx(bool bvisible)
{
	if (bvisible)
	{
		image_ = QImage(image_size_, QImage::Format_ARGB32);
		image_.fill(qRgba(255, 255, 255, 0));
		this->setFixedSize(image_size_);

		PaintContent(image_, true);
	}
	else if (!bvisible)
	{
		image_ = QImage();
	}
}

void PaintWidget::SetTeaPenColor(QColor color)
{
	teaPenColor_ = color;
}

void PaintWidget::SetStuPenColor(QColor color)
{
	stuPenColor_ = color;
}

void PaintWidget::ReleaseAllDrawUnits()
{
	for (int i = 0; i < vec_stu_waiting_draw_units_.size(); i++)
	{
		DrawUnit* item = vec_stu_waiting_draw_units_[i];
		if (item)
		{
			delete item;
			item = NULL;
		}
	}
	for (int i = 0; i < vec_tea_waiting_draw_units_.size(); i++)
	{
		DrawUnit* item = vec_tea_waiting_draw_units_[i];
		if (item)
		{
			delete item;
			item = NULL;
		}
	}
	vec_tea_waiting_draw_units_.clear();
	vec_stu_waiting_draw_units_.clear();

	for (auto item : list_draw_units_)
	{
		if (item.m_draw_unit_)
		{
			delete item.m_draw_unit_;
			item.m_draw_unit_ = NULL;
		}
	}
	list_draw_units_.clear();

	if (current_stu_draw_unit_)
	{
		delete current_stu_draw_unit_;
		current_stu_draw_unit_ = NULL;
	}

	if (current_tea_draw_unit_)
	{
		delete current_tea_draw_unit_;
		current_tea_draw_unit_ = NULL;
	}
}

bool PaintWidget::UndoStuDraw()
{
	bool redraw = false;
	UpdateDrawList();
	for (auto itor = list_draw_units_.rbegin(); itor != list_draw_units_.rend();)
	{
		if ((itor->m_sign_ & DrawRecordSign_StuDraw) != 0
			&& (itor->m_sign_ & DrawRecordSign_TeaClear) == 0
			&& (itor->m_sign_ & DrawRecordSign_StuClear) == 0)
		{
			itor = std::list<DrawRecord>::reverse_iterator(list_draw_units_.erase((++itor).base()));
			redraw = true;
			break;
		}
		else if ((itor->m_sign_ & DrawRecordSign_StuDraw) != 0
			&& (itor->m_sign_ & DrawRecordSign_StuClear) != 0)
		{
			itor->m_sign_ &= ~DrawRecordSign_StuClear;
			redraw = true;
			itor++;
		}
		else
		{
			itor++;
		}
	}
	return redraw;
}

bool PaintWidget::UndoTeaDraw()
{
	bool redraw = false;
	UpdateDrawList();
	for (auto itor = list_draw_units_.rbegin(); itor != list_draw_units_.rend();)
	{
		if ((itor->m_sign_ & DrawRecordSign_TeaDraw) != 0
			&& (itor->m_sign_ & DrawRecordSign_TeaClear) == 0)
		{
			itor = std::list<DrawRecord>::reverse_iterator(list_draw_units_.erase((++itor).base()));
			redraw = true;
			break;
		}
		else if ((itor->m_sign_ & DrawRecordSign_TeaClear) != 0)
		{
			itor->m_sign_ &= ~DrawRecordSign_TeaClear;
			redraw = true;
			itor++;
		}
		else
		{
			itor++;
		}
	}
	return redraw;
}

bool PaintWidget::ClearStuDraw()
{
	bool redraw = false;
	UpdateDrawList();
	for (auto itor = list_draw_units_.begin(); itor != list_draw_units_.end();)
	{
		if ((itor->m_sign_ & DrawRecordSign_StuDraw) != 0
			&& (itor->m_sign_ & DrawRecordSign_StuClear) == 0
			&& (itor->m_sign_ & DrawRecordSign_TeaClear) == 0)
		{
			itor->m_sign_ |= DrawRecordSign_StuClear;
			itor++;
			redraw = true;
		}
		else if ((itor->m_sign_ & DrawRecordSign_StuDraw) != 0
			&& (itor->m_sign_ & DrawRecordSign_StuClear) != 0)
		{
			itor = list_draw_units_.erase(itor);
		}
		else
		{
			itor++;
		}
	}
	return redraw;
}

bool PaintWidget::ClearTeaDraw()
{
	bool redraw = false;
	UpdateDrawList();
	for (auto itor = list_draw_units_.begin(); itor != list_draw_units_.end();)
	{
		if ((itor->m_sign_ & DrawRecordSign_TeaClear) == 0
			&& (itor->m_sign_ & DrawRecordSign_StuClear) == 0)
		{
			itor->m_sign_ |= DrawRecordSign_TeaClear;
			itor++;
			redraw = true;
		}
		else if ((itor->m_sign_ & DrawRecordSign_TeaClear) != 0)
		{
			itor = list_draw_units_.erase(itor);
		}
		else
		{
			itor++;
		}
	}
	return redraw;
}

void PaintWidget::OnStuDrawInfos(std::list<DrawOpInfo> info_list, bool b_paint)
{
	bool redraw = false;
	bool draw = false;
	for (auto& it : info_list)
	{
		QPoint pt = QPoint(it.x_ * image_size_.width(), it.y_ * image_size_.height());

		switch (it.draw_op_type_)
		{
		case DrawOpStart:
		{
			SetStuShape(PaintShape_None);
			if (current_stu_draw_unit_)
			{
				vec_stu_waiting_draw_units_.push_back(current_stu_draw_unit_);
				current_stu_draw_unit_ = NULL;
			}
			CreateStuDrawUnit(pt);
			draw = true;
			break;
		}
		case DrawOpMove:
		{
			if (!current_stu_draw_unit_)
			{
				CreateStuDrawUnit(pt);
				draw = true;
			}
			else if (current_stu_draw_unit_->SetEndPoint(pt))
			{
				draw = true;
			}
			break;
		}
		case DrawOpEnd:
		{
			if (current_stu_draw_unit_)
			{
				current_stu_draw_unit_->SetEndPoint(pt);
				vec_stu_waiting_draw_units_.push_back(current_stu_draw_unit_);
				current_stu_draw_unit_ = NULL;
				draw = true;
			}
			break;
		}
		case DrawOpUndoCb:
		{
			redraw = UndoStuDraw();
			draw = true;
			break;
		}
		case DrawOpClearCb:
		{
			redraw = ClearStuDraw();
			draw = true;
			break;
		}
		case DrawOpPenColor:
		{
			stuPenColor_ = it.pen_color_;
			break;
		}
		case EraserOpStart:
		{
			SetStuShape(PaintShape_Erase);
			if (current_stu_draw_unit_)
			{
				vec_stu_waiting_draw_units_.push_back(current_stu_draw_unit_);
				current_stu_draw_unit_ = NULL;
			}
			CreateStuDrawUnit(pt);
			draw = true;
			break;
		}
		case EraserOpMove:
		{
			if (!current_stu_draw_unit_)
			{
				CreateStuDrawUnit(pt);
				draw = true;
			}
			else if (current_stu_draw_unit_->SetEndPoint(pt))
			{
				draw = true;
			}
			break;
		}
		case EraserOpEnd:
		{
			if (current_stu_draw_unit_)
			{
				current_stu_draw_unit_->SetEndPoint(pt);
				vec_stu_waiting_draw_units_.push_back(current_stu_draw_unit_);
				current_stu_draw_unit_ = NULL;
				draw = true;
			}
			break;
		}
		default:
			break;
		}
	}

	if (draw && b_paint)
	{
		PaintContent(image_, redraw);
	}
}

void PaintWidget::OnTeaDrawInfos(std::list<DrawOpInfo> info_list, bool b_paint)
{
	bool redraw = false;
	bool draw = false;
	for (auto& it : info_list)
	{
		QPoint pt = QPoint(it.x_ *  image_size_.width(), it.y_ * image_size_.height());

		switch (it.draw_op_type_)
		{
		case DrawOpStart:
		{
			SetTeaShape(PaintShape_None);
			if (current_tea_draw_unit_)
			{
				vec_tea_waiting_draw_units_.push_back(current_tea_draw_unit_);

				current_tea_draw_unit_ = NULL;
			}
			CreateTeaDrawUnit(pt);
			draw = true;
			break;
		}
		case DrawOpMove:
		{
			if (!current_tea_draw_unit_)
			{
				CreateTeaDrawUnit(pt);
				draw = true;
			}
			else if (current_tea_draw_unit_->SetEndPoint(pt))
			{
				draw = true;
			}
			break;
		}
		case DrawOpEnd:
		{
			if (current_tea_draw_unit_)
			{
				current_tea_draw_unit_->SetEndPoint(pt);
				vec_tea_waiting_draw_units_.push_back(current_tea_draw_unit_);

				current_tea_draw_unit_ = NULL;
				draw = true;
			}
			break;
		}
		case DrawOpUndoCb:
		{
			redraw = UndoTeaDraw();
			draw = true;
			break;
		}
		case DrawOpClearCb:
		{
			redraw = ClearTeaDraw();
			draw = true;
			break;
		}
		case DrawOpPenColor:
		{
			teaPenColor_ = it.pen_color_;
			break;
		}
		case EraserOpStart:
		{
			SetTeaShape(PaintShape_Erase);
			if (current_tea_draw_unit_)
			{
				vec_tea_waiting_draw_units_.push_back(current_tea_draw_unit_);

				current_tea_draw_unit_ = NULL;
			}
			CreateTeaDrawUnit(pt);
			draw = true;
			break;
		}
		case EraserOpMove:
		{
			if (!current_tea_draw_unit_)
			{
				CreateTeaDrawUnit(pt);
				draw = true;
			}
			else if (current_tea_draw_unit_->SetEndPoint(pt))
			{
				draw = true;
			}
			break;
		}
		case EraserOpEnd:
		{
			if (current_tea_draw_unit_)
			{
				current_tea_draw_unit_->SetEndPoint(pt);
				vec_tea_waiting_draw_units_.push_back(current_tea_draw_unit_);

				current_tea_draw_unit_ = NULL;
				draw = true;
			}
			break;
		}
		default:
			break;
		}
	}

	if (draw && b_paint)
	{
		PaintContent(image_, redraw);
	}
}

void PaintWidget::SetDrawCb(DrawOpCallback cb)
{
	draw_op_cb_ = cb;
}

void PaintWidget::SetAcceptEvent(bool bAccept)
{
	b_accept_event_ = bAccept;
	pen_point_ = QPoint(-1, -1);
	update();
}


QRect PaintWidget::GetDrawRect()
{
	return draw_rect_;
}

int PaintWidget::GetCoursewareType()
{
	return m_type_;
}

int PaintWidget::GetPage()
{
	return page_;
}

bool PaintWidget::GetDrawUnits(std::list<DrawRecord>& list_draw_units)
{
	list_draw_units = list_draw_units_;
	return true;
}

void PaintWidget::ReplaceEraseColorPixel(bool redraw)
{
	
	
}

void PaintWidget::CalcDrawRect(QRect rect, bool& bfirst)
{
	if (bfirst)
	{
		draw_rect_ = rect;
	}
	else
	{
		draw_rect_.setTop(min(rect.top(), draw_rect_.top()));
		draw_rect_.setBottom(max(rect.bottom(), draw_rect_.bottom()));
		draw_rect_.setLeft(min(rect.left(), draw_rect_.left()));
		draw_rect_.setRight(max(rect.right(), draw_rect_.right()));
	}
	bfirst = true;
}

void PaintWidget::UpdateDrawList()
{
	for (auto it : vec_tea_waiting_draw_units_)
	{
		if (it)
		{
			DrawRecord item;
			item.m_draw_unit_ = it;
			item.m_sign_ = DrawRecordSign_TeaDraw;
			list_draw_units_.push_back(item);
		}
	}
	vec_tea_waiting_draw_units_.clear();

	for (auto it : vec_stu_waiting_draw_units_)
	{
		if (it)
		{
			DrawRecord item;
			item.m_draw_unit_ = it;
			item.m_sign_ = DrawRecordSign_StuDraw;
			list_draw_units_.push_back(item);
		}
	}
	vec_stu_waiting_draw_units_.clear();
}

void PaintWidget::OnLButtonDown(QPoint pt)
{
	MovePenIcon(pt);
	QRect rcItem(0, 0, image_size_.width(), image_size_.height());
	/*std::cout << "OnLButtonDown, x: " << rcItem.x() << " y: " << rcItem.y() << " width: " << rcItem.width() << " height: " << rcItem.height() << std::endl;
	std::cout << "OnLButtonDown, QPoint(" << pt.x() << ", " << pt.y() << ")." << std::endl;*/
	if (rcItem.contains(pt))
	{
		OnDrawUnitStart(pt);
	}
}

void PaintWidget::OnMouseMove(QPoint pt)
{
	MovePenIcon(pt);
	if (tea_shape_ == PaintShape_None || tea_shape_ == PaintShape_Erase)
	{
		QRect rcItem(0, 0, image_size_.width(), image_size_.height());
		if (rcItem.contains(pt))
		{
			OnDrawUnitProcess(pt);
		}
		PaintContent(image_);
	}
}

void PaintWidget::OnLButtonUp(QPoint pt)
{
	MovePenIcon(pt);
	QRect rcItem(0, 0, image_size_.width(), image_size_.height());
	if (rcItem.contains(pt))
	{
		OnDrawUnitEnd(pt);
	}
	PaintContent(image_);
}

void PaintWidget::ClearDrawedUnits()
{
	for (auto itor = list_draw_units_.begin(); itor != list_draw_units_.end();)
	{
		itor = list_draw_units_.erase(itor);
	}
	PaintContent(image_, true);
}