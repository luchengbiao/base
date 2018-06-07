#include "image_view_manager.h"
#include <algorithm>
#include "QApplication"
#include "base\file\file_util.h"
#include "QDesktopWidget"
using namespace std;
using namespace nbase;


ImageViewManager::ImageViewManager() : message_id_(L""), m_curr_show_index_(0)
{

}

ImageViewManager::~ImageViewManager()
{
	
}

bool ImageViewManager::StartViewPicList(ImageViewInfoList info_list, int m_from_index_)
{
	if (info_list.empty())
	{
		return false;
	}
	m_from_index_ = min(m_from_index_, info_list.size() - 1);
	image_view_info_list_ = info_list;
	m_curr_show_index_ = m_from_index_;
	ImageViewForm* image_view_ = StartViewPic(info_list[m_from_index_].path_, L"", true, false);
	if (image_view_)
	{
		image_view_->SetNextOrPreImageBtnVisible(info_list.size() > 1);
		return true;
	}
	return false;
}

bool ImageViewManager::StartViewSinglePic(const std::wstring &path_pic, bool size)
{
	ImageViewInfoList p_list;
	JYXB_IMAGEINFO p;
	p.m_index_ = 0;
	p.path_ = path_pic;

	p_list.push_back(p);
	return StartViewPicList(p_list, 0);
}

ImageViewForm* ImageViewManager::StartViewPic(const std::wstring &path_pic, std::wstring message_id, bool size, bool nosave)
{
	ImageViewForm* image_view = ImageViewForm::GetSelfWindow();
	if (!image_view)
	{
		image_view = new ImageViewForm();
		image_view->Create();
	}
	message_id_ = message_id;
	image_view->StartViewPic(path_pic, size, nosave);
	image_view->show();

	return image_view;
}

bool ImageViewManager::ShowNextImageView(bool next)
{
	if (image_view_info_list_.size() <=0)
	{
		return false;
	}
	if (next)
	{
		m_curr_show_index_ = min(m_curr_show_index_ + 1, image_view_info_list_.size()-1);
	}
	else
	{
		m_curr_show_index_ = max(0, m_curr_show_index_ - 1);
	}

	if (image_view_info_list_.size() > m_curr_show_index_)
	{
		ImageViewInfo info = image_view_info_list_.at(m_curr_show_index_);
		return StartViewPic(info.path_, L"", true, false);
	}

	return false;
}

bool ImageViewManager::IsExistNextImage(bool next)
{
	for (int i = m_curr_show_index_; i >= 0 && i < image_view_info_list_.size();)
	{
		ImageViewInfo info = image_view_info_list_.at(i);
		
		if (next) 
		{
			i++;
		}
		else
		{
			i--;
		}

		if (i >= 0 && i < image_view_info_list_.size())
		{
			return true;
		}
	}
	return false;
}

void ImageViewManager::GetImagePage(long& cur, long& sum)
{
	cur = m_curr_show_index_+1;
	sum = image_view_info_list_.size();
}