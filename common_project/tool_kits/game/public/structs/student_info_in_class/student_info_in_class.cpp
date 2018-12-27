#include "student_info_in_class.h"

GAME_NAMESPACE_BEGIN

StuInfoInClass::StuInfoInClass()
{
	client_ = ClientType::Student;
}

void StuInfoInClass::Add(const std::string&	account_id, const std::string& name, const std::string& portrait_url)
{
	added_ = true;
	account_id_ = account_id;
	name_ = name;
	portrait_url_ = portrait_url;
}

void StuInfoInClass::Reset()
{
	added_ = false;
	client_ = ClientType::Student;
	account_id_.clear();
	name_.clear();
	portrait_url_.clear();
}

void BothStuInfoInClass::Reset()
{
	black_side_.Reset();
	white_side_.Reset();
}

BothStuInfoInClass BothStuInClass;

GAME_NAMESPACE_END