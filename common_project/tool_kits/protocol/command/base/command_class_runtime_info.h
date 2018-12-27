#ifndef __PROTO_BASE_COMMAND_CLASS_RUNTIME_INFO_H__
#define __PROTO_BASE_COMMAND_CLASS_RUNTIME_INFO_H__
#include <memory>
#include "common\class_runtime_info\class_runtime_info.h"
#include "..\command_types.h"

namespace proto { class Command; }

using PFuncCommandCreator = std::unique_ptr<proto::Command>(*)();
using CommandClassRuntimeInfo = ClassRuntimeInfoWithType<proto::Command, proto::CommandType, PFuncCommandCreator>;

#define DECLARE_COMMAND_RUNTIME_INFO(className) \
public: \
	static std::unique_ptr<className> Create(); \
	template<typename... Args> \
	static std::unique_ptr<className> CreateWithVariants(Args&&... args) \
{ auto ptr = className::Create(); ptr->PushVariants(std::forward<Args>(args)...);  return ptr; } \
	static std::unique_ptr<proto::Command> CreateHomogeneously(); \
	static const CommandClassRuntimeInfo& RegisterClassRuntimeInfo(); \
	virtual const CommandClassRuntimeInfo& GetClassRuntimeInfo() const override; \
protected: \
	className();

#define IMPLEMENT_COMMAND_RUNTIME_INFO_WTTHOUT_CTOR(type, className, bassClassName) \
	std::unique_ptr<className> className::Create() \
{ return std::unique_ptr<className>(new className); } \
	std::unique_ptr<proto::Command> className::CreateHomogeneously() \
{ return className::Create(); } \
	const CommandClassRuntimeInfo& className::RegisterClassRuntimeInfo() \
{ static CommandClassRuntimeInfo className##ClassRuntimeInfo(type, #className, (PFuncCommandCreator)(&className::Create)); return className##ClassRuntimeInfo; } \
	const CommandClassRuntimeInfo& className::GetClassRuntimeInfo() const { return className::RegisterClassRuntimeInfo(); } \

#define IMPLEMENT_COMMAND_RUNTIME_INFO(type, className, bassClassName) \
	className::className() = default; \
	IMPLEMENT_COMMAND_RUNTIME_INFO_WTTHOUT_CTOR(type, className, bassClassName) \

#define REGISTER_COMMAND_RUNTIME_INFO(className)  className::RegisterClassRuntimeInfo()

#endif