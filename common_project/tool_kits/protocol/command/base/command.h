#ifndef __PROTO_BASE_COMMAND_H__
#define __PROTO_BASE_COMMAND_H__
#include <stddef.h> // for size_t
#include <string>
#include <functional>
#include "base/memory/objectcounter.h"
#include "command_variant.h"
#include "command_class_runtime_info.h"
#include "command_fwd.h"
#include "../command_types.h"

PROTO_NAMESPACE_BEGIN

class Command
#ifdef PROTO_TEST_MEMORY_LEAK
	: public nbase::ObjectCounter<Command>
#endif
{
public:
	virtual ~Command() = default;
	virtual const CommandClassRuntimeInfo&	GetClassRuntimeInfo() const = 0;

	CommandType								Type() const;
	std::string								ClassName() const;

	virtual int								UnpackFromString(const std::string& str, int startIndex) = 0;
	virtual std::string						PackToString() const = 0;

	virtual int								NumberOfVariants() const;
	virtual Variant::Type					TypeOfVariantAt(size_t index) const;

	virtual int								IntAt(size_t index) const;
	virtual long long						LongLongAt(size_t index) const;
	virtual float							FloatAt(size_t index) const;
	virtual std::string						StringAt(size_t index) const;

	template<typename... Args>
	void									PushVariants(Args&&... args);

	virtual void							PushInt(int i);
	virtual void							PushLongLong(long long ll);
	virtual void							PushFloat(float f);
	virtual void							PushString(std::string str);

	virtual void							ForeachVariant(const std::function<void(const Variant&)>& callback) const;

	virtual bool							EqualTo(const Command& rh) const = 0;

	std::string								StringOfLog(const std::string& strPrefix) const;

protected:
	Command();
	Command(const Command&) = delete;
	Command& operator=(const Command&) = delete;

	virtual std::string						_StringOfLog(const std::string& strPrefix) const;

	template<typename HeadArg, typename... TailArgs>
	void									_PushVariants(HeadArg&& head, TailArgs&&... tail);
	void									_PushVariants() {} // end of the recursive above
	inline void								_PushVariant(int i) { this->PushInt(i); }
	inline void								_PushVariant(long long ll) { this->PushLongLong(ll); }
	inline void								_PushVariant(float f) { this->PushFloat(f); }
	inline void								_PushVariant(std::string str) { this->PushString(std::move(str)); }

	template<typename OStream>
	friend OStream&							operator<<(OStream& stream, const Command& cmd);
};

template<typename... Args>
void Command::PushVariants(Args&&... args)
{
	this->_PushVariants(std::forward<Args>(args)...);
}

template<typename HeadArg, typename... TailArgs>
void Command::_PushVariants(HeadArg&& head, TailArgs&&... tail)
{
	this->_PushVariant(std::forward<HeadArg>(head));

	this->_PushVariants(std::forward<TailArgs>(tail)...);
}

template<typename OStream>
OStream& operator<<(OStream& stream, const Command& cmd)
{
	stream << cmd.StringOfLog("");

	return stream;
}

PROTO_NAMESPACE_END

#endif