#ifndef __RUNTIME_CLASS_INFO_H__
#define __RUNTIME_CLASS_INFO_H__
#include <string>

template<typename Class, typename Creator>
struct ClassRuntimeInfo
{
	ClassRuntimeInfo(const std::string& className, Creator creator);
	ClassRuntimeInfo(const ClassRuntimeInfo&) = delete;

	std::string					ClassName() const { return _className; }

	template<typename... Args>
	auto						Create(Args&&... args) const ->decltype(_creator(std::forward<Args>(args)...)) { return _creator ? _creator(std::forward<Args>(args)...) : decltype(_creator(std::forward<Args>(args)...))(); }
	
	ClassRuntimeInfo*			Next() const { return _next; }

private:
	std::string					_className;
	Creator						_creator;
	ClassRuntimeInfo*			_next;
};

template<typename Class, typename TypeClass, typename Creator>
struct ClassRuntimeInfoWithType
{
	ClassRuntimeInfoWithType(TypeClass type, const std::string& className, Creator creator);
	ClassRuntimeInfoWithType(const ClassRuntimeInfoWithType&) = delete;
	
	TypeClass					Type() const { return _type; }
	std::string					ClassName() const { return _className; }

	template<typename... Args>
	auto						Create(Args&&... args) const ->decltype(_creator(std::forward<Args>(args)...)) { return _creator ? _creator(std::forward<Args>(args)...) : decltype(_creator(std::forward<Args>(args)...))(); }
	
	ClassRuntimeInfoWithType*	Next() const { return _next; }

private:
	TypeClass					_type;
	std::string					_className;
	Creator						_creator;
	ClassRuntimeInfoWithType*	_next;
};
#endif