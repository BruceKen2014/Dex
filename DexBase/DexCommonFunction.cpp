
#include "DexCommonFunction.h"
#include "DexMemoryManager.h"

DString DexCommonFunction::int_to_str(int i)
{
	char ch[32];
	sprintf(ch, "%d", i);
	return DString(ch);
}
DString DexCommonFunction::float_to_str(float f)
{
	char ch[32];
	sprintf(ch, "%.3f",  f);
	return DString(ch);
}



int DexCommonFunction::str_to_int(DString str)
{
	return atoi(str.c_str());
}
int DexCommonFunction::str_to_int(char* str)
{
	if(str == NULL)
		return 0;
	return atoi(str);
}

float DexCommonFunction::str_to_float(DString str)
{
	return atof(str.c_str());
}
float DexCommonFunction::str_to_float(char* str)
{
	if(str == NULL)
		return 0.0f;
	return atof(str);
}

bool DexCommonFunction::str_to_bool(DString str)
{
	if(str == "true")
		return true;
	if(str == "false")
		return false;
	if(str_to_int(str) == 0)
		return false;
	return true;
}

bool DexCommonFunction::str_to_bool(char* str)
{
	if(str == NULL)
		return false;
	if(strcmp(str,"true") == 0)
		return true;
	if(strcmp(str,"false") == 0)
		return false;
	if(str_to_int(str) == 0)
		return false;
	return true;
}

bool DexCommonFunction::strcat(DString& ret, DString str)
{
	ret += str;
	return true;
}
bool DexCommonFunction::strcat(char* ret, const char* str)
{
	::strcat(ret, str);
	return true;
}

void DexCommonFunction::SplitStr(DString str, char split_char, DVector<DString> &out)
{
	char* pt = (char*)str.c_str();
	char* begin = pt;
	char* end = strchr(pt, split_char);
	while(end != NULL)
	{		
		DString temp;
		temp.insert(0, begin, end - begin);
		out.push_back(temp);
		begin = end +1;
		end = strchr(begin, split_char);
	}
	if(end == NULL && begin != NULL)
	{
		DString temp;
		temp.insert(0, begin, pt + str.size() - begin);
		out.push_back(temp);
	}
}

void DexCommonFunction::DeleteStrSpace(DString& str)
{
	DUDInt32 length = str.length();
	char* tempString = DexMemoryManager::sGetMemoryManager()->allocateMemory(length + 1);
	char* flag = tempString;
	const char* ptr = str.c_str();
	while (*ptr != '\0')
	{
		if (*ptr != ' ')
			*flag = *ptr;
		++flag;
		++ptr;
	}
	*flag = '\0';
	str.assign(tempString);
	DexMemoryManager::sGetMemoryManager()->deallocateMemory((void*)tempString);
}

void DexCommonFunction::DeleteStrSpace(char* str)
{
	DUDInt32 length = dexstrlen(str);
	if (length == 0)
		return;
	char* tempString = DexMemoryManager::sGetMemoryManager()->allocateMemory(length + 1);
	char* flag = tempString;
	const char* ptr = str;
	while (*ptr != '\0')
	{
		if (*ptr != ' ')
			*flag++ = *ptr;
		++ptr;
	}
	*flag = '\0';
	dexstrcpy(str, tempString);
	DexMemoryManager::sGetMemoryManager()->deallocateMemory((void*)tempString);
}

DString DexCommonFunction::D3DVector3toStr(const D3DXVECTOR3& vec, char split_char)
{
	DString str;
	DString temp;
	temp = float_to_str(vec.x);
	str += temp;
	str.append(1, split_char);
	temp.clear();
	temp = float_to_str(vec.y);
	str += temp;
	str.append(1, split_char);
	temp.clear();
	temp = float_to_str(vec.z);
	str += temp;
	return str;
}
bool DexCommonFunction::StrToD3DVector3(DString str, D3DXVECTOR3& vec, char split_char)
{
	DVector<DString> str_arr;
	SplitStr(str, split_char, str_arr);
	if(str_arr.size() != 3)
		return false;
	vec.x  = atof(str_arr[0].c_str());
	vec.y  = atof(str_arr[1].c_str());
	vec.z  = atof(str_arr[2].c_str());
	return true;
}

DString& DexCommonFunction::getFilePath(DString filename, DString& path)
{
	int length = filename.length();
	int last_pos = filename.rfind('/');
	path = filename.substr(0, last_pos + 1);
	return path;
}
