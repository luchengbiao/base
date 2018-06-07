#ifndef IMAGECLIPFORM_H
#define IMAGECLIPFORM_H

#include <QWidget>
#include "QImage"
#include "QPoint"
#include <functional>
#include <string>
#include "base_control\common\base\basewindow.h"
typedef std::function<void(std::wstring)> image_clip_callback;

namespace Ui {
class ImageClipForm;
}

class ImageClipWindow : public BaseWindow
{
    Q_OBJECT

public:
	explicit ImageClipWindow(QWidget *parent = 0);
	~ImageClipWindow();

	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual void InitWindow() override;

	bool StartViewPic(std::wstring oriPath, std::wstring destPath, const image_clip_callback& cb);

private:
	bool GetDefaultWindowSize(int &wnd_width, int &wnd_height);
	float GetDefaultScale(int control_width, int control_height, int image_width, int image_height);
	void SetWindowSize(int width, int height);
	bool IsClipMove(QPoint point);

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

public:
	static const std::wstring kClassName;
	static ImageClipWindow* GetSelfWindow();
	static void ShowSelfWindow(std::wstring oriPath, std::wstring destPath, const image_clip_callback& cb);

private slots:
	void OnCloseWindow();
	void OnRotateImage();
	void OnUploadImage();

private:
    Ui::ImageClipForm *ui;
	QImage m_image_;
	QImage imgScaled_;
	int hit_flag_;
	QPoint point_off_;
	bool b_clip_moving_;
	int m_angle_;
	image_clip_callback clip_cb_;
	std::wstring s_clip_image_path_;
	bool b_pressed_;
};

#endif // IMAGECLIPFORM_H
