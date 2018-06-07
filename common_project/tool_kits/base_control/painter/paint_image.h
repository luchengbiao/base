#pragma once
#include <QWidget>
#include<QMouseEvent>
#include <string>
#include "QImage"
#include "base\callback\callback.h"
#include "paint_data.h"

class PaintImage : public QWidget, public nbase::SupportWeakCallback
{
	Q_OBJECT
public:
	PaintImage(PaintBaseInfo info, QWidget *parent = 0);
	~PaintImage();
	QSize GetImageSize();
	void SetBackgroundImage(std::wstring image_path);
	virtual void SetVisibleEx(bool bvisible);
	void SetUrl(std::string url);
	void SetVedioUrl(std::string url);
	std::string GetVedioUrl();
	void SetImageDir(std::wstring image_dir);
	void SetCenter(bool bCenter);
	QImage GetBackgroundImage();
	void SetImageAngle(int angle);
	void RotateImage();
	int GetAngle();
	void ResizeImage(int width, int height);
	void SetText(std::wstring tooltip);
	std::wstring GetLocalImage();
	bool IsBackgroundModified();
	void SetDownloadImageCb(DownloadBoardImageCb cb);
	void SetMarkText(std::wstring text);
	void SetShowBkImage(bool b_show);
	void GetImageRect(QRect &rect);

signals:
	void SignalDownloadFileEnd(std::wstring);

private slots:
	void SlotSetBackImage(std::wstring path);

protected:
	void paintEvent(QPaintEvent *);

private:
	void ShowImage(QImage& loadedImage);
	bool CheckImageQuality(std::wstring& file_path, QImage& image);

private:
	QSize image_size_;
	QImage image_;
	bool b_loaded_image_;
	std::wstring tooltip_;
	std::wstring origin_image_path_;
	std::string image_url_;
	std::wstring image_dir_;
	std::string vedio_url_;
	bool bCenter_;
	bool bVisible_;
	bool b_show_bk_;  // «∑Òœ‘ æ±≥æ∞≤„
	int angle_;
	bool b_modify_;
	DownloadBoardImageCb download_image_cb_;
	std::wstring mark_text_;

	QRect img_rect_;
};