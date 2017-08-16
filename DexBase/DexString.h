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

//s2b:small to bigger,DTRUE:从小到大排序 DFALSE:从大到小排序
inline void DString_Sort(DString& str, DBool s2b = DTRUE)
{//使用lambda表达式进行排序
	std::sort(str.begin(), str.end(), [s2b](const DChar& a, const DChar& b)-> DBool { return  s2b ? (a < b) : (a > b); });
}
#endif