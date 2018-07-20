#pragma once
#include <QWidget>
#include<QMouseEvent>
#include <string>
#include "QImage"
#include "common\weak_callback\weak_callback.h"
#include "paint_data.h"
#include <unordered_set>

class PaintImageProxy;

class PaintImage : public QWidget
{
	Q_OBJECT
public:
	PaintImage(PaintBaseInfo info, QWidget *parent = 0);
	~PaintImage();
	QSize GetImageSize();
	void SetBackgroundImage(std::wstring image_path);
	virtual void SetVisibleEx(bool bvisible);
	void SetUrl(std::string url);
	std::string GetImageUrl() const;
	void SetVedioUrl(std::string url);
	std::string GetVedioUrl();
	void SetImageDir(std::wstring image_dir);
	void SetCenter(bool bCenter);
	QImage GetBackgroundImage();
	QImage GetBackgroundImageByForce();
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
	bool IsBkImageVisible() const;
	void GetImageRect(QRect &rect);

private slots:
	void SlotImageDownloaded(std::string url, std::string path);

protected:
	void paintEvent(QPaintEvent *);

private:
	void ShowImage(QImage& loadedImage);
	void PaintOnImage(QImage& loadedImage, QImage& imageToPaintOn);
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

	std::unordered_set<std::string> set_image_url_fetching_;

	std::shared_ptr<PaintImageProxy> proxy_;
};

class PaintImageProxy : public QObject, public wcb::SupportWeakCallback<PaintImageProxy>
{
	Q_OBJECT

public:
	void	DownloadImage(const std::string& url, const std::string& localDir);

signals:
	void	SignalImageDownloaded(std::string url, std::string path);
};