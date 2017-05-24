
#ifndef _DEX_FILE_H
#define _DEX_FILE_H
#include <stdio.h>
#include "DexType.h"
class DexFile
{
public:
	typedef enum 
	{
		DSEEK_CUR,
		DSEEK_SET,
		DSEEK_END
	}DexFile_Seek;
public:
	DexFile();
	virtual ~DexFile();
public:
	//read
	bool OpenFile(const char* filename);
	uint64 Read(void* buffer, uint64 offset, int64 count);
	bool ReadLine(char* buffer); //返回值表明读完后文件是否结束
	//write
	bool NewFile(const char* filename);
	void Write(void* buffer, uint64 count);
	bool Seek(DexFile_Seek seek, int offset);
	//
	void Close();
	uint64 Tell();
	uint64 Length();
	uint64 ReCalLength();
private:
	FILE* m_pFileHandle;
	uint64 m_iLength;
};
#endif