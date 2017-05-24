
#ifndef _DEX_BIGFILE_HANDLE_H
#define _DEX_BIGFILE_HANDLE_H
#include <map>
#include <vector>
#include <string>
#include "DexType.h"
using namespace std;
#include "DexMd5Generator.h"

class DexFile;
class DexBigFileHandle
{
public:
typedef struct _stDexBigFileContent
{
		   DexMd5 md5Key;  //16 byte
		   uint32  iBlockIndex;
		   uint32  iBlockCount;
		   uint32  iOrigionSize;
		   uint32  dummy;
		   _stDexBigFileContent()
		   {
			   iBlockCount = 0;
			   iBlockIndex = 0;
			   iBlockCount = 0;
			   iOrigionSize = 0;
			   dummy = 0;
		   }
}stDexBigFileContent;
typedef struct _stDexBigFileHeader
{
	 uint32 count;
	 uint32 blockCount;
     _stDexBigFileHeader() :count(0), blockCount(0){};
}stDexBigFileHeader;
typedef struct _stDexBigFileBlock
{
	uint64 iByteOffset;
	uint32  iBlockSize;
	bool	bCompress;
	
	_stDexBigFileBlock() :bCompress(true), iBlockSize(0), iByteOffset(0){};
}stDexBigFileBlock;
public:
	void setPrefix(string prefix); //use for save big file
	//read
	bool InitBigFile(const char*  filename);
	uint64 Read(void* buffer);
	uint64 Read(const char* filename, void* buffer);
	//write
	void AddPackFile(const char* filename);
	bool SaveBigFile(const char* filename);
	//func
	bool OpenFile(const char* filename);
	uint64 FileSize();
	void CloseFile();
	uint32 ContentCount();
	void ClearContent();
public:
	DexBigFileHandle();
	virtual ~DexBigFileHandle();
protected:
	uint64 ReadContent(const stDexBigFileContent* content, void* buffer);
public:
	static const int s_iBlockSize = 1024 * 64;
	
protected:
	stDexBigFileHeader  m_header;
	typedef map<DexMd5, stDexBigFileContent*> mapContent;
	mapContent   m_mapContents;
	stDexBigFileContent* m_pCurContent;
	DexFile*     m_pFile;		
	DexFile*     m_pReaderForContent;
	vector<string> m_vecContentName; //for pack bigfile

	char        m_pReadCache[s_iBlockSize];
	char        m_pWriteCache[s_iBlockSize];

	vector<stDexBigFileBlock> m_vecBlocks;
	string prefix;
};
#endif