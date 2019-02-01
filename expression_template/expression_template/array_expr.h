#ifndef ARRAY_EXPR_H_INCLUDED
#define ARRAY_EXPR_H_INCLUDED
#include <type_traits>
#include "sarray.h"

// array with default delegate/proxy
template<typename T, typename Rep = SArray<T>>
class Array
{
public:
	explicit Array(size_t size) : rep_(size) {}

	Array(const Rep& rep) : rep_(rep) {}

	Array& operator=(const Array& rhs)
	{
		assert(size() < rhs.size());

		for (size_t i = 0; i < size(); ++i)
		{
			(*this)[i] = rhs[i];
		}

		return (*this);
	}

	template<typename T2, typename Rep2>
	Array& operator=(const Array<T2, Rep2>& rhs)
	{
		assert(size() < rhs.size());

		for (size_t i = 0; i < size(); ++i)
		{
			(*this)[i] = rhs[i];
		}

		return (*this);
	}

	//decltype(auto) operator[](size_t index) const
	//{
	//	assert(index < rep_.size());

	//	return rep_[index];
	//}

	T operator[](size_t index) const
	{
		assert(index < rep_.size());

		return rep_[index];
	}

	T& operator[](size_t index)
	{ 
		assert(index < rep_.size());

		return rep_[index];
	}

	size_t size() const { return rep_.size(); }

	const Rep& rep() const { return rep_; }
	Rep& rep() { return rep_; }

private:
	Rep rep_;
};

// scalar
template<typename T>
class A_Scalar
{
public:
	A_Scalar(const T& s) : s_(s) {}

	const T& operator[](size_t index) const { return s_; }

	size_t size() const { return 0; }

private:
	const T& s_;
};

// original traits for const reference
template<typename T>
struct A_Traits
{
	typedef const T& ExprRef;
};

// partial specialization for scalars
template<typename T>
struct A_Traits<A_Scalar<T>>
{
	typedef A_Scalar<T> ExprRef;
};

// operand holder
template<typename T, typename OP1, typename OP2>
class A_OpHolder
{
public:
	A_OpHolder(const OP1& op1, const OP2& op2) : op1_(op1), op2_(op2) {}

	size_t size() const
	{
		assert(op1_.size() == 0 || op2_.size() == 0 || op1_.size() == op2_.size());

		return op1_.size() != 0 ? op1_.size() : op2_.size();
	}

protected:
	typename A_Traits<OP1>::ExprRef op1_;
	typename A_Traits<OP2>::ExprRef op2_;
};

// add
template<typename T, typename OP1, typename OP2>
class A_Add : public A_OpHolder<T, OP1, OP2>
{
public:
	A_Add(const OP1& op1, const OP2& op2) : A_OpHolder(op1, op2) {}

	T operator[](size_t index) const
	{
		assert(index < size());

		return op1_[index] + op2_[index];
	}
};

// subtract
template<typename T, typename OP1, typename OP2>
class A_Sub : public A_OpHolder<T, OP1, OP2>
{
public:
	A_Sub(const OP1& op1, const OP2& op2) : A_OpHolder(op1, op2) {}

	T operator[](size_t index) const
	{
		assert(index < size());

		return op1_[index] - op2_[index];
	}
};

// multiply
template<typename T, typename OP1, typename OP2>
class A_Mult : public A_OpHolder<T, OP1, OP2>
{
public:
	A_Mult(const OP1& op1, const OP2& op2) : A_OpHolder(op1, op2) {}

	T operator[](size_t index) const
	{
		assert(index < size());

		return op1_[index] * op2_[index];
	}
};

// operator + between Array and Array
template<typename T, typename OP1, typename OP2>
Array<T, A_Add<T, OP1, OP2>> operator+(const Array<T, OP1>& op1, const Array<T, OP2>& op2)
{
	return Array<T, A_Add<T, OP1, OP2>>(A_Add<T, OP1, OP2>(op1.rep(), op2.rep()));
}

// operator - between Array and Array
template<typename T, typename OP1, typename OP2>
Array<T, A_Sub<T, OP1, OP2>> operator-(const Array<T, OP1>& op1, const Array<T, OP2>& op2)
{
	return Array<T, A_Sub<T, OP1, OP2>>(A_Sub<T, OP1, OP2>(op1.rep(), op2.rep()));
}

// operator * between Array and Array
template<typename T, typename OP1, typename OP2>
Array<T, A_Mult<T, OP1, OP2>> operator*(const Array<T, OP1>& op1, const Array<T, OP2>& op2)
{
	return Array<T, A_Mult<T, OP1, OP2>>(A_Mult<T, OP1, OP2>(op1.rep(), op2.rep()));
}

// operator * between Scalar and Array
template<typename T, typename OP2>
Array<T, A_Mult<T, A_Scalar<T>, OP2>> operator*(const T& s, const Array<T, OP2>& op2)
{
	return Array<T, A_Mult<T, A_Scalar<T>, OP2>>(A_Mult<T, A_Scalar<T>, OP2>(A_Scalar<T>(s), op2.rep()));
}

// operator * between Array and Scalar
template<typename T, typename OP2>
Array<T, A_Mult<T, OP2, A_Scalar<T>>> operator*(const Array<T, OP2>& op2, const T& s)
{
	return Array<T, A_Mult<T, OP2, A_Scalar<T>>>(A_Mult<T, OP2, A_Scalar<T>>(op2.rep(), A_Scalar<T>(s)));
}

#endif