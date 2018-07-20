#ifndef __LOG_CUSTOM_ITEM_H__
#define __LOG_CUSTOM_ITEM_H__
#include <stdint.h> // for int int64_t
#include <string>

struct CustomLogItem
{
	std::string		log_type_;
	std::string		content_;
	std::string		source_;
	int				biz_id_;
	int64_t			course_id_;
	std::string		file_;
	std::string		log_urls_;

	CustomLogItem()
		: biz_id_(0)
		, course_id_(0)
	{}
};

#endif