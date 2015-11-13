
/*************************************************************
****Dex引擎配置文件读写类
****@作者：阚正杰(BruceKen)
****2012.12
*************************************************************/
#pragma  once

#include "typedefine.h"
#include <fstream>

#define   INI_MAX_SIZE   1024   //配置文件中键内容的最长字节
class CIni
{
public:
	static int   GetInt(char* filename, char* field, char* key);
	static void  SetInt(char* filename, char*field, char*key, int _value);

	static float GetFloat(char* filename, char* field, char* key);
	static void  SetFloat(char* filename, char*field, char*key, float _value, int precise = 3);	 

	static void  GetString(char* filename, char* field, char* key, char* ret);
	static void  SetString(char* filename, char*field, char*key, char* content);	

	static void  Trans(char* savefile);
};