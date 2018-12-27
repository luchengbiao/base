#include "head_portrait.h"
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QResizeEvent>
#include "base/util/string_util.h"
#include "game/public/downloaded_files_recorder/downloaded_files_recorder.h"
#include "game/public/public_setting/public_setting.h"
#include "game/view/widget_with_pixmap/widget_with_pixmap.h"
#include "http_request/file_fetch.h"

GAME_NAMESPACE_BEGIN

static std::unique_ptr<QImage> s_stu_default_image;
static std::unique_ptr<QImage> s_tea_default_image;
static std::unique_ptr<QImage> s_mask_image;
static std::unique_ptr<QImage> s_border_image;

HeadPortrait::HeadPortrait(QWidget* parent)
: HeadPortrait(true, parent)
{}

HeadPortrait::HeadPortrait(bool is_teahcer, QWidget* parent)
: QWidget(parent)
, QObjectSupportWeakCallback(this)
, is_teacher_(is_teahcer)
{
	if (MyObjectCounter::live() == 1)
	{
		s_stu_default_image.reset(new QImage(R"(:portrait/res/images/portrait/default_stu.png)"));
		s_tea_default_image.reset(new QImage(R"(:portrait/res/images/portrait/default_tea.png)"));
		s_mask_image.reset(new QImage(R"(:portrait/res/images/portrait/mask_44.png)"));
		s_border_image.reset(new QImage(R"(:portrait/res/images/portrait/border_44.png)"));
	}

	this->setWindowFlags(Qt::FramelessWindowHint);

	this->Init();
}

HeadPortrait::~HeadPortrait()
{
	if (MyObjectCounter::live() == 1)
	{
		s_stu_default_image.reset();
		s_tea_default_image.reset();
		s_mask_image.reset();
		s_border_image.reset();
	}
}

void HeadPortrait::Init()
{
	portrait_ = new WidgetWithPixmap(this);
}

void HeadPortrait::resizeEvent(QResizeEvent *resize_event)
{
	auto size = resize_event->size();

	portrait_->setGeometry(0, 0, size.width(), size.height());

	this->UpdateHeadPortrait();
}

void HeadPortrait::SetHeadPortraitViaUrl(const std::string& file_url)
{
	std::wstring file_path;
	if (DOWNLOADED_FILES_RECORDER->ExistingFileOfUrl(file_url, &file_path))
	{
		this->SetHeadPortraitViaFile(file_path);
	}
	else
	{
		this->SetHeadPortraitViaFile(std::wstring());

		auto weak_helper(weak_callback_helper_->GetWeakPtr());
		auto cb_completed = weak_callback_helper_->ToWeakCallback([=](const std::string& file_path){
			auto helper = weak_helper.lock(); // helper is definitely not nullptr here if callback can be invoked from WeakCallback
			auto file_path_w = nbase::UTF8ToUTF16(file_path);
			DOWNLOADED_FILES_RECORDER->Record(file_url, file_path_w);

			helper->EmitClosureToHolder([=]{
				if (file_url_ == file_url)
				{
					this->SetHeadPortraitViaFile(file_path_w);
				}
			});
		});

		file_url_ = file_url;

		FileFetcher ffetcher;
		ffetcher.AsyncFetchFile(std::string(file_url), nbase::UTF16ToUTF8(DirctoryToStorePlayerPortrait()), cb_completed, true, nullptr, "", "");
	}
}

void HeadPortrait::SetHeadPortraitViaFile(const std::wstring& file_path)
{
	file_path_ = file_path;

	this->UpdateHeadPortrait();
}

void HeadPortrait::UpdateHeadPortrait()
{
	auto size = this->size();
	int width = size.width();
	int height = size.height();
	int length = min(width, height);
	int left = (width - length) / 2;
	int top = (height - length) / 2;

	QImage head_image(QString::fromStdWString(file_path_));
	if (head_image.isNull())
	{
		head_image = is_teacher_ ? *s_tea_default_image : *s_stu_default_image;
	}

	QImage canvas(size, QImage::Format_ARGB32_Premultiplied);

	{
		QPainter painter(&canvas);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setCompositionMode(QPainter::CompositionMode_Source);

		painter.fillRect(canvas.rect(), Qt::transparent);

		// draw mask
		if (s_mask_image && !s_mask_image->isNull())
		{
			auto mask_image_scaled = s_mask_image->scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
			
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			painter.drawImage(left, top, mask_image_scaled);
			painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		}

		// draw head portrait
		if (!head_image.isNull())
		{
			auto head_image_scaled = head_image.scaled(length, length, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

			painter.drawImage(left, top, head_image_scaled);
		}

		// draw border
		if (s_border_image && !s_border_image->isNull())
		{
			auto border_image_scaled = s_border_image->scaled(length, length, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			painter.drawImage(left, top, border_image_scaled);
		}

		painter.end();
	}

	auto pixmap = QPixmap::fromImage(canvas);

	portrait_->setPixmap(pixmap);
}

void HeadPortrait::SetIsTeacher(bool is_teacher)
{
	is_teacher_ = is_teacher;
}

GAME_NAMESPACE_END