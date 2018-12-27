#include "command.h"
#include <sstream>

PROTO_NAMESPACE_BEGIN

Command::Command() = default;

CommandType Command::Type() const
{
	return this->GetClassRuntimeInfo().Type();
}

std::string Command::ClassName() const
{
	return this->GetClassRuntimeInfo().ClassName();
}

int Command::NumberOfVariants() const 
{ 
	return 0; 
}

Variant::Type Command::TypeOfVariantAt(size_t index) const 
{ 
	return Variant::Type::None; 
}

int Command::IntAt(size_t index) const 
{ 
	return 0; 
}

long long Command::LongLongAt(size_t index) const 
{ 
	return 0; 
}

float Command::FloatAt(size_t index) const 
{ 
	return 0.0f;
}

std::string Command::StringAt(size_t index) const 
{ 
	return std::string(); 
}

void Command::PushInt(int i) 
{}

void Command::PushLongLong(long long ll) 
{}

void Command::PushFloat(float f) 
{}

void Command::PushString(std::string str) 
{}

void Command::ForeachVariant(const std::function<void(const Variant&)>& callback) const 
{}

std::string Command::StringOfLog(const std::string& strPrefix) const
{
	std::stringstream ss;
	ss << strPrefix;
	ss << this->ClassName() << "(" << static_cast<int>(Type()) << ")" << " ( " << this->_StringOfLog("") << " )";

	return ss.str();
}

std::string Command::_StringOfLog(const std::string& strPrefix) const
{
	return strPrefix;
}

PROTO_NAMESPACE_END