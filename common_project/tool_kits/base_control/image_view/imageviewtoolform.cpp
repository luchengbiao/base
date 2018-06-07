#include "imageviewtoolform.h"
#include "ui_imageviewtoolform.h"
#include "base\file\file_util.h"
#include "imageviewform.h"
#include "base\macros.h"

ImageViewToolBox::ImageViewToolBox(QWidget *parent) : QWidget(parent), ui(new Ui::imageViewToolForm)
{
    ui->setupUi(this);
	this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
	this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口

	ModifyUI();
}

ImageViewToolBox::~ImageViewToolBox()
{
	SAFE_DELETE(ui);
}

void ImageViewToolBox::ModifyUI()
{
	SetCursor();
	ui->frame->setFixedSize(384, 66);
	
	ui->frame->setStyleSheet(QString("QFrame#frame{border-image:url(res/images/image_view/toolbar_bk.png)20 20 20 20;"
		"border-top: 20px transparent;"
		"border-bottom: 20px transparent;"
		"border-right: 20px transparent;"
		"border-left: 20px transparent;}"));

	ui->btn_normal->setStyleSheet("QToolButton{border-image: url(res/images/image_view/normal_1.png);}"
		"QToolButton:hover{border-image: url(res/images/image_view/normal_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/normal_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/normal_4.png);}");
	ui->btn_normal->setFixedSize(48,48);

	ui->btn_max->setStyleSheet("QToolButton{border-image: url(res/images/image_view/max_1.png);}"
		"QToolButton:hover{border-image: url(res/images/image_view/max_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/max_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/max_4.png);}");
	ui->btn_max->setFixedSize(48,48);
	ui->btn_max->setVisible(false);

	ui->btn_restore->setStyleSheet("QToolButton{border-image: url(res/images/image_view/restore_1.png);}"
		"QToolButton:hover{border-image: url(res/images/image_view/restore_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/restore_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/restore_4.png);}");
	ui->btn_restore->setFixedSize(48,48);

	ui->btn_magnify->setStyleSheet("QToolButton{border-image: url(res/images/image_view/magnify_1.png);}"
		"QToolButton:hover{border-image: url(res/images/image_view/magnify_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/magnify_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/magnify_4.png);}");
	ui->btn_magnify->setFixedSize(48,48);

	ui->btn_reduce->setStyleSheet("QToolButton{border-image: url(res/images/image_view/reduce_1.png);}"
		"QToolButton:hover{border-image: url(res/images/image_view/reduce_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/reduce_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/reduce_4.png);}");
	ui->btn_reduce->setFixedSize(48,48);

	ui->btn_rotate->setStyleSheet("QToolButton{border-image: url(res/images/image_view/rotate_1.png);}"
		"QToolButton:hover{border-image: url(res/images/image_view/rotate_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/rotate_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/rotate_4.png);}");
	ui->btn_rotate->setFixedSize(48,48);

	ui->btn_save->setStyleSheet("QToolButton{border-image: url(res/images/image_view/save_1.png)}"
		"QToolButton:hover{border-image: url(res/images/image_view/save_2.png);}"
		"QToolButton:pressed{border-image: url(res/images/image_view/save_3.png);}"
		"QToolButton:disabled{border-image: url(res/images/image_view/save_4.png);}");
	ui->btn_save->setFixedSize(48,48);

	connect(ui->btn_normal, SIGNAL(clicked()), this, SLOT(OnImageNormal()));
	connect(ui->btn_max, SIGNAL(clicked()), this, SLOT(OnImageMax()));
	connect(ui->btn_restore, SIGNAL(clicked()), this, SLOT(OnImageRestore()));
	connect(ui->btn_magnify, SIGNAL(clicked()), this, SLOT(OnImageMagnify()));
	connect(ui->btn_reduce, SIGNAL(clicked()), this, SLOT(OnImageReduce()));
	connect(ui->btn_rotate, SIGNAL(clicked()), this, SLOT(OnImageRotate()));
	connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(OnImageSave()));
}

void ImageViewToolBox::OnImageNormal()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnNormalImage();
	}
}

void ImageViewToolBox::OnImageMax()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnMaxWindow();
	}
}

void ImageViewToolBox::OnImageRestore()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnRestoredWindow();
	}
}

void ImageViewToolBox::OnImageMagnify()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnScaleImage(ImageZoom::kMagnifyImage);
	}
}

void ImageViewToolBox::OnImageReduce()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnScaleImage(ImageZoom::kReduceImage);
	}
}

void ImageViewToolBox::OnImageRotate()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnRotateImage();
	}
}

void ImageViewToolBox::OnImageSave()
{
	ImageViewForm* form = ImageViewForm::GetSelfWindow();
	if (form)
	{
		form->OnSaveImage();
	}
}

void ImageViewToolBox::SetBtnNormal(bool bvisible, bool benable)
{
	ui->btn_normal->setVisible(bvisible);
	ui->btn_normal->setEnabled(benable);
}

void ImageViewToolBox::SetBtnMax(bool bvisible, bool benable)
{
	ui->btn_max->setVisible(bvisible);
	ui->btn_max->setEnabled(benable);
}

void ImageViewToolBox::SetBtnRestore(bool bvisible, bool benable)
{
	ui->btn_restore->setVisible(bvisible);
	ui->btn_restore->setEnabled(benable);
}

void ImageViewToolBox::SetBtnMagnify(bool bvisible, bool benable)
{
	ui->btn_magnify->setVisible(bvisible);
	ui->btn_magnify->setEnabled(benable);
}

void ImageViewToolBox::SetBtnReduce(bool bvisible, bool benable)
{
	ui->btn_reduce->setVisible(bvisible);
	ui->btn_reduce->setEnabled(benable);
}

void ImageViewToolBox::SetBtnRotate(bool bvisible, bool benable)
{
	ui->btn_rotate->setVisible(bvisible);
	ui->btn_rotate->setEnabled(benable);
}

void ImageViewToolBox::SetBtnSave(bool bvisible, bool benable)
{
	ui->btn_save->setVisible(bvisible);
	ui->btn_save->setEnabled(benable);
}

void ImageViewToolBox::SetCursor()
{

	ui->btn_magnify->setCursor(Qt::PointingHandCursor);
	ui->btn_normal->setCursor(Qt::PointingHandCursor);
	ui->btn_save->setCursor(Qt::PointingHandCursor);
	ui->btn_rotate->setCursor(Qt::PointingHandCursor);
	ui->btn_reduce->setCursor(Qt::PointingHandCursor);
	ui->btn_max->setCursor(Qt::PointingHandCursor);
	ui->btn_restore->setCursor(Qt::PointingHandCursor);
}
