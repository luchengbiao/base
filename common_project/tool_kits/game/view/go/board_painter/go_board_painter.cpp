#include "go_board_painter.h"
#include <vector>
#include <QtGlobal>
#include <QImage>
#include <QPainter>
#include <QPen>
#include "game/model/base/grid/grid_index.h"
#include "game/model/go/go_trait.h"
#include "common/macros/assert.h"

GAME_NAMESPACE_BEGIN

QPixmap GoBoardPainter::CreatePixmap(int grid_count)
{
	D_ASSERT(GoTrait::IsGridValid(grid_count), L"Invalid grid count");

	const auto& board_info = GoTrait::GoBoardInfoByGrid(grid_count);

	QImage canvas(R"(:go/res/images/go/go_board_bk.png)");
	D_ASSERT(!canvas.isNull(), L"bk_image cannot be null");

	const int grid_width = board_info.grid_width_;
	const QPoint grid_offset = board_info.offset_;

	const int outline_line_width = 4;
	const int grid_line_width = 1;

	const int gap_outline_grids = 3 + outline_line_width / 2;
	const int width_grids = (grid_count - 1) * grid_width;

	QPainter painter(&canvas);

	// draw outline
	{
		painter.setPen(QPen(QColor(69, 38, 18), outline_line_width));

		QPoint outline_lt = grid_offset - QPoint(gap_outline_grids, gap_outline_grids);
		QPoint outline_rb = grid_offset + QPoint(width_grids, width_grids) + QPoint(gap_outline_grids, gap_outline_grids);
		painter.drawRoundedRect(QRect(outline_lt, outline_rb), 1, 1);
	}


	// draw grid lines
	{
		painter.setPen(QPen(QColor(69, 38, 18), grid_line_width));

		int offset = 0;
		QVector<QLine> lines;
		for (int i = 0; i < grid_count; ++i)
		{
			lines.push_back(QLine(grid_offset + QPoint(0, offset), grid_offset + QPoint(width_grids, offset))); // horizontal
			lines.push_back(QLine(grid_offset + QPoint(offset, 0), grid_offset + QPoint(offset, width_grids))); // vertical

			offset += grid_width;
		}
		painter.drawLines(lines);
	}


	// draw tengen dots
	{
		QImage dot(R"(:go/res/images/go/dot.png)");
		D_ASSERT(!dot.isNull(), L"dot cannot be null");
		QPoint dot_offset(dot.width() / 2, dot.height() / 2);

		static std::vector<GridIndex> dot_grids_19{ { 3, 3 }, { 3, 9 }, { 3, 15 }, { 9, 3 }, { 9, 9 }, { 9, 15 }, { 15, 3 }, { 15, 9 }, { 15, 15 } };
		static std::vector<GridIndex> dot_grids_13{ { 3, 3 }, { 3, 9 }, { 6, 6 }, { 9, 3 }, { 9, 9 } };
		static std::vector<GridIndex> dot_grids_9{ { 2, 2 }, { 2, 6 }, { 4, 4 }, { 6, 2 }, { 6, 6 } };

		const auto& dot_grids = grid_count == 9 ? dot_grids_9 : (grid_count == 13 ? dot_grids_13 : dot_grids_19);
		for (const auto& grid : dot_grids)
		{
			painter.drawImage(grid_offset + QPoint(grid_width * grid.X(), grid_width * grid.Y()) - dot_offset, dot);
		}
	}


	// draw scale texts
	/*
	{
		painter.setRenderHint(QPainter::Antialiasing);

		std::string x_texts[19] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S" };
		std::string y_texts[19] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };

		QFont font("Microsoft YaHei", 8, QFont::Medium);
		font.setPixelSize(12);
		painter.setFont(font);
		QSize sz_font(20, 20);
		QPoint offset_font(-sz_font.width() / 2, -sz_font.height() / 2);
		QTextOption x_txt_opt(Qt::AlignCenter);
		QTextOption y_txt_opt(Qt::AlignCenter);

		int gap_text_grids = gap_outline_grids + outline_line_width + 5;
		painter.setPen(QPen(QColor(255, 255, 255, 128), 2));
		int offset = 0;
		for (int i = 0; i < grid_count; ++i)
		{
			painter.drawText(QRect(grid_offset + QPoint(offset, -gap_text_grids) + offset_font, sz_font), QString::fromStdString(x_texts[i]), x_txt_opt); // x
			painter.drawText(QRect(grid_offset + QPoint(-gap_text_grids, offset) + offset_font, sz_font), QString::fromStdString(y_texts[i]), y_txt_opt); // y

			offset += grid_width;
		}

		painter.setPen(QPen(QColor(0, 0, 0), 2));
		offset = 0;
		for (int i = 0; i < grid_count; ++i)
		{
			painter.drawText(QRect(grid_offset + QPoint(offset, -gap_text_grids - 1) + offset_font, sz_font), QString::fromStdString(x_texts[i]), x_txt_opt); // x
			painter.drawText(QRect(grid_offset + QPoint(-gap_text_grids, offset - 1) + offset_font, sz_font), QString::fromStdString(y_texts[i]), y_txt_opt); // y

			offset += grid_width;
		}
	}
	*/

	return QPixmap::fromImage(canvas);
}

GAME_NAMESPACE_END