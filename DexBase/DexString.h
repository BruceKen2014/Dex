#ifndef _DEX_DSTRING_H
#define _DEX_DSTRING_H

#include <string>
#include <algorithm>
#include "DexType.h"

typedef std::string DString;

#define dexstrcmp		strcmp
#define dexstricmp		stricmp
#define dexstrchr		strchr
#define dexstrcat		strcat
#define dexstrcpy		strcpy
#define dexatoi			atoi
#define dexstrlen		strlen

//s2b:small to bigger,DTRUE:��С�������� DFALSE:�Ӵ�С����
inline void DString_Sort(DString& str, DBool s2b = DTRUE)
{//ʹ��lambda���ʽ��������
	std::sort(str.begin(), str.end(), [s2b](const DChar& a, const DChar& b)-> DBool { return  s2b ? (a < b) : (a > b); });
}
#endif