#include "encrypt.h"
#include "crypto/include/md5.h"
#include "base/util/string_util.h"
#include "crypto/include/cryptlib.h"
#include "crypto/include/config.h"

#ifdef _DEBUG
#pragma comment(lib, "cryptlib_d.lib")
#else
#pragma comment(lib, "cryptlib.lib")
#endif

using namespace CryptoPP;

void CalMd5(std::string &input, std::string &output)
{
	byte arr_byte[16] = { 0 };

	Weak1::MD5 md5; 
	md5.CalculateDigest(arr_byte, (byte*)(input.c_str()), input.size());
	
	output = nbase::BinaryToHexString((void*)arr_byte, sizeof(arr_byte));
}