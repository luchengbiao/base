#pragma once
#include <wtypes.h>
#include "QtGui\QColor"
#include <functional>

using DownloadBoardImageCb = std::function<void(bool b_success)>;

enum DrawOpType
{
	DrawOpNone = 0,
	DrawOpStart,
	DrawOpMove,
	DrawOpEnd,
	DrawOpUndoCb,
	DrawOpClearCb,
	DrawOpPenColor,
	EraserOpStart,
	EraserOpMove,
	EraserOpEnd,
};

enum Shape {
	PaintShape_None = 0,
	PaintShape_Line,
	PaintShape_Rect,
	PaintShape_Elic,
	PaintShape_Erase
};


enum DrawRecordSign {
	DrawRecordSign_None = 0,
	DrawRecordSign_StuDraw = 1,
	DrawRecordSign_TeaDraw = 2,
	DrawRecordSign_StuClear = 4,
	DrawRecordSign_TeaClear = 8
};

struct DrawOpInfo
{
	DrawOpType draw_op_type_;
	double x_;
	double y_;
	QColor pen_color_;
	DrawOpInfo()
	{
		draw_op_type_ = DrawOpNone;
		x_ = 0;
		y_ = 0;
	}
};


struct PaintBaseInfo
{
	int width;
	int height;
	int m_type_;
	bool b_accept_event_;
	int offset_x_;
	int offset_y_;
	int page_;
	bool b_stu_;

	PaintBaseInfo() : width(0), height(0), m_type_(0),
		b_accept_event_(true), offset_x_(0), offset_y_(0),
		page_(0), b_stu_(false)
	{

	}
};