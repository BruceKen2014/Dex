

#ifndef _DEX_BASE_TYPE_TRANSLATER_H
#define _DEX_BASE_TYPE_TRANSLATER_H

#include <d3dx9math.h>

#include "DexString.h"
#include "DexDVector.h"

class DexCommonFunction
{
public:
	static DString int_to_str(int i);
	static DString float_to_str(float f);

	static int    str_to_int(DString str);
	static int    str_to_int(char* str);

	static float  str_to_float(DString str);
	static float  str_to_float(char* str);

	static bool   str_to_bool(DString str);
	static bool   str_to_bool(char* str);

	static bool   strcat(DString& ret, DString str);
	static bool   strcat(char* ret,const char* str);
	  
	static void SplitStr(DString str, char split_char, DVector<DString> &out);
	static void DeleteStrSpace(DString& str); //�Ƴ��ַ����еĿո�
	static void DeleteStrSpace(char* str); //�Ƴ��ַ����еĿո�
	static DString D3DVector3toStr(const D3DXVECTOR3& vec, char split_char = ';');
	static bool   StrToD3DVector3(DString str, D3DXVECTOR3& vec, char split_char = ';');

	//�������ܣ�ȡ���ļ����е�·�����紫��filename��model/x.obj,����path��model/
	static DString&   getFilePath(DString filename, DString& path);

	//�������ܣ�ȡ���ļ����еĴ��ļ������紫��filename��model/x.obj,����name��x.obj
	static DString&   GetFilePureName(DString filename, DString& name);

	//�ж�filename�ĺ�׺���Ƿ���Type
	//��filename = 123.mp3
	//Type = mp3
	bool IsTypeFile(const char* filename, const char* Type);

	template<typename T>
	static inline T& clip(T& _value, const T& _small, const T& _big);
};
template<typename T>
inline T& DexCommonFunction::clip(T& _value, const T& _small, const T& _big)
{
	if (_value <_small) _value = _small;
	else if (_value >_big)   _value = _big;
	return _value;
}
#endif