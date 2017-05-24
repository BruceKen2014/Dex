

#include "DexMd5Generator.h"



DexMd5Generator* DexMd5Generator::s_pMd5Generator = nullptr;

DexMd5Generator::DexMd5Generator()
{

}

DexMd5Generator::~DexMd5Generator()
{
	FreeLibrary(m_hDLL);
}

bool DexMd5Generator::sCreateDexMd5Generator()
{
	bool ret = true;
	if (s_pMd5Generator == nullptr)
	{
		s_pMd5Generator = new DexMd5Generator;
		ret = s_pMd5Generator->Initialize();
	}
	return ret;
}

DexMd5Generator* DexMd5Generator::sGetDexMd5Generator()
{
	return s_pMd5Generator;
}

bool DexMd5Generator::Initialize()
{
	m_hDLL = LoadLibrary("Cryptdll.dll");
	if (m_hDLL == nullptr)
		return false;

	MD5Init = (MD5Init_Type)GetProcAddress(m_hDLL, "MD5Init");
	MD5Update = (MD5Update_Type)GetProcAddress(m_hDLL, "MD5Update");
	MD5Final = (MD5Final_Type)GetProcAddress(m_hDLL, "MD5Final");
	if (MD5Init == nullptr || MD5Update == nullptr || MD5Final == nullptr)
	{
		FreeLibrary(m_hDLL);
		return false;
	}
	return  true;
}

DexMd5 DexMd5Generator::GenerateMd5(const char* str)
{
	DexMd5 ret;
	MD5_CTX md5Context;
	MD5Init(&md5Context);
	MD5Update(&md5Context, (unsigned char*)str, strlen(str));
	MD5Final(&md5Context);
	ret.low = *((long long*)md5Context.digest);
	ret.high = *((long long*)&(md5Context.digest[8]));
	return ret;
}