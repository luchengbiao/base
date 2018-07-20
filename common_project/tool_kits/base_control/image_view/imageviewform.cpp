#include "ImageViewForm.h"
#include "ui_ImageViewForm.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "base\util\string_util.h"
#include "base\file\file_util.h"
#include "QBoxLayout"
#include "imageviewtoolform.h"
#include "image_view_manager.h"
#include "QDesktopWidget"
#include "QRect"
#include "QApplication"
#include "QMatrix"
#include "QFileDialog"
#include "base\macros.h"
#include "QTimer"
#include "common\windows_manager\windows_manage.h"
#include "messagebox\msghint.h"

#define MINWNDWIDTH 700
#define MINWNDHEIGHT 500

const std::wstring ImageViewForm::kClassName = L"ImageViewForm";

ImageViewForm::ImageViewForm(QWidget *parent) : BaseWindow(parent), ui(new Ui::imageViewForm)
{
    ui->setupUi(this);
	this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
	this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口

	default_path_ = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	imgScaled_ = NULL;
	b_max_window_ = false;
	m_angle_ = 0;
	m_scale_ = 1.0;
	pic_moving_ = false;
	show_nextorpre_btn_ = false;
	image_path_ = L"";
	show_scale_ = false;
	point_off_ = QPoint(0, 0);
	toolbox_ = NULL;
	toolLayout_ = NULL;
	b_pressed_ = false;
}

ImageViewForm::~ImageViewForm()
{
	SAFE_DELETE(ui);
}

std::wstring ImageViewForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring ImageViewForm::GetWindowId() const
{
	return kClassName;
}

void ImageViewForm::InitWindow()
{
	SetCursor();
	QString str = QString::fromLocal8Bit("图片预览");
	setWindowTitle(str);
	QRect frameRect = ui->frame->geometry();
	int height = frameRect.height();
	int width = frameRect.width();
	ui->frame->setStyleSheet(QString("QFrame#frame{border-image:url(res/images/image_view/win_bg.png)16 16 16 16;"
		"border-top: 16px transparent;"
		"border-bottom: 16px transparent;"
		"border-right: 16px transparent;"
		"border-left: 16px transparent;}"));

	ui->btn_close2->setStyleSheet("QToolButton{border-image: url(res/images/image_view/close_max_1.png);padding-left:24px;}"
		"QToolButton:hover{border-image: url(res/images/image_view/close_max_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/close_max_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/close_max_4.png);}");
	ui->btn_close2->setGeometry(QRect(width - 24, 0, 24, 24));

	ui->btn_close->setStyleSheet("QToolButton{border-image: url(res/images/image_view/close_1.png);padding-left:24px;}"
		"QToolButton:hover{border-image: url(res/images/image_view/close_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/close_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/close_4.png);}");
	ui->btn_close->setGeometry(QRect(width - 62, 20, 42, 42));

	ui->btn_pre->setStyleSheet("QToolButton{border-image: url(res/images/image_view/left1.png);padding-left:24px;}"
		"QToolButton:disabled{border-image: url(res/images/image_view/left0.png);}");
	ui->btn_pre->setGeometry(QRect(20, (height - 66) / 2, 32, 66));

	ui->btn_next->setStyleSheet("QToolButton{border-image: url(res/images/image_view/right1.png);padding-left:24px;}"
		"QToolButton:disabled{border-image: url(res/images/image_view/right0.png);}");
	ui->btn_next->setGeometry(QRect(width - 52, (height - 66) / 2, 32, 66));

	ui->lab_num->setStyleSheet("QLabel{border-image: url(res/images/image_view/num_tip.png); font-family:'Microsoft YaHei';font-size:10px;color:#ffffff;}");
	ui->lab_num->setFixedWidth(50);
	ui->lab_num->setFixedHeight(20);
	ui->lab_num->setAlignment(Qt::AlignCenter);
	ui->lab_num->setVisible(false);

	toolLayout_ = new QHBoxLayout();
	toolbox_ = new ImageViewToolBox(this);
	toolLayout_->addWidget(toolbox_);
	ui->frame->setLayout(toolLayout_);

	ui->btn_next->setVisible(false);
	ui->btn_pre->setVisible(false);

	connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(OnCloseWindow()));
	connect(ui->btn_close2, SIGNAL(clicked()), this, SLOT(OnCloseWindow()));
	connect(ui->btn_pre, SIGNAL(clicked()), this, SLOT(OnPreImage()));
	connect(ui->btn_next, SIGNAL(clicked()), this, SLOT(OnNextImage()));
}

void ImageViewForm::SetNextOrPreImageBtnVisible(bool show)
{
	if (!show)
	{
		ui->btn_next->setVisible(false);
		ui->btn_pre->setVisible(false);
	}
	show_nextorpre_btn_ = show;
	AdjustToolbarStatus();
}

void ImageViewForm::SetWindowSize(int width, int height)
{
	this->setFixedWidth(width);
	this->setFixedHeight(height);

	QDesktopWidget *pDesk = QApplication::desktop();
	QRect deskRect = pDesk->availableGeometry();
	move((deskRect.width() - width) / 2, (deskRect.height() - height) / 2);

	ui->frame->setFixedWidth(width);
	ui->frame->setFixedHeight(height);

	ui->btn_close2->setGeometry(QRect(width - 42, 0, 42, 42));
	ui->btn_close2->setVisible(b_max_window_);
	ui->btn_close->setGeometry(QRect(width - 40, 16, 24, 24));
	ui->btn_close->setVisible(!b_max_window_);
	ui->btn_close->raise();
	ui->btn_pre->setGeometry(QRect(20, (height - 66) / 2, 32, 66));
	ui->btn_next->setGeometry(QRect(width - 52, (height - 66) / 2, 32, 66));

	QMargins margin((width - 415) / 2, height - 120, 0, 0);
	toolLayout_->setContentsMargins(margin);
}

bool ImageViewForm::StartViewPic(std::wstring path_pic, bool size, bool nosave)
{
	if (path_pic.empty()
		|| !m_image_.load(QString::fromStdWString(path_pic))) {
		
		m_image_.load("res/images/image_view/unload_image.png");
	}

	image_path_.clear();
	toolbox_->SetBtnSave(true, !nosave);

	image_path_ = path_pic;
	int wnd_width = 0;
	int wnd_height = 0;
	int bg_corner = 0;
	
	GetDefaultWindowSize(wnd_width, wnd_height, bg_corner, m_scale_);
	int width = this->width();
	int height = this->height();
	if (size && !b_max_window_)
	{
		width = wnd_width + bg_corner > MINWNDWIDTH ? wnd_width + bg_corner : MINWNDWIDTH;
		height = wnd_height + bg_corner > MINWNDHEIGHT ? wnd_height + bg_corner : MINWNDHEIGHT;
		SetWindowSize(width, height);
	}
	
	show_scale_ = true;
	int iwidth = imgScaled_->width();
	int iheight = imgScaled_->height();
	int ix = (width - iwidth) / 2;
	int iy = (height - iheight) / 2;
	ui->lab_image->setGeometry(QRect(ix, iy, iwidth, iheight));
	ui->lab_image->setPixmap(QPixmap::fromImage(*imgScaled_));
	ui->lab_image->setScaledContents(true);
	ui->lab_image->show();
	CheckNextOrPreImageBtnStatus();
	return true;

}

void ImageViewForm::OnNormalImage()
{
	SetScale(1.0);
}

void ImageViewForm::OnMaxWindow()
{
	b_max_window_ = true;
	
	ui->frame->setStyleSheet("QFrame#frame{border-image: url(res/images/image_view/win_bg_2.png);};");

	QDesktopWidget *pDesk = QApplication::desktop();
	QRect deskRect = pDesk->availableGeometry();
	int width = deskRect.width();
	int height = deskRect.height();
	SetWindowSize(width, height);
	

	show_scale_ = true;
	QRect rect = ui->lab_image->geometry();
	int iwidth = rect.width();
	int iheight = rect.height();
	int ix = (width - iwidth) / 2;
	int iy = (height - iheight) / 2;
	ui->lab_image->setGeometry(QRect(ix, iy, iwidth, iheight));
	AdjustToolbarStatus();
}

void ImageViewForm::OnRestoredWindow()
{
	int wnd_width = 0;
	int wnd_height = 0;
	int bg_corner = 0;
	float scale = 0;
	b_max_window_ = false;

	ui->frame->setStyleSheet(QString("QFrame#frame{border-image:url(res/images/image_view/win_bg.png)16 16 16 16;"
		"border-top: 16px transparent;"
		"border-bottom: 16px transparent;"
		"border-right: 16px transparent;"
		"border-left: 16px transparent;}"));

	GetDefaultWindowSize(wnd_width, wnd_height, bg_corner, scale);
	int width = wnd_width + bg_corner > MINWNDWIDTH ? wnd_width + bg_corner : MINWNDWIDTH;
	int height = wnd_height + bg_corner > MINWNDHEIGHT ? wnd_height + bg_corner : MINWNDHEIGHT;
	SetWindowSize(width, height);

	show_scale_ = true;
	QRect rect = ui->lab_image->geometry();
	int iwidth = rect.width();
	int iheight = rect.height();
	int ix = (width - iwidth) / 2;
	int iy = (height - iheight) / 2;
	ui->lab_image->setGeometry(QRect(ix, iy, iwidth, iheight));
	AdjustToolbarStatus();
}

void ImageViewForm::OnScaleImage(ImageZoom zoom)
{
	switch (zoom)
	{
	case kReduceImage:
	{
		if (m_scale_ <= 0.1)
		{
			return;
		}
		int scale = m_scale_ * 10;
		if (scale <= 10)
		{
			scale -= 1;
		}
		else if (scale <= 20)
		{
			scale -= 2;
		}
		else
		{
			scale -= 5;
		}

		if (scale < 1)
		{
			scale = 1;
		}

		SetScale(scale / 10.0f);
	}
	break;
	case kMagnifyImage:
	{
		if (m_scale_ >= 5.0)
		{
			return;
		}
		int scale = m_scale_ * 10;
		if (scale < 10)
		{
			scale += 1;
		}
		else if (scale < 20)
		{
			scale += 2;
		}
		else
		{
			scale += 5;
		}
		
		if (scale > 50)
		{
			scale = 50;
		}
		
		SetScale(scale / 10.0f);
	}
	break;
	}
}

void ImageViewForm::OnRotateImage()
{
	m_angle_ -= 90;
	m_angle_ = (m_angle_ + 360) % 360;

	QMatrix matrix;
	matrix.rotate(-90);
	*imgScaled_ = imgScaled_->transformed(matrix, Qt::SmoothTransformation);

	QRect rect = ui->lab_image->geometry();
	int width = rect.height();
	int height = rect.width();
	int wnd_width = this->width();
	int wnd_height = this->height();
	int x = (wnd_width - width) / 2;
	int y = (wnd_height - height) / 2;
	ui->lab_image->setPixmap(QPixmap::fromImage(*imgScaled_));
	ui->lab_image->setGeometry(QRect(x, y, width, height));
	AdjustToolbarStatus();
}

void ImageViewForm::OnSaveImage()
{
	QFileDialog dlg;
	dlg.setWindowTitle(QString::fromLocal8Bit("保存"));
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	dlg.setFileMode(QFileDialog::AnyFile);
	dlg.setViewMode(QFileDialog::Detail);
	dlg.setDirectory(default_path_);
	QStringList filters;
	filters << "*.png" << "*.jpg" << "*.jpeg";
	dlg.setNameFilters(filters);
	dlg.setDefaultSuffix(".png");
	if (dlg.exec() == QFileDialog::Accepted)
	{
		QString path = dlg.selectedFiles()[0];
		bool ret = m_image_.save(path);
		if (dlg.selectedNameFilter() == "*.png")
		{
			dlg.setDefaultSuffix(".png");
		}
		else if (dlg.selectedNameFilter() == "*.jpg")
		{
			dlg.setDefaultSuffix(".jpg");
		}
		else if (dlg.selectedNameFilter() == "*.jpeg")
		{
			dlg.setDefaultSuffix(".jpeg");
		}
		if (!ret)
		{
			ShowAutoCloseMsgBox(L"保存失败！请检查文件名或文件类型！");
		}
		QDir dir = dlg.directory();
		default_path_ = dir.absolutePath();
	}
}

void ImageViewForm::mousePressEvent(QMouseEvent *event)
{
	QPoint point = event->pos();

	if (IsImageMove(point))
	{
		pic_moving_ = true;
	}
	else
	{
		b_pressed_ = true;
	}
	point_off_ = point;
}

void ImageViewForm::mouseMoveEvent(QMouseEvent *event)
{
	QPoint point = event->pos();
	if (b_pressed_)
	{
		move(event->pos() - point_off_ + this->pos());
		return;
	}
	else if (!IsImageMove(point))
	{
		return;
	}

	if (pic_moving_)
	{
		if (point.x() == point_off_.x() && point.y() == point_off_.y())
		{
			return ;
		}
		QRect rect = ui->lab_image->geometry();

		int x = rect.x() - (point_off_.x() - point.x());
		int y = rect.y() - (point_off_.y() - point.y());
		QRect newRect(x, y, rect.width(), rect.height());

		ui->lab_image->setGeometry(newRect);
		point_off_ = point;

		ui->lab_image->setCursor(Qt::PointingHandCursor);
	}
	else
	{
		ui->lab_image->setCursor(Qt::SizeAllCursor);
	}
}

void ImageViewForm::mouseReleaseEvent(QMouseEvent *event)
{
	if (pic_moving_)
	{
		pic_moving_ = false;
		ui->lab_image->setCursor(Qt::SizeAllCursor);
	}

	b_pressed_ = false;
}

bool ImageViewForm::GetDefaultWindowSize(int &wnd_width, int &wnd_height, int &bg_corner, float& scale)
{
	int image_width = m_image_.width();
	int image_height = m_image_.height();

	if (m_angle_ == 90 || m_angle_ == 270)
	{
		image_width = m_image_.height();
		image_height = m_image_.width();
	}

	bg_corner = 30;
	if (b_max_window_)
	{
		bg_corner = 0;
	}

	RECT rect_workarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect_workarea, 0);
	int cxScreenWork = rect_workarea.right - rect_workarea.left;
	int cyScreenWork = rect_workarea.bottom - rect_workarea.top;

	scale = GetDefaultScale(cxScreenWork - bg_corner, cyScreenWork - bg_corner, image_width, image_height);
	wnd_width = int(image_width * scale);
	wnd_height = int(image_height * scale);
	if (imgScaled_)
	{
		SAFE_DELETE(imgScaled_);
	}
	imgScaled_ = new QImage();

	QMatrix matrix;
	matrix.rotate(m_angle_);
	*imgScaled_ = m_image_.transformed(matrix, Qt::SmoothTransformation);

	*imgScaled_ = imgScaled_->scaled(wnd_width, wnd_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	if (wnd_width > cxScreenWork - bg_corner)
		wnd_width = cxScreenWork - bg_corner;
	if (wnd_height > cyScreenWork - bg_corner)
		wnd_height = cyScreenWork - bg_corner;
	return true;
}

float ImageViewForm::GetDefaultScale(int control_width, int control_height, int image_width, int image_height)
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

void ImageViewForm::CheckNextOrPreImageBtnStatus()
{
	bool next = ImageViewManager::GetInstance()->IsExistNextImage(true);
	bool pre = ImageViewManager::GetInstance()->IsExistNextImage(false);

	ui->btn_next->setEnabled(next);
	std::wstring tip = next ? L"" : L"最后一张";
	ui->btn_next->setToolTip(QString::fromStdWString(tip));

	ui->btn_pre->setEnabled(pre);
	tip = pre ? L"" : L"最早一张";
	ui->btn_pre->setToolTip(QString::fromStdWString(tip));

	UpdateImagePage();
	AdjustToolbarStatus();
}

void ImageViewForm::UpdateImagePage()
{
	long sum = 0;
	long cur = 0;
	ImageViewManager::GetInstance()->GetImagePage(cur, sum);
	if (sum > 1 && cur > 0)
	{
		std::wstring num;
		nbase::StringPrintf(num, L"%d / %d", cur, sum);
		ui->lab_num->setText(QString::fromStdWString(num));
		ui->lab_num->setVisible(true);
	}
	else
	{
		ui->lab_num->setVisible(false);
	}
}

void ImageViewForm::AdjustToolbarStatus()
{
	bool show_btn = true;

	if (show_nextorpre_btn_)
	{
		ui->btn_next->setVisible(show_btn);
		ui->btn_pre->setVisible(show_btn);
	}
	if (show_btn)
	{
		int w = 0, h = 0;
		bool b_normal = false;
		if (IsScale(1.0, w, h)
			&& abs(m_scale_ - 1.0) > 0.000001)
		{
			b_normal = true;
		}
		toolbox_->SetBtnNormal(true, b_normal);

		if (b_max_window_)
		{
			toolbox_->SetBtnMax(false, true);
			toolbox_->SetBtnRestore(true, true);
		}
		else
		{
			toolbox_->SetBtnMax(true, true);
			toolbox_->SetBtnRestore(false, true);
		}
	}
}

void ImageViewForm::OnCloseWindow()
{
	this->close();
}

void ImageViewForm::OnPreImage()
{
	ImageViewManager::GetInstance()->ShowNextImageView(false);
}

void ImageViewForm::OnNextImage()
{
	ImageViewManager::GetInstance()->ShowNextImageView(true);
}

void ImageViewForm::OnHideScaleHite()
{
	ShowImageScale(false);
}

bool ImageViewForm::IsScale(float scale, int& iwidth, int& iheight)
{
	if (scale <= 0.0) {
		return false;
	}

	iwidth = m_image_.width();
	iheight = m_image_.height();
	if (m_angle_ == 90 || m_angle_ == 270)
	{
		iwidth = m_image_.height();
		iheight = m_image_.width();
	}
	iwidth *= scale;
	iheight *= scale;
	QDesktopWidget *pDesk = QApplication::desktop();
	QRect deskRect = pDesk->availableGeometry();
	int deskWidth = deskRect.width();
	int deskHeight = deskRect.height();
	if (m_angle_ == 90 || m_angle_ == 270)
	{
		deskHeight = deskRect.width();
		deskWidth = deskRect.height();
	}
	if (iwidth > deskWidth * 2
		|| iheight > deskHeight * 2)
	{
		return false;
	}

	return true;
}

void ImageViewForm::SetScale(float scale)
{
	int iwidth = 0, iheight = 0;
	if (!IsScale(scale, iwidth, iheight))
	{
		return;
	}

	m_scale_ = scale;
	int width = this->width();
	int height = this->height();
	int ix = (width - iwidth) / 2;
	int iy = (height - iheight) / 2;
	ui->lab_image->setGeometry(QRect(ix, iy, iwidth, iheight));

	ShowImageScale(true);
	AdjustToolbarStatus();
}

void ImageViewForm::ShowImageScale(bool show)
{
	if (show)
	{
		QTimer::singleShot(2000, this, SLOT(OnHideScaleHite()));
		std::wstring size;
		int size_hum = int(m_scale_ * 100);
		
		if (size_hum % 10 >= 5)
		{
			size_hum = size_hum / 10 * 10 + 10;
		}
		else
		{
			size_hum = size_hum / 10 * 10;
		}

		size = nbase::StringPrintf(L"%d%%", size_hum);
		ui->lab_num->setText(QString::fromStdWString(size));
		int width = this->width();
		int height = this->height();
		ui->lab_num->setGeometry(QRect((width - 50) / 2, (height - 20) / 2, 50, 20));
		ui->lab_num->setVisible(true);
	}
	else
	{
		ui->lab_num->setVisible(false);
	}

}

bool ImageViewForm::IsImageMove(QPoint point)
{
	int width = this->width();
	int height = this->height();
	QRect rect = ui->lab_image->geometry();
	if (rect.contains(point)
		&& (width < rect.width() || height < rect.height()))
	{
		return true;
	}
	return false;
}

ImageViewForm* ImageViewForm::GetSelfWindow()
{
	return (ImageViewForm*)WindowsManager::GetInstance()->GetWindow(ImageViewForm::kClassName, ImageViewForm::kClassName);
}

void ImageViewForm::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		//进行界面退出，重写Esc键，否则重写reject()方法
	case Qt::Key_Escape:
	{
		if (b_max_window_)
		{
			OnRestoredWindow();
		}
		break;
	}
	default:
		break;
	}
}

void ImageViewForm::SetCursor()
{
	ui->btn_close->setCursor(Qt::PointingHandCursor);
	ui->btn_close2->setCursor(Qt::PointingHandCursor);
	ui->btn_next->setCursor(Qt::PointingHandCursor);
	ui->btn_pre->setCursor(Qt::PointingHandCursor);
}
