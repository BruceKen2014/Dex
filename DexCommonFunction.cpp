
#include "DexCommonFunction.h"


string DexCommonFunction::int_to_str(int i)
{
	char ch[32];
	sprintf(ch, "%d", i);
	return string(ch);
}
string DexCommonFunction::float_to_str(float f)
{
	char ch[32];
	sprintf(ch, "%.3f",  f);
	return string(ch);
}



int DexCommonFunction::str_to_int(string str)
{
	return atoi(str.c_str());
}
int DexCommonFunction::str_to_int(char* str)
{
	if(str == NULL)
		return 0;
	return atoi(str);
}

float DexCommonFunction::str_to_float(string str)
{
	return atof(str.c_str());
}
float DexCommonFunction::str_to_float(char* str)
{
	if(str == NULL)
		return 0.0f;
	return atof(str);
}

bool DexCommonFunction::str_to_bool(string str)
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

bool DexCommonFunction::strcat(string& ret, string str)
{
	ret += str;
	return true;
}
bool DexCommonFunction::strcat(char* ret, const char* str)
{
	::strcat(ret, str);
	return true;
}

void DexCommonFunction::SplitStr(string str, char split_char, vector<string> &out)
{
	char* pt = (char*)str.c_str();
	char* begin = pt;
	char* end = strchr(pt, split_char);
	while(end != NULL)
	{		
		string temp;
		temp.insert(0, begin, end - begin);
		out.push_back(temp);
		begin = end +1;
		end = strchr(begin, split_char);
	}
	if(end == NULL && begin != NULL)
	{
		string temp;
		temp.insert(0, begin, pt + str.size() - begin);
		out.push_back(temp);
	}
}

string DexCommonFunction::D3DVector3toStr(const D3DXVECTOR3& vec, char split_char)
{
	string str;
	string temp;
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
bool DexCommonFunction::StrToD3DVector3(string str, D3DXVECTOR3& vec, char split_char)
{
	std::vector<string> str_arr;
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
