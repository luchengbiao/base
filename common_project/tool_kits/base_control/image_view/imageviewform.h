#ifndef IMAGEVIEWFORM_H
#define IMAGEVIEWFORM_H

#include <QWidget>
#include <string>
#include "imageviewtoolform.h"
#include "QBoxLayout"
#include "QLabel"
#include "QPoint"
#include "qevent.h"
#include "base_control\common\base\basewindow.h"

namespace Ui {
class imageViewForm;
}

enum ImageDirectionOfRotation
{
	kClockWise = 0,		//˳ʱ��
	kAntiClockWise		//��ʱ��
};

enum ImageZoom
{
	kMagnifyImage = 0,	//�Ŵ�
	kReduceImage		//��С
};

class ImageViewForm : public BaseWindow
{
    Q_OBJECT

public:
	ImageViewForm(QWidget *parent = 0);
	~ImageViewForm();

	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual void InitWindow() override;

	void SetNextOrPreImageBtnVisible(bool show);
	bool StartViewPic(std::wstring path_pic, bool size, bool nosave);

	void OnNormalImage();
	void OnMaxWindow();
	void OnRestoredWindow();
	void OnScaleImage(ImageZoom zoom);
	void OnRotateImage();
	void OnSaveImage();

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *event);

private:
	bool GetDefaultWindowSize(int &wnd_width, int &wnd_height, int &bg_corner, float& scale);
	float GetDefaultScale(int control_width, int control_height, int image_width, int image_height);
	void SetWindowSize(int width, int height);
	void CheckNextOrPreImageBtnStatus();
	void UpdateImagePage();
	void AdjustToolbarStatus();
	void SetScale(float scale);
	bool IsScale(float scale, int& iwidth, int& iheight);
	void ShowImageScale(bool show);
	bool IsImageMove(QPoint point);
	void SetCursor();

private slots:
	void OnCloseWindow();
	void OnPreImage();
	void OnNextImage();
	void OnHideScaleHite();

public:
	static const std::wstring kClassName;
	static ImageViewForm* GetSelfWindow();

private:
    Ui::imageViewForm *ui;
	ImageViewToolBox* toolbox_;
	QHBoxLayout *toolLayout_;
	QImage m_image_;
	QImage* imgScaled_;
	float m_scale_;
	bool b_max_window_;
	int m_angle_;
	bool pic_moving_;			//�ж��Ƿ��ƶ�ͼƬ:true ��ʾ�ƶ�ͼƬ
	QPoint point_off_;			//��¼���仯֮ǰ��λ��
	bool show_scale_;			//���µ�ͼƬʱ�Ƿ�Ҫ����ʾһ�µ�ǰ�����ű���	
	std::wstring image_path_;	//���ڲ鿴��ͼƬ��·��
	bool show_nextorpre_btn_;	//�Ƿ���ʾ��һ��/��һ����ť
	bool b_pressed_;
	QString default_path_;
};

#endif // IMAGEVIEWFORM_H
