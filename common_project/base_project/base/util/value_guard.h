#ifndef BASE_UTIL_VALUE_GUARD_H_
#define BASE_UTIL_VALUE_GUARD_H_

namespace nbase
{
	template<typename ValueT>
	struct ValueGuard
	{
		ValueGuard(ValueT& src) : src_(src), cpy_(src) {}
		~ValueGuard() { src_ = cpy_; }

	private:
		ValueT& src_;
		ValueT	cpy_;
	};

}  // namespace nbase

#endif  // BASE_UTIL_VALUE_GUARD_H_