#ifndef __PROTO_BASE_COMMAND_CELL_H__
#define __PROTO_BASE_COMMAND_CELL_H__
#include <vector>
#include "command.h"

PROTO_NAMESPACE_BEGIN

class CommandCell : public Command
{
	DECLARE_COMMAND_RUNTIME_INFO(CommandCell)

public:
	virtual int				UnpackFromString(const std::string& str, int startIndex) override;
	virtual std::string		PackToString() const override;

	virtual int				NumberOfVariants() const override;
	virtual Variant::Type	TypeOfVariantAt(size_t index) const;

	virtual int				IntAt(size_t index) const override;
	virtual long long		LongLongAt(size_t index) const override;
	virtual float			FloatAt(size_t index) const override;
	virtual std::string		StringAt(size_t index) const override;

	virtual void			PushInt(int i) override;
	virtual void			PushLongLong(long long ll) override;
	virtual void			PushFloat(float f) override;
	virtual void			PushString(std::string str) override;

	virtual void			ForeachVariant(const std::function<void(const Variant&)>& callback) const override;

	virtual bool			EqualTo(const Command& rh) const override;

protected:
	virtual std::string		_StringOfLog(const std::string& strPrefix) const override;

protected:
	std::vector<Variant>	variants_;
};

PROTO_NAMESPACE_END

#endif