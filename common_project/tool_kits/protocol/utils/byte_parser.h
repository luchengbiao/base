#ifndef __PROTO_UTILS_BYTE_PARSER_H__
#define __PROTO_UTILS_BYTE_PARSER_H__
#include <stddef.h> // for size_t
#include <stdint.h> // for uint8_t
#include <string>
#include "protocol/macros.h"

PROTO_NAMESPACE_BEGIN

namespace byte_parser
{
	class EndianChecker
	{
	public:
		static bool IsLittleEndian()
		{
			static EndianChecker checker;

			return checker.u_.c == 1;
		}

	private:
		EndianChecker()
		{
			u_.i = 1;
		}

		union
		{
			int		i;
			char	c;
		}u_;
	};

#define IS_LITTLE_ENDIAN (EndianChecker::IsLittleEndian())

	// Read an integer (signed or unsigned) from |buf|
	template<typename T>
	inline void ReadFromBytes(const char buf[], T* out, bool read_as_lb_order = true)
	{
		if (IS_LITTLE_ENDIAN == read_as_lb_order)
		{
			*out = buf[0];
			for (size_t i = 1; i < sizeof(T); ++i)
			{
				*out <<= 8;
				// Must cast to uint8_t to avoid clobbering by sign extension.
				*out |= static_cast<uint8_t>(buf[i]);
			}
		}
		else
		{
			*out = buf[sizeof(T) - 1];
			for (int i = sizeof(T) - 2; i >= 0; --i)
			{
				*out <<= 8;
				*out |= static_cast<uint8_t>(buf[i]);
			}
		}
	}

	template<typename T>
	inline T ReadFromBytes(const char buf[], bool read_as_lb_order = true)
	{
		T t;

		ReadFromBytes(buf, &t, read_as_lb_order);

		return t;
	}

	// Write an integer (signed or unsigned) |val| to |buf|
	template<typename T>
	inline void WriteToBytes(char buf[], T val, bool write_as_lb_order = true)
	{
		if (IS_LITTLE_ENDIAN == write_as_lb_order)
		{
			for (size_t i = 0; i < sizeof(T); ++i)
			{
				buf[sizeof(T)-i - 1] = static_cast<char>(val & 0xFF);
				val >>= 8;
			}
		}
		else
		{
			for (size_t i = 0; i < sizeof(T); ++i)
			{
				buf[i] = static_cast<char>(val & 0xFF);
				val >>= 8;
			}
		}
	}

	inline int Bytes2Int(const char bytes[], bool read_as_lb_order = true)
	{
		return ReadFromBytes<int>(bytes, read_as_lb_order);
	}

	inline std::string Int2Bytes(int i, bool write_as_lb_order = true)
	{
		char bytes[sizeof(int)];
		
		WriteToBytes(&bytes[0], i, write_as_lb_order);

		return std::string(std::cbegin(bytes), std::cend(bytes));
	}

	inline long long Bytes2Longlong(const char *bytes, bool read_as_lb_order = true)
	{
		return ReadFromBytes<long long>(bytes, read_as_lb_order);
	}

	inline std::string Longlong2Bytes(long long ll, bool write_as_lb_order = true)
	{
		char bytes[sizeof(long long)];

		WriteToBytes(&bytes[0], ll, write_as_lb_order);

		return std::string(std::cbegin(bytes), std::cend(bytes));
	}

	union IntUFloat 
	{
		float	f;
		int		i;
	};

	inline float Bytes2Float(const char *bytes, bool read_as_lb_order = true)
	{
		IntUFloat iuf;

		iuf.i = Bytes2Int(bytes, read_as_lb_order);

		return iuf.f;
	}

	inline std::string Float2Bytes(float f, bool write_as_lb_order = true)
	{
		IntUFloat iuf;
		iuf.f = f;

		return Int2Bytes(iuf.i, write_as_lb_order);
	}
}

PROTO_NAMESPACE_END

#endif