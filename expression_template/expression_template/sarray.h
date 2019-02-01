#ifndef SARRAY_H_INCLUDED
#define SARRAY_H_INCLUDED
#include <assert.h>
#include <stddef.h> // size_t
#include <iostream>

using namespace std;

template<typename T>
class SArray
{
public:
	explicit SArray(size_t size);
	SArray(const SArray& rhs);
	SArray(SArray&& rhs);
	template<typename Arr>
	SArray(const Arr& arr);
	~SArray();

	SArray& operator=(const SArray& rhs);
	SArray& operator=(SArray&& rhs);

	const T& operator[](size_t index) const;
	T& operator[](size_t index);

	size_t size() const;

	T* begin();
	const T* cbegin() const;

	T* end();
	const T* cend() const;

	template<typename U>
	friend SArray<U> operator+(const SArray<U>& lhs, const SArray<U>& rhs);
	template<typename U>
	friend SArray<U> operator-(const SArray<U>& lhs, const SArray<U>& rhs);
	template<typename U>
	friend SArray<U> operator*(const SArray<U>& lhs, const SArray<U>& rhs);
	template<typename U>
	friend SArray<U> operator*(const U t, const SArray<U>& arr);
	template<typename U>
	friend SArray<U> operator*(const SArray<U>& arr, const U t);

private:
	T* storage_;
	size_t size_;
};

template<typename T>
SArray<T>::SArray(size_t size) : storage_(new T[size]), size_(size) {}

template<typename T>
SArray<T>::SArray(const SArray& rhs) : storage_(new T[rhs.size_]), size_(rhs.size_)
{
	for (size_t i = 0; i < size_; ++i)
	{
		storage_[i] = rhs.storage_[i];
	}
}

template<typename T>
SArray<T>::SArray(SArray&& rhs) : storage_(rhs.storage_), size_(rhs.size_)
{
	rhs.storage_ = nullptr;
	rhs.size_ = 0;
}

template<typename T>
template<typename Arr>
SArray<T>::SArray(const Arr& arr) : storage_(new T[arr.size()]), size_(arr.size())
{
	for (size_t i = 0; i < size_; ++i)
	{
		storage_[i] = arr[i];
	}
}

template<typename T>
SArray<T>::~SArray()
{
	delete[] storage_;

#ifdef _DEBUG
	std::cout << "SArray<T>::~SArray()" << std::endl;
#endif
}

template<typename T>
SArray<T>& SArray<T>::operator=(const SArray& rhs)
{
	if (&rhs != this)
	{
		assert(size_ == rhs.size_);

		for (size_t i = 0; i < size_; ++i)
		{
			storage_[i] = rhs.storage_[i];
		}
	}

	return (*this);
}

template<typename T>
SArray<T>& SArray<T>::operator=(SArray&& rhs)
{
	if (&rhs != this)
	{
		assert(size_ == rhs.size_);

		for (size_t i = 0; i < size_; ++i)
		{
			storage_[i] = rhs.storage_[i];
		}

		delete[] rhs.storage_;
		rhs.storage_ = nullptr;
		rhs.size_ = 0;
	}

	return (*this);
}

template<typename T>
const T& SArray<T>::operator[](size_t index) const
{
	assert(index < size_);

	return storage_[index];
}

template<typename T>
T& SArray<T>::operator[](size_t index)
{
	assert(index < size_);

	return storage_[index];
}

template<typename T>
size_t SArray<T>::size() const
{ 
	return size_; 
}

template<typename T>
T* SArray<T>::begin()
{
	return storage_;
}

template<typename T>
const T* SArray<T>::cbegin() const
{
	return storage_;
}

template<typename T>
T* SArray<T>::end()
{
	return storage_ + size_;
}

template<typename T>
const T* SArray<T>::cend() const
{
	return storage_ + size_;
}

template<typename U>
SArray<U> operator+(const SArray<U>& lhs, const SArray<U>& rhs)
{
	assert(lhs.size_ == rhs.size_);

	SArray<U> out(lhs.size_);
	for (size_t i = 0; i < lhs.size_; ++i)
	{
		out[i] = lhs[i] + rhs[i];
	}

	return out;
}

template<typename U>
SArray<U> operator-(const SArray<U>& lhs, const SArray<U>& rhs)
{
	assert(lhs.size_ == rhs.size_);

	SArray<U> out(lhs.size_);
	for (size_t i = 0; i < lhs.size_; ++i)
	{
		out[i] = lhs[i] - rhs[i];
	}

	return out;
}

template<typename U>
SArray<U> operator*(const SArray<U>& lhs, const SArray<U>& rhs)
{
	assert(lhs.size_ == rhs.size_);

	SArray<U> out(lhs.size_);
	for (size_t i = 0; i < lhs.size_; ++i)
	{
		out[i] = lhs[i] * rhs[i];
	}

	return out;
}

template<typename U>
SArray<U> operator*(const U t, const SArray<U>& arr)
{
	SArray<U> out(arr.size_);
	for (size_t i = 0; i < arr.size_; ++i)
	{
		out[i] = t * arr[i];
	}

	return out;
}

template<typename U>
SArray<U> operator*(const SArray<U>& arr, const U t)
{
	return t * arr;
}

#endif