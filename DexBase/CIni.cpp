

#include "CIni.h"

int CIni::GetInt(char *filename, char *field, char *key)
{
	return ::GetPrivateProfileInt(field, key, 0, filename);
}
void CIni::SetInt(char *filename, char *field, char *key, int _value)
{
	char char_value[16];
	::itoa(_value, char_value, 10);
	SetString(filename, field, key, char_value);
}

float CIni::GetFloat(char *filename, char *field, char *key)
{
	char char_value[16];
	GetString(filename, field, key, char_value);
	return ::atof(char_value);
}

void CIni::SetFloat(char *filename, char *field, char *key, float _value, int precise)
{
	CHAR cData[32];
	char format[32];
	sprintf(format, "%%.%df", precise, _value);
	sprintf(cData, format, _value);
	SetString(filename, field, key, cData);
}

void CIni::GetString(char *filename, char *field, char *key, char *ret)
{
	::GetPrivateProfileString(field, key, "not find!", ret, INI_MAX_SIZE, filename);
}

void CIni::SetString(char *filename, char *field, char *key, char *content)
{
	::WritePrivateProfileString(field, key, content, filename);
}

void CIni::Trans(char *savefile)
{	//save操作，读入save文件，生成临时文本文件，从文本中读入数据，最后删除临文件
	std::ifstream infile;
	infile.open(savefile, std::ios::binary);
	char* content = new char[4096];
	int count = 0; 
	char temp; 
	while(!infile.eof())
	{
		infile.get(temp);
		content[count] = temp;
		count++;
	}
	std::ofstream outfile;
	outfile.open("script\\new.save", std::ios::binary | std::ios::trunc);
	for( int j = 0 ;j < count; j++)
		outfile.put(content[j]+10);
	//outfile.write(content, 4096);
	outfile.close();
	infile.close();
	_SafeFree(content);
	infile.open("script\\new.save", std::ios::binary);
	infile.seekg(0, std::ios::beg);
	char* content2 = new char[4096];
	count = 0;
	while(!infile.eof())
	{
		infile.get(temp);
		content2[count] = temp - 10;
		count++;
	}
	outfile.open("script\\new2.save", std::ios::binary | std::ios::trunc);
	outfile.seekp(0, std::ios::beg);
	for( int j = 0 ;j < count; j++)
		outfile.put(content2[j]);
	outfile.close();
	infile.close();
	_SafeFree(content2);
}