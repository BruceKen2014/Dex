
#include "stdafx.h"
#include <stdio.h>
#include  "CFile.h"


CFile::CFile()
{
	m_file = NULL;
}
CFile::~CFile()
{

}
bool CFile::Check()
{
	if(m_file == INVALID_HANDLE_VALUE)
		return false;
	return true;
}
bool CFile::CreateCFile(char* filename)
{
	m_file= CreateFile(TEXT(filename), GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_file == INVALID_HANDLE_VALUE)
		return false;
	return true;
}
bool CFile::OpenCFile(char * filename)
{
	m_file= CreateFile(TEXT(filename), GENERIC_WRITE, 0, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_file == INVALID_HANDLE_VALUE)
		return false;
	return true;
}
void CFile::GoToEnd()
{
	SetFilePointer(m_file, 0, 0, FILE_END);
}
void CFile::Close()
{
	CloseHandle(m_file);
	m_file = NULL;
}
void CFile::InBlankLine(int count)
{
	if(!Check())
		return;
	for(int i = 0; i < count; i++)
	{
		CHAR cData[5];
		wsprintf(cData, "\r\n");
		DWORD dwBytesWritten;
		WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
	}
}
void CFile::InBlankChar(int count)
{
	if(!Check())
		return;
	for(int i = 0; i < count; i++)
	{
		CHAR cData[2];
		wsprintf(cData, " ");
		DWORD dwBytesWritten;
		WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
	}
}
void CFile::InNumber(int integer)
{
	if(!Check())
		return;
	CHAR cData[32];
	wsprintf(cData, "%d", integer);
	DWORD dwBytesWritten;
	WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
}
void CFile::InNumber(long _long)
{
	if(!Check())
		return;
	CHAR cData[32];
	wsprintf(cData, "%ld", _long);
	DWORD dwBytesWritten;
	WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
}

void CFile::InNumber(int64 llong)
{
	if(!Check())
		return;
	char cData[64];
	//_i64toa(llong, cData,10); //功能和下个语句相同
	sprintf(cData, "%I64d", llong);
	DWORD dwBytesWritten;
	WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
}

void CFile::InNumber(float f, int precise)
{
	if(!Check())
		return;
	CHAR cData[32];
	char format[32];
	sprintf(format, "%%.%df", precise, f);
	sprintf(cData, format, f);
	DWORD dwBytesWritten;
	WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
}
void CFile::InString(char *word, ...)
{
	if(!Check())
		return;
	va_list v_a;											
	va_start(v_a, word);
	CHAR cData[1024];
	wvsprintf(cData, word, v_a);  //不支持浮点数
	va_end(v_a);
	DWORD dwBytesWritten;
	WriteFile(m_file, &cData, strlen(cData), &dwBytesWritten, NULL);
}

CFile* getFileHandler()
{
	static CFile* g_File = new CFile;
	return g_File;
}