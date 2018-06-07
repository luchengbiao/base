#include "paint_image.h"
#include "QPainter"
#include "base\file\file_util.h"
#include "http_request\file_fetch.h"
#include "base\util\string_util.h"
#include "base\util\string_number_conversions.h"
#include "common\system\system_tool.h"
#define MAX_IMAGE_WIDTH 1920
#define MAX_IMAGE_HEIGHT 1080
#define THUMBNAIL_SIZE 100

PaintImage::PaintImage(PaintBaseInfo info, QWidget *parent /*= 0*/) :QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	image_size_ = QSize(info.width, info.height);
	origin_image_path_ = L"";
	image_url_ = "";
	image_dir_ = L"";
	bCenter_ = false;
	angle_ = 0;
	tooltip_ = L"";
	b_modify_ = false;
	b_loaded_image_ = false;
	download_image_cb_ = nullptr;
	bVisible_ = false;
	b_show_bk_ = true;
	vedio_url_ = "";
	connect(this, SIGNAL(SignalDownloadFileEnd(std::wstring)), this, SLOT(SlotSetBackImage(std::wstring)));
}

PaintImage::~PaintImage()
{

}

void PaintImage::SetBackgroundImage(std::wstring image_path)
{
	if (bVisible_)
	{
		image_ = QImage(image_size_, QImage::Format_ARGB32);
		image_.fill(qRgba(255, 255, 255, 255));
	}

	if (nbase::FilePathIsExist(image_path, false))
	{
		QImage load_image;
		CheckImageQuality(image_path, load_image);
		if (!load_image.isNull()
			&& load_image.width() > 0 && load_image.height() > 0)
		{
			origin_image_path_ = image_path;
			if (bVisible_)
			{
				ShowImage(load_image);
				update(); // 刷新页面
			}
		}
	}
	else if (bVisible_)
	{
		QImage load_image;
		ShowImage(load_image);

		update(); // 刷新页面
	}
}

void PaintImage::SetVisibleEx(bool bvisible)
{
	bVisible_ = bvisible;
	this->setVisible(bvisible);
	if (!nbase::FilePathIsExist(origin_image_path_, false)
		&& nbase::FilePathIsExist(image_dir_, true)
		&& !image_url_.empty() && bVisible_)
	{
		auto comp_cb = ToWeakCallback([this](std::string& path)
		{
			std::wstring wpath = nbase::UTF8ToUTF16(path);
			if (nbase::FilePathIsExist(wpath, false))
			{
				emit SignalDownloadFileEnd(wpath);
			}
			if (download_image_cb_)
			{
				download_image_cb_(nbase::FilePathIsExist(wpath, false));
			}
		});
		std::string file_name = nbase::StringPrintf("pic_%lld", systembase::get_time_ms());
		FileFetcher().AsyncFetchImage(image_url_, nbase::UTF16ToUTF8(image_dir_), comp_cb, true, nullptr, file_name);
	}
	else
	{
		if (bVisible_)
		{
			SetBackgroundImage(origin_image_path_);
		}
		else
		{
			b_loaded_image_ = false;
			image_ = QImage();
		}

		if (download_image_cb_)
		{
			download_image_cb_(true);
		}
	}
}

void PaintImage::SlotSetBackImage(std::wstring wpath)
{
	origin_image_path_ = wpath;
	SetBackgroundImage(wpath);
}

void PaintImage::paintEvent(QPaintEvent * event)
{
	if (!image_.isNull()
		&& b_loaded_image_ && b_show_bk_)
	{
		QPainter painter(this);
		painter.scale(1, 1);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.drawImage(0, 0, image_);
	}
}

void PaintImage::SetUrl(std::string url)
{
	image_url_ = url;
}

void PaintImage::SetImageDir(std::wstring image_dir)
{
	if (nbase::FilePathIsExist(image_dir, false))
	{
		image_dir_ = image_dir;
	}
}

void PaintImage::SetCenter(bool bCenter)
{
	bCenter_ = bCenter;
}

QImage PaintImage::GetBackgroundImage()
{
	QImage background;
	if (b_loaded_image_)
	{
		b_modify_ = false;
		background = image_;
	}
	
	return background;
}

void PaintImage::SetImageAngle(int angle)
{
	angle = ((angle % 360) + 360) % 360;
	if (angle_ != angle) {
		angle_ = angle;
		SetBackgroundImage(origin_image_path_);
	}
}

void PaintImage::ResizeImage(int width, int height)
{
	image_size_ = QSize(width, height);

	SetBackgroundImage(origin_image_path_);
}

void PaintImage::ShowImage(QImage& load_image)
{
	if (!load_image.isNull())
	{
		if (angle_ != 0) {
			QMatrix matrix;
			matrix.rotate(angle_);
			load_image = load_image.transformed(matrix);
		}
		load_image = load_image.scaled(image_.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		int x = 0, y = 0;
		if (bCenter_)
		{
			x = (image_.width() - load_image.width()) / 2;
			y = (image_.height() - load_image.height()) / 2;
		}
		img_rect_.setX(x);
		img_rect_.setY(y);
		img_rect_.setWidth(load_image.width());
		img_rect_.setHeight(load_image.height());
		QPainter painter(&image_);
		painter.drawImage(x, y, load_image);
	}

	if (!tooltip_.empty())
	{
		QPainter painter(&image_);
		painter.setPen(QColor(80, 80, 80));
		QFont font;
		font.setFamily("Microsoft YaHei");
		font.setPointSize(12);
		painter.setFont(font);

		// 绘制文本
		painter.drawText(QRect(0, 0, image_.width(), image_.height()), Qt::AlignCenter, QString::fromStdWString(tooltip_));
	}

	if (!mark_text_.empty())
	{
		QPainter painter(&image_);
		painter.setPen(QColor(5, 0xb5, 0xeb));
		QFont font;
		font.setFamily("Microsoft YaHei");
		font.setPointSize(12);
		painter.setFont(font);

		// 绘制文本
		painter.drawText(QRect(0, 5, image_.width() - 5, 50), Qt::AlignRight , QString::fromStdWString(mark_text_));
	}

	load_image = image_;
	b_loaded_image_ = true;
	b_modify_ = true;
	update();
}

void PaintImage::RotateImage()
{
	SetImageAngle(angle_ - 90);
}

QSize PaintImage::GetImageSize()
{
	return image_size_;
}

void PaintImage::SetText(std::wstring tooltip)
{
	tooltip_ = tooltip;
	SetBackgroundImage(origin_image_path_);
}

int PaintImage::GetAngle()
{
	return angle_;
}

std::wstring PaintImage::GetLocalImage()
{
	return origin_image_path_;
}

bool PaintImage::IsBackgroundModified()
{
	return b_modify_;
}

void PaintImage::SetDownloadImageCb(DownloadBoardImageCb cb)
{
	download_image_cb_ = cb;
}

bool PaintImage::CheckImageQuality(std::wstring& file_path, QImage& image)
{
	if (nbase::FilePathIsExist(file_path, false))
	{
		bool ret = image.load(QString::fromStdWString(file_path));
		if (image.width() > MAX_IMAGE_WIDTH && image.height() > MAX_IMAGE_HEIGHT)
		{
			QImage tmp_image = image.scaled(MAX_IMAGE_WIDTH, MAX_IMAGE_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			std::wstring file_dir = image_dir_;
			std::wstring file_name = L"";
			std::wstring suffix = L"";
			nbase::FilePathApartFileName(file_path, file_name);
			nbase::FilePathExtension(file_path, suffix);
			nbase::StringReplaceAll(suffix, L"", file_name);
			file_name.append(L"_new");
			file_dir.append(file_name).append(L".jpeg");
			if (tmp_image.save(QString::fromStdWString(file_dir), NULL, 80))
			{
				file_path = file_dir;
				image = tmp_image;
				return true;
			}
		}
	}
	return false;
}

void PaintImage::SetMarkText(std::wstring text)
{
	mark_text_ = text;
	SetBackgroundImage(origin_image_path_);
}

void PaintImage::SetShowBkImage(bool b_show)
{
	b_show_bk_ = b_show;
}

void PaintImage::SetVedioUrl(std::string url)
{
	vedio_url_ = url;
}

std::string PaintImage::GetVedioUrl()
{
	return vedio_url_;
}

void PaintImage::GetImageRect(QRect &rect)
{
	rect.setX(img_rect_.x());
	rect.setY(img_rect_.y());
	rect.setWidth(img_rect_.width());
	rect.setHeight(img_rect_.height());
}

