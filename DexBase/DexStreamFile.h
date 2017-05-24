
#ifndef _DEX_STREAM_FILE_H
#define _DEX_STREAM_FILE_H
#include "DexType.h"
#define  USE_BIG_FILE
class DexFile;
class DexBigFileHandle;
class DexStreamFile
{
public:
	static DexStreamFile* sCreateStreamFile();
	static DexStreamFile* sGetStreamFile();
public:
	bool AddBigFile(const char* filename);
	uint64 ReadData(void* buffer);
	bool OpenFile(const char* filename);
	uint64 FileSize();
	void CloseFile();
	//open file->get file size0->allocate memory->read data->close file
protected:
	DexStreamFile();
	virtual ~DexStreamFile();
private:
	static DexStreamFile* s_pStreamFile;
	static const int s_iBigFileCount = 5;
	DexBigFileHandle*  m_arrBigFile[s_iBigFileCount];
	DexFile*    m_pDexFile;  //use for small file
	DexBigFileHandle* m_pBigFileHandler; //use for big file
};
#endif //_DEX_STREAM_FILE_H