#ifndef __PROTO_BASE_COMMAND_VARIANT_H__
#define __PROTO_BASE_COMMAND_VARIANT_H__
#include <string>
#include <memory.h>
#include <utility>
#include <sstream>
#include "protocol/macros.h"

PROTO_NAMESPACE_BEGIN

struct Variant
{
	enum class Type
	{
		None		= -1,
		Int			= 0,
		LongLong	= 1,
		Float		= 2,
		String		= 3,
	};

	Variant() : type_(Type::None) { this->ZeroData(); }
	Variant(int i) : type_(Type::Int) { d_.i = i; }
	Variant(long long ll) : type_(Type::LongLong) { d_.ll = ll; }
	Variant(float f) : type_(Type::Float) { d_.f = f; }
	Variant(std::string str) : type_(Type::String) { d_.str = new std::string(std::move(str)); }
	~Variant() { if (Type::String == type_) { delete d_.str; } }
	Variant(const Variant& var) : type_(Type::None)  { this->CopyFrom(var); }
	Variant(Variant&& var) : type_(Type::None) { this->MoveFrom(std::move(var)); }
	Variant& operator=(const Variant& var) { this->CopyFrom(var);  return *this; }
	Variant& operator=(Variant&& var) { this->MoveFrom(std::move(var));  return *this; }

	std::string Info() const;

	inline Type			GetType() const { return type_; }

	inline int			ToInt() const { return (Type::Int == type_) ? d_.i : static_cast<int>(d_.ll); }
	inline long	long	ToLongLong() const { return (Type::LongLong == type_) ? d_.ll : d_.i; }
	inline float		ToFloat() const { return (Type::Float == type_) ? d_.f : (float)d_.i; }
	inline std::string	ToString() const { return (Type::String == type_) ? (*d_.str) : std::string(); }

	std::string			PackAsProto() const;
	
private:
	void				CopyFrom(const Variant& var);

	void				MoveFrom(Variant&& var);

	void				TryToDeleteString();

	void				ZeroData();

private:
	Type type_;
	union D
	{
		int				i;
		long long		ll;
		float			f;
		std::string*	str;
		void*			ptr;
	}d_;
};

PROTO_NAMESPACE_END

#endif