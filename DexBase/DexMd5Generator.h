

#ifndef _DEX_MD5_GENERATOR_H
#define _DEX_MD5_GENERATOR_H
#include <windows.h>
#include "DexType.h"
//这个md5生成器只用于windows平台，如果之后需要在其他平台生成md5，需要进行抽象并且派生各个平台的子类
typedef struct _DexMd5
{
	long long low;
	long long high;
	_DexMd5() :low(0), high(0){};
	bool operator < (const _DexMd5& md5) const
	{
		if (high < md5.high)
			return true;
		else if (high == md5.high)
		{
			return low < md5.low;
		}
		return false;
	};
}DexMd5;

typedef struct
{
	unsigned long i[2];
	unsigned long buf[4];
	unsigned char in[64];
	unsigned char digest[16];
}MD5_CTX;

typedef void(WINAPI* MD5Init_Type)(MD5_CTX* context);
typedef void(WINAPI* MD5Update_Type)(MD5_CTX* content, unsigned char* input, DUDInt32 inlen);
typedef void(WINAPI* MD5Final_Type)(MD5_CTX* context);

class DexMd5Generator
{
	
public:
	static bool sCreateDexMd5Generator();
	static DexMd5Generator* sGetDexMd5Generator();

public:
	DexMd5 GenerateMd5(const char* str);
protected:
	DexMd5Generator();
	virtual ~DexMd5Generator();
protected:
	virtual bool Initialize();
protected:
	static DexMd5Generator* s_pMd5Generator;
	HINSTANCE m_hDLL;
	MD5Init_Type MD5Init;
	MD5Update_Type MD5Update;
	MD5Final_Type MD5Final;
};
#endif