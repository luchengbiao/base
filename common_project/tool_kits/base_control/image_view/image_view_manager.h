#ifndef IMAGE_VIEW_MANAGER_H__
#define IMAGE_VIEW_MANAGER_H__

#include "imageviewform.h"
#include <stdint.h>
#include <string>
#include <vector>
#include "base\memory\singleton.h"


class ImageViewManager
{
public:
	SINGLETON_DEFINE(ImageViewManager);

	struct ImageViewInfo
	{
		int		m_index_;
		uint64_t id_;
		std::wstring url_;
		std::wstring path_;
		bool	b_stu_;
		ImageViewInfo() :b_stu_(false), m_index_(0), id_(0)
		{}
	};
	typedef std::vector<ImageViewInfo> ImageViewInfoList;

	ImageViewManager();
	~ImageViewManager();

	ImageViewForm* StartViewPic(const std::wstring &path_pic, std::wstring message_id, bool size, bool nosave = false);

	//显示下一张/上一张图片
	bool ShowNextImageView(bool next);
	//判断是否存在下一张/上一张图片
	bool IsExistNextImage(bool next);
	void GetImagePage(long& cur, long& sum);

	bool StartViewPicList(ImageViewInfoList info_list, int m_from_index_ = 0);
	bool StartViewSinglePic(const std::wstring &path_pic, bool size = true);
private:
	std::wstring   message_id_;
	ImageViewInfoList image_view_info_list_;
	int				m_curr_show_index_;
};

typedef  ImageViewManager::ImageViewInfo JYXB_IMAGEINFO;
#endif // IMAGE_VIEW_MANAGER_H__