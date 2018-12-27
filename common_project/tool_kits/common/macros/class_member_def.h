#ifndef __CLASS_MEMBER_DEF_H__
#define __CLASS_MEMBER_DEF_H__

#define D_MEM_GET(type, name, getter) \
public: \
	type getter() const { return name; } \
private: type name

#define D_MEM_GET_BY_REF(type, name, getter) \
public: \
	const type& getter() const { return name; } \
private: type name

#define D_MEM_SET(type, name, setter) \
public: \
	void setter(const type& name##Src) { name = name##Src; } \
private: type name

#define D_MEM_SET_BY_VAL(type, name, setter) \
public: \
	void setter(type name##Src) { name = name##Src; } \
private: type name

#define D_MEM_GET_SET(type, name, getter, setter)  \
public: \
	type getter() const { return name; } \
	void setter(const type& name##Src) { name = name##Src; } \
private: type name

#define D_MEM_GET_BY_REF_SET(type, name, getter, setter) \
public: \
	const type& getter() const { return name; } \
	void setter(const type& name##Src) { name = name##Src; } \
private: type name

#define D_MEM_GET_SET_BY_VAL(type, name, getter, setter)  \
public: \
	type getter() const { return name; } \
	void setter(type name##Src) { name = name##Src; } \
private: type name

#endif