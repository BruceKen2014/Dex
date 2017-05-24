
#include <iostream>
#include "../sdk/zlib/include/zlib.h"
#include "DexBigFileHandle.h"
#include "DexFile.h"
#pragma comment(lib, "../sdk/zlib/lib/zdll.lib")
DexBigFileHandle::DexBigFileHandle()
{
	m_pFile = new DexFile;
	m_pReaderForContent = new DexFile;
	m_pCurContent = nullptr;
}
DexBigFileHandle::~DexBigFileHandle()
{
	ClearContent();
	delete m_pFile;
	delete m_pReaderForContent;
}

void DexBigFileHandle::setPrefix(string prefix)
{
	this->prefix = prefix;
}
bool DexBigFileHandle::InitBigFile(const char* filename)
{
	bool ret = m_pFile->OpenFile(filename);
	if (!ret)
		return false;
	uint64 fileLength = m_pFile->Length();
	uint64 offset = 0;
	m_pFile->Read(&m_header, offset, sizeof(m_header)); 
	uint32 iContentSize = sizeof(stDexBigFileContent);
	uint32 iBlockCount = m_header.blockCount;
	offset = fileLength - iContentSize * m_header.count - sizeof(stDexBigFileBlock) * iBlockCount;
	//read content data
	for (uint32 i = 0; i < m_header.count; ++i)
	{
		stDexBigFileContent* pContentPtr = new stDexBigFileContent;
		m_pFile->Read(pContentPtr, offset, iContentSize);
		m_mapContents[pContentPtr->md5Key] = pContentPtr;
		offset += iContentSize;
	}
	//read compress size
	m_vecBlocks.resize(iBlockCount);
	m_pFile->Read(&m_vecBlocks[0], offset, sizeof(stDexBigFileBlock)*iBlockCount);
	return ret;
}

uint64 DexBigFileHandle::ReadContent(const stDexBigFileContent* content, void* buffer)
{
	uint32 iBlockIndex = content->iBlockIndex;
	uint32 iBlockCount = content->iBlockCount;
	uint32 iBlockMaxSize = s_iBlockSize;
	uint32 iBlockSize = 0;
	uint64 iBlockOffset = 0;
	uint64 iBufferOffset = 0;
	bool		 bBlockCompress = true;
	uint32 destLength = s_iBlockSize;
	const stDexBigFileBlock* block = nullptr;
	for (uint32 b = 0; b < iBlockCount; ++b)
	{
		block = &m_vecBlocks[iBlockIndex];
		iBlockOffset = block->iByteOffset;
		iBlockSize = block->iBlockSize;
		bBlockCompress = block->bCompress;
		m_pFile->Read(m_pReadCache, iBlockOffset, iBlockSize);
		destLength = s_iBlockSize;
		if (bBlockCompress)
		{
			uncompress((Bytef*)((char*)buffer + iBufferOffset), (uLongf*)&destLength, (const Bytef*)m_pReadCache, iBlockSize);
			iBufferOffset += destLength;
		}
		else
		{
			memcpy((char*)buffer + iBufferOffset, m_pReadCache, iBlockSize);
			iBufferOffset += iBlockSize;
		}
		iBlockIndex++;
	}
	return iBufferOffset;
}

uint64 DexBigFileHandle::Read(void* buffer)
{
	uint64 ret = 0;
	if (m_pCurContent != nullptr)
		ret = ReadContent(m_pCurContent, buffer);
	return ret;
}
uint64 DexBigFileHandle::Read(const char* filename, void* buffer)
{
	uint64 ret = 0;
	DexMd5 md5 = DexMd5Generator::sGetDexMd5Generator()->GenerateMd5(filename);
	mapContent::iterator iter = m_mapContents.find(md5);
	if (iter != m_mapContents.end())
	{
		stDexBigFileContent* pContent = iter->second;
		ret = ReadContent(pContent, buffer);
	}
	return ret;
}

void DexBigFileHandle::AddPackFile(const char* filename)
{
	m_vecContentName.push_back(filename);
}

bool DexBigFileHandle::SaveBigFile(const char* filename)
{
	bool ret = m_pFile->NewFile(filename);
	if (!ret)
		return false;
	m_header.count = m_vecContentName.size();
	m_pFile->Write(&m_header, sizeof(m_header));
	uint32 iContentByte = sizeof(stDexBigFileContent);
	uint64 time = GetTickCount64();
	//write content file data
	uint32 destSize = 0;
	float ratio = 0.0f;
	uint32 startIndex = 0;
	uint64 fileOffset = 0;
	uint64 fileSize = 0;
	uint32 fileBlockCount = 0;
	uint32 tailByte = 0;
	uint32 writeLength = 0;
	uint32 BigFileBlockCount = 0;
	uint32 zlibSrcLength = 0;
	uint64 bigFileCompressLength = 0;
	uint64 bigFileSrcLength = 0;
	stDexBigFileBlock block;
	bool		 bCompress = false;
	int          compressRet = 0;
	for (vector<string>::iterator nameIte = m_vecContentName.begin(); nameIte < m_vecContentName.end(); ++nameIte)
	{
		const char* filename = (*nameIte).c_str();
		string  md5Filename = filename;
		size_t pos = md5Filename.find(prefix);
		if (pos != md5Filename.npos)
			md5Filename = md5Filename.substr(prefix.length());
		DexMd5 md5 = DexMd5Generator::sGetDexMd5Generator()->GenerateMd5(md5Filename.c_str());
		mapContent::iterator iter = m_mapContents.find(md5);
		if (iter == m_mapContents.end())
		{//新的文件
			ret = m_pReaderForContent->OpenFile(filename);
			if (ret)
			{
				fileOffset = 0;
				fileSize = m_pReaderForContent->Length();
				fileBlockCount = fileSize / s_iBlockSize;
				tailByte = fileSize % s_iBlockSize;
				if (tailByte != 0)
					fileBlockCount++;
				stDexBigFileContent* newContent = new stDexBigFileContent;
				newContent->md5Key = md5;
				newContent->iBlockIndex = BigFileBlockCount;
				newContent->iBlockCount = fileBlockCount;
				newContent->iOrigionSize = fileSize;
				m_mapContents[md5] = newContent;
				for (uint32 b = 0; b < fileBlockCount; ++b)
				{
					writeLength = s_iBlockSize;
					if (b == fileBlockCount - 1 && tailByte != 0)
					//最后一块数据 && 少于iBlockSize
						zlibSrcLength = tailByte;
					else
						zlibSrcLength = s_iBlockSize;
					m_pReaderForContent->Read(m_pReadCache, fileOffset, zlibSrcLength);
					compressRet = compress((Bytef*)m_pWriteCache, (uLongf*)&writeLength, (const Bytef*)m_pReadCache, zlibSrcLength);
					bCompress = compressRet == Z_OK;
					block.bCompress = bCompress;
					block.iByteOffset = m_pFile->Tell();
					if (bCompress)
					{
						m_pFile->Write(m_pWriteCache, writeLength);
						bigFileCompressLength += writeLength;
						block.iBlockSize = writeLength;
					}
					else
					{
						m_pFile->Write(m_pReadCache, zlibSrcLength);
						bigFileCompressLength += zlibSrcLength;
						block.iBlockSize = zlibSrcLength;
					}
					m_vecBlocks.push_back(block);
					fileOffset += zlibSrcLength;
				}
				bigFileSrcLength += fileSize;
				BigFileBlockCount += fileBlockCount;
				cout << "pack " <<md5Filename << " ok!" << endl;
			}
			else
			{//log open file error
			}
			m_pReaderForContent->Close();
		}
		else
		{//log file repeat error
		}
	}
	//write content
	for (mapContent::iterator ite = m_mapContents.begin(); ite != m_mapContents.end(); ++ite)
	{
		stDexBigFileContent* pContent = ite->second;
		m_pFile->Write(pContent, iContentByte);
	}
	//write block info
	uint32 blockCount = m_vecBlocks.size();
	if (blockCount != 0)
		m_pFile->Write(&m_vecBlocks[0], blockCount * sizeof(m_vecBlocks[0]));
	//写入block数量
	m_header.blockCount = blockCount;
	m_pFile->Seek(DexFile::DSEEK_SET, 0);
	m_pFile->Write(&m_header, sizeof(m_header));
	uint64 resLength = m_pFile->ReCalLength();
	m_pFile->Close();
	ratio = (float)bigFileCompressLength / bigFileSrcLength;
	time = GetTickCount64() - time;

	std::cout << "compress complete, " << " ratio:" << ratio << " use time:" << time << " ms " << 
		(double)bigFileSrcLength / 1024 / 1024 / time * 1000  <<" MB/s"<<endl;
	return ret;
}

bool DexBigFileHandle::OpenFile(const char* filename)
{
	DexMd5 md5 = DexMd5Generator::sGetDexMd5Generator()->GenerateMd5(filename);
	mapContent::iterator ite = m_mapContents.find(md5);
	if (ite != m_mapContents.end())
	{
		m_pCurContent = ite->second;
		return true; 
	}
	m_pCurContent = nullptr;
	return false;
}
uint64 DexBigFileHandle::FileSize()
{
	uint64 ret = 0;
	if (m_pCurContent != nullptr)
		ret = m_pCurContent->iOrigionSize;
	return ret;
}
void DexBigFileHandle::CloseFile()
{
	m_pCurContent = nullptr;
}

uint32 DexBigFileHandle::ContentCount()
{
	return m_mapContents.size();
}

void DexBigFileHandle::ClearContent()
{
	for (mapContent::iterator ite = m_mapContents.begin(); ite != m_mapContents.end(); ++ite)
	{
		if (ite->second != nullptr)
		{
			delete ite->second;
		}
	}
	m_mapContents.clear();
	m_vecContentName.clear();
	m_vecBlocks.clear();
}