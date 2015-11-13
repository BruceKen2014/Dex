

#ifndef _DEX_BASE_TYPE_TRANSLATER_H
#define _DEX_BASE_TYPE_TRANSLATER_H

#include <string>
#include <vector>

#include <d3dx9math.h>

using namespace std;
class CDexCommonFunction
{
	static string int_to_str(int i);
	static string float_to_str(float f);	

	static int    str_to_int(string str);
	static int    str_to_int(char* str);

	static float  str_to_float(string str);
	static float  str_to_float(char* str);

	static bool   str_to_bool(string str);
	static bool   str_to_bool(char* str);

	static bool   strcat(string& ret, string str);
	static bool   strcat(char* ret,const char* str);
	  
	static void SplitStr(string str, char split_char, vector<string> &out);
	static string D3DVector3toStr(const D3DXVECTOR3& vec, char split_char=';');
	static bool   StrToD3DVector3(string str, D3DXVECTOR3& vec, char split_char=';');

};
#endif