#include "imageclipform.h"
#include "ui_imageclipform.h"
#include "QDesktopWidget"
#include "QPixmap"
#include "base\util\string_util.h"
#include "qthread_manager\closure.h"
#include "base\macros.h"
#include "qevent.h"
#include "common\windows_manager\windows_manage.h"
#include "common\system\system_tool.h"
#include "messagebox\msghint.h"
#include "QObject"

#define MINWNDWIDTH 700
#define MINWNDHEIGHT 500
#define MINCLIPLEN 150

const std::wstring ImageClipWindow::kClassName = L"ImageClipWindow";

ImageClipWindow::ImageClipWindow(QWidget *parent) :BaseWindow(parent), ui(new Ui::ImageClipForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
	b_pressed_ = false;
}

ImageClipWindow::~ImageClipWindow()
{
	SAFE_DELETE(ui);
}

std::wstring ImageClipWindow::GetWindowClassName() const
{
	return kClassName;
}

std::wstring ImageClipWindow::GetWindowId() const
{
	return kClassName;
}

void ImageClipWindow::InitWindow()
{
	QString str = QString::fromLocal8Bit("截取图片");
	setWindowTitle(str);
	this->setLayout(ui->vlayout_box);

	ui->lab_top->setFixedHeight(0);
	ui->lab_top->setMinimumWidth(50);
	ui->lab_bottom->setFixedHeight(0);
	ui->lab_bottom->setMinimumWidth(50);
	ui->lab_left->setFixedWidth(0);
	ui->lab_left->setMinimumHeight(50);
	ui->lab_right->setFixedWidth(0);
	ui->lab_right->setMinimumHeight(50);

	ui->lab_top_shadow->setFixedHeight(0);
	ui->lab_top_shadow->setMinimumWidth(50);
	ui->lab_bottom_shadow->setFixedHeight(0);
	ui->lab_bottom_shadow->setMinimumWidth(50);
	ui->lab_left_shadow->setFixedWidth(0);
	ui->lab_left_shadow->setMinimumHeight(50);
	ui->lab_right_shadow->setFixedWidth(0);
	ui->lab_right_shadow->setMinimumHeight(50);

	QImage shadowImage;
	if (shadowImage.load("res/images/image_clip/trans.png"))
	{
		ui->lab_top_shadow->setPixmap(QPixmap::fromImage(shadowImage));
		ui->lab_top_shadow->setScaledContents(true);

		ui->lab_bottom_shadow->setPixmap(QPixmap::fromImage(shadowImage));
		ui->lab_bottom_shadow->setScaledContents(true);

		ui->lab_left_shadow->setPixmap(QPixmap::fromImage(shadowImage));
		ui->lab_left_shadow->setScaledContents(true);

		ui->lab_right_shadow->setPixmap(QPixmap::fromImage(shadowImage));
		ui->lab_right_shadow->setScaledContents(true);
	}

	ui->lab_top_left->setStyleSheet("QLabel{border-image: url(res/images/image_clip/left_top.png);padding-left:24px;}");
	ui->lab_top_left->setFixedSize(QSize(56, 56));

	ui->lab_top_right->setStyleSheet("QLabel{border-image: url(res/images/image_clip/right_top.png);padding-left:24px;}");
	ui->lab_top_right->setFixedSize(QSize(56, 56));

	ui->lab_bottom_left->setStyleSheet("QLabel{border-image: url(res/images/image_clip/left_bottom.png);padding-left:24px;}");
	ui->lab_bottom_left->setFixedSize(QSize(56, 56));

	ui->lab_bottom_right->setStyleSheet("QLabel{border-image: url(res/images/image_clip/right_bottom.png);padding-left:24px;}");
	ui->lab_bottom_right->setFixedSize(QSize(56, 56));

	ui->btn_cancel->setStyleSheet("QPushButton{background-image:url(res/images/image_clip/btnB51.png);border:none;font-family:'Microsoft YaHei';font-size:13px;color:#555555;}"
		"QPushButton:hovre{background-image:url(res/images/image_clip/btnB52.png);}"
		"QPushButton:pressed{background-image:url(res/images/image_clip/btnB53.png);}");
	ui->btn_cancel->setFixedSize(QSize(76, 27));

	ui->btn_rotate->setStyleSheet("QPushButton{background-image:url(res/images/image_clip/btnB51.png);border:none;font-family:'Microsoft YaHei';font-size:13px;color:#555555;}"
		"QPushButton:hovre{background-image:url(res/images/image_clip/btnB52.png);}"
		"QPushButton:pressed{background-image:url(res/images/image_clip/btnB53.png);}");
	ui->btn_rotate->setFixedSize(QSize(76, 27));

	ui->btn_upload_2->setStyleSheet("QPushButton#btn_upload_2{background-image:url(res/images/image_clip/btnB51.png);border:none;font-family:'Microsoft YaHei';font-size:13px;color:#555555;}"
		"QPushButton#btn_upload_2:hovre{background-image:url(res/images/image_clip/btnB52.png);}"
		"QPushButton#btn_upload_2:pressed{background-image:url(res/images/image_clip/btnB53.png);}");
	ui->btn_upload_2->setFixedSize(QSize(76, 27));

	ui->lab_top_left->setCursor(Qt::SizeFDiagCursor);
	ui->lab_top_right->setCursor(Qt::SizeBDiagCursor);
	ui->lab_bottom_right->setCursor(Qt::SizeFDiagCursor);
	ui->lab_bottom_left->setCursor(Qt::SizeBDiagCursor);

	m_angle_ = 0;

	connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(OnCloseWindow()));
	connect(ui->btn_rotate, SIGNAL(clicked()), this, SLOT(OnRotateImage()));
	connect(ui->btn_upload_2, SIGNAL(clicked()), this, SLOT(OnUploadImage()));
}

bool ImageClipWindow::StartViewPic(std::wstring oriPath, std::wstring destPath, const image_clip_callback& cb)
{
	if (oriPath.empty()
		|| !m_image_.load(QString::fromStdWString(oriPath))) {
		return false;
	}

	s_clip_image_path_ = destPath;
	clip_cb_ = cb;
	m_angle_ = 0;

	int wnd_width = 0;
	int wnd_height = 0;

	GetDefaultWindowSize(wnd_width, wnd_height);
	int width = this->width();
	int height = this->height();
	
	width = wnd_width  > MINWNDWIDTH ? wnd_width  : MINWNDWIDTH;
	height = wnd_height > MINWNDHEIGHT ? wnd_height  : MINWNDHEIGHT;
	SetWindowSize(width, height);
	
	int iwidth = imgScaled_.width();
	int iheight = imgScaled_.height();
	int ix = (width - iwidth) / 2;
	int iy = (height - iheight - 47) / 2;
	ui->lab_image->setGeometry(QRect(ix, iy, iwidth, iheight));
	ui->lab_image->setPixmap(QPixmap::fromImage(imgScaled_));
	ui->lab_image->setScaledContents(true);
	ui->lab_image->show();
	return true;

}

bool ImageClipWindow::GetDefaultWindowSize(int &wnd_width, int &wnd_height)
{
	int image_width = m_image_.width();
	int image_height = m_image_.height();
	int hor_corner = 50;
	int ver_corner = 97;

	RECT rect_workarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect_workarea, 0);
	int cxScreenWork = rect_workarea.right - rect_workarea.left;
	int cyScreenWork = rect_workarea.bottom - rect_workarea.top;

	float scale = GetDefaultScale(cxScreenWork - hor_corner, cyScreenWork - ver_corner, image_width, image_height);
	wnd_width = int(image_width * scale);
	wnd_height = int(image_height * scale);

	imgScaled_ = m_image_.scaled(wnd_width, wnd_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	if (wnd_width + hor_corner <= cxScreenWork)
		wnd_width = wnd_width + hor_corner;
	if (wnd_height + ver_corner <= cyScreenWork)
		wnd_height = wnd_height + ver_corner;
	return true;
}

float ImageClipWindow::GetDefaultScale(int control_width, int control_height, int image_width, int image_height)
{
	float fScale = 10.0;
	int width_sub = control_width - image_width;
	int height_sub = control_height - image_height;

	if (width_sub < 0 || height_sub < 0)//超出显示区域
	{
		fScale = float(control_width * 10.0 / image_width);
		fScale = min(fScale, float(control_height * 10.0 / image_height));
	}

	if (fScale < 1)
	{
		fScale = 1;
	}
	return fScale / 10;
}

void ImageClipWindow::SetWindowSize(int width, int height)
{
	this->setFixedSize(width, height);

	int iwidth = imgScaled_.width();
	int iheight = imgScaled_.height();

	int iview_width = width - 50;
	int iview_height = height - 97;
	int tmp_height = max((iview_height - iheight) / 2, 0);
	int tmp_width = max((iview_width - iwidth) / 2, 0);

	ui->lab_top->setFixedHeight(tmp_height);
	ui->lab_bottom->setFixedHeight(tmp_height);
	ui->lab_left->setFixedWidth(tmp_width);
	ui->lab_right->setFixedWidth(tmp_width);

	ui->lab_top_shadow->setFixedHeight(0);
	ui->lab_bottom_shadow->setFixedHeight(0);
	ui->lab_left_shadow->setFixedWidth(0);
	ui->lab_right_shadow->setFixedWidth(0);

	QDesktopWidget *pDesk = QApplication::desktop();
	QRect deskRect = pDesk->availableGeometry();
	move((deskRect.width() - width) / 2, (deskRect.height() - height) / 2);
}

void ImageClipWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
	{
		return;
	}
	QPoint point = event->pos();
	b_clip_moving_ = false;
	hit_flag_ = 0;
	if (IsClipMove(point))
	{
		b_clip_moving_ = true;
	}
	else
	{
		b_pressed_ = true;
	}
	point_off_ = point;
}

void ImageClipWindow::mouseMoveEvent(QMouseEvent *event)
{
	QPoint point = event->pos();
	
	if (b_clip_moving_)
	{
		if (point.x() == point_off_.x() && point.y() == point_off_.y())
		{
			return;
		}
		int x = point.x() - point_off_.x();
		int y = point.y() - point_off_.y();
		if (hit_flag_ & 1)
		{
			int height = ui->lab_top_shadow->height() + y;
			height = max(height, 0);
			int width = ui->lab_left_shadow->width() + x;
			width = max(width, 0);
			ui->lab_top_shadow->setFixedHeight(height);
			ui->lab_left_shadow->setFixedWidth(width);

			if (ui->image_vlayout2->geometry().height() <= MINCLIPLEN)
			{
				height += ui->image_vlayout2->geometry().height() - MINCLIPLEN;
				ui->lab_top_shadow->setFixedHeight(height);
			}
			if (ui->image_vlayout2->geometry().width() <= MINCLIPLEN)
			{
				width += ui->image_vlayout2->geometry().width() - 150;
				ui->lab_left_shadow->setFixedWidth(width);
			}
		}
		else if (hit_flag_ & 2)
		{
			int oriHeight = ui->lab_top_shadow->height();
			int oriWidth = ui->lab_right_shadow->width();

			int height = ui->lab_top_shadow->height() + y;
			height = max(height, 0);
			int width = ui->lab_right_shadow->width() - x;
			width = max(width, 0);
			ui->lab_top_shadow->setFixedHeight(height);
			ui->lab_right_shadow->setFixedWidth(width);

			if (ui->image_vlayout2->geometry().height() <= MINCLIPLEN)
			{
				height += ui->image_vlayout2->geometry().height() - MINCLIPLEN;
				ui->lab_top_shadow->setFixedHeight(height);
			}
			if (ui->image_vlayout2->geometry().width() <= MINCLIPLEN)
			{
				width += ui->image_vlayout2->geometry().width() - MINCLIPLEN;
				ui->lab_right_shadow->setFixedWidth(width);
			}
		}
		else if (hit_flag_ & 4)
		{
			int height = ui->lab_bottom_shadow->height() - y;
			height = max(height, 0);
			int width = ui->lab_right_shadow->width() - x;
			width = max(width, 0);
			ui->lab_bottom_shadow->setFixedHeight(height);
			ui->lab_right_shadow->setFixedWidth(width);

			if (ui->image_vlayout2->geometry().height() <= MINCLIPLEN)
			{
				height += ui->image_vlayout2->geometry().height() - MINCLIPLEN;
				ui->lab_bottom_shadow->setFixedHeight(height);
			}
			if (ui->image_vlayout2->geometry().width() <= MINCLIPLEN)
			{
				width += ui->image_vlayout2->geometry().width() - MINCLIPLEN;
				ui->lab_right_shadow->setFixedWidth(width);
			}
		}
		else if (hit_flag_ & 8)
		{
			int height = ui->lab_bottom_shadow->height() - y;
			height = max(height, 0);
			int width = ui->lab_left_shadow->width() + x;
			width = max(width, 0);
			ui->lab_bottom_shadow->setFixedHeight(height);
			ui->lab_left_shadow->setFixedWidth(width);

			if (ui->image_vlayout2->geometry().height() <= MINCLIPLEN)
			{
				height += ui->image_vlayout2->geometry().height() - MINCLIPLEN;
				ui->lab_bottom_shadow->setFixedHeight(height);
			}
			if (ui->image_vlayout2->geometry().width() <= MINCLIPLEN)
			{
				width += ui->image_vlayout2->geometry().width() - MINCLIPLEN;
				ui->lab_left_shadow->setFixedWidth(width);
			}
		}
		point_off_ = point;
	}
	else if (b_pressed_)
	{
		move(event->pos() - point_off_ + this->pos());
	}
}

void ImageClipWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (b_clip_moving_)
	{
		hit_flag_ = 0;
		b_clip_moving_ = false;
	}

	b_pressed_ = false;
}

bool ImageClipWindow::IsClipMove(QPoint point)
{
	if (ui->lab_top_left->geometry().contains(point))
	{
		hit_flag_ |= 1;
		return true;
	}
	else if (ui->lab_top_right->geometry().contains(point))
	{
		hit_flag_ |= 2;
		return true;
	}
	else if (ui->lab_bottom_right->geometry().contains(point))
	{
		hit_flag_ |= 4;
		return true;
	}
	else if (ui->lab_bottom_left->geometry().contains(point))
	{
		hit_flag_ |= 8;
		return true;
	}
	return false;
}

void ImageClipWindow::OnCloseWindow()
{
	this->close();
}

void ImageClipWindow::OnRotateImage()
{
	m_angle_ += 90;
	m_angle_ = (m_angle_ + 360) % 360;

	QMatrix matrix;
	matrix.rotate(90);
	m_image_ = m_image_.transformed(matrix, Qt::SmoothTransformation);

	int wnd_width = 0;
	int wnd_height = 0;

	GetDefaultWindowSize(wnd_width, wnd_height);
	int width = this->width();
	int height = this->height();

	width = wnd_width > MINWNDWIDTH ? wnd_width : MINWNDWIDTH;
	height = wnd_height > MINWNDHEIGHT ? wnd_height : MINWNDHEIGHT;
	SetWindowSize(width, height);

	int iwidth = imgScaled_.width();
	int iheight = imgScaled_.height();
	int ix = (width - iwidth) / 2;
	int iy = (height - iheight - 47) / 2;
	ui->lab_image->setGeometry(QRect(ix, iy, iwidth, iheight));
	ui->lab_image->setPixmap(QPixmap::fromImage(imgScaled_));
	ui->lab_image->setScaledContents(true);
	ui->lab_image->show();
}

void ImageClipWindow::OnUploadImage()
{
	QRect clipRect = ui->image_vlayout2->geometry();
	QRect imageRect = ui->lab_image->geometry();
	QRect rect = QRect(clipRect.x() - imageRect.x(), clipRect.y() - imageRect.y(), clipRect.width(), clipRect.height());
	
	QPixmap pixmap = QPixmap::grabWidget(ui->lab_image, rect);

	std::wstring destPath = s_clip_image_path_;
	std::wstring fileName = nbase::StringPrintf(L"image_tea_%lld.png", systembase::get_time_ms());
	destPath.append(fileName);
	pixmap.save(QString::fromStdWString(destPath));

	clip_cb_(destPath);
	
	close();
}

ImageClipWindow* ImageClipWindow::GetSelfWindow()
{
	return (ImageClipWindow*)WindowsManager::GetInstance()->GetWindow(ImageClipWindow::kClassName, ImageClipWindow::kClassName);
}

void ImageClipWindow::ShowSelfWindow(std::wstring oriPath, std::wstring destPath, const image_clip_callback& cb)
{
	ImageClipWindow *window = GetSelfWindow();
	if (!window)
	{
		window = new ImageClipWindow();
		window->Create();
		bool read_success = window->StartViewPic(oriPath, destPath, cb);
		if (read_success)
		{
			window->show();
		}
		else
		{
			ShowAutoCloseMsgBox(QObject::tr("COMMON_NOT_SUPPORTED").toStdWString());
			window->close();
		}
	}
	else
	{
		window->show();
		window->raise();
		window->activateWindow();
	}
}
