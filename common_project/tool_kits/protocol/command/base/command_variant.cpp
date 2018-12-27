#include "command_variant.h"
#include "common/macros/assert.h"
#include "protocol/utils/byte_parser.h"

PROTO_NAMESPACE_BEGIN

std::string Variant::Info() const
{
	std::stringstream ss;

	if (Type::Int == type_)
	{
		ss << "Int(" << ToInt();
	}
	else if (Type::LongLong == type_)
	{
		ss << "LongLong(" << ToLongLong();
	}
	else if (Type::Float == type_)
	{
		ss << "Float(" << ToFloat();
	}
	else if (Type::String == type_)
	{
		ss << "String(" << ToString();
	}

	ss << ")";

	return ss.str();
}

void Variant::CopyFrom(const Variant& var)
{
	if (this != &var)
	{
		this->TryToDeleteString();

		type_ = var.type_;

		if (Type::String == type_)
		{
			d_.str = new std::string(*var.d_.str);
		}
		else
		{
			memcpy(&d_, &(var.d_), sizeof(D));
		}
	}
}

void Variant::MoveFrom(Variant&& var)
{
	if (this != &var)
	{
		this->TryToDeleteString();

		type_ = var.type_;

		if (Type::String == type_)
		{
			d_.str = var.d_.str;
		}
		else
		{
			memcpy(&d_, &(var.d_), sizeof(D));
		}

		var.type_ = Type::None;
		var.ZeroData();
	}
}

void Variant::TryToDeleteString()
{
	if (Type::String == type_)
	{
		delete d_.str;
		d_.str = nullptr;
	}
}

void Variant::ZeroData()
{
	memset(&d_, 0, sizeof(D));
}

std::string Variant::PackAsProto() const
{
	std::string str_proto;
	str_proto.push_back(static_cast<char>(type_));

	switch (type_)
	{
	case Variant::Type::Int:
		str_proto.append(byte_parser::Int2Bytes(ToInt()));
		break;

	case Variant::Type::LongLong:
		str_proto.append(byte_parser::Longlong2Bytes(ToLongLong()));
		break;

	case Variant::Type::Float:
		str_proto.append(byte_parser::Float2Bytes(ToFloat()));
		break;

	case Variant::Type::String:
	{
		auto str = ToString();
		str_proto.append(byte_parser::Int2Bytes(str.size()));
		str_proto.append(str);
		break;
	}

	default:
		D_ASSERT(false, "Unknown variant-type");
		break;
	}

	return str_proto;
}

PROTO_NAMESPACE_END