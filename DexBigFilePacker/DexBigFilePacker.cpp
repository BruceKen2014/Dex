


#include <iostream>
#include <windows.h>
#include <io.h>
#include <string>
#include <deque>
#include "../DexBase/DexMd5Generator.h"
#include "../DexBase/DexBigFileHandle.h"
#include "../xml/tinyxml.h"
#include "../DexBase/DexFile.h"
#include "../sdk/zlib/include/zlib.h"
#include "CPackTask.h"

#pragma comment(lib, "../bin/Dex.lib")
#pragma comment(lib, "../sdk/zlib/lib/zdll.lib")

using namespace std;
char prefix[128] = "F:/MyFiles/";//default value
char log_filename[128]= "log.txt";
vector<string>  vec_filters;
vector<string>  g_vecFileName;
DexFile logFile;
bool dealFileName(string& filename)
{
	//check filter
	for (size_t i = 0; i < vec_filters.size(); ++i)
	{
		if (filename.find(vec_filters[i]) != filename.npos)
		{
			//be filtered
			return false;
		}
	}
	//check prefix
	size_t pos = filename.find(prefix);
	if (pos != filename.npos)
		filename = filename.substr(strlen(prefix));
	return true;
}
void pak_log(const char* text, ...)
{
	va_list v_p;
	va_start(v_p, text);
	char szText[2048] = "\0";
	vsprintf_s(szText, text, v_p);
	va_end(v_p);
	cout << szText;
	logFile.Write(szText, strlen(szText));
}
void addDirectoryFiles(const char* directory)
{
	_finddata_t fileinfo;
	long hFile = 0;
	string temp;
	hFile = _findfirst(temp.append(directory).append("/*").c_str(), &fileinfo);
	if (hFile != -1)
	{
		do
		{
			string p;
			if (fileinfo.attrib & _A_SUBDIR)
			{//folder
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					addDirectoryFiles(p.append(directory).append("/").append(fileinfo.name).c_str());
			}
			else
			{//file
				string addFileNmae(directory);
				addFileNmae.append("/").append(fileinfo.name);
				g_vecFileName.push_back(addFileNmae);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

CObjectPool<CPackTaskFlow> g_taskPool(64);
CPackTaskManager g_taskManager;
HANDLE g_readEvent;
HANDLE g_pakOver;
DWORD WINAPI ReadThread(LPVOID lpParam)
{
	while (1)
	{
		g_taskManager.RunRead();
	}
	return 0;
}
DWORD WINAPI CompressThread(LPVOID lpParam)
{
	while (1)
	{
		g_taskManager.RunCompress();
	}
	return 0;
}
DWORD WINAPI WriteThread(LPVOID lpParam)
{
	while (1)
	{
		g_taskManager.RunWrite();
	}
	return 0;
}
DWORD WINAPI CheckThread(LPVOID lpParam)
{
	while (1)
	{
		if (g_taskManager.m_taskRead.m_queue.size() == 0 &&
			g_taskManager.m_taskCompress.m_queue.size() == 0 &&
			g_taskManager.m_taskWrite.m_queue.size() == 0)
			SetEvent(g_pakOver);
	}
	return 0;
}
bool SaveBigFile(const char* filename)
{
/*
���ļ��ĸ�ʽΪ
1���ļ�ͷ��Ϣ
2��������ļ�����file data��ȫ��������ѹ��
3��������ļ���Ϣ���� content area
4���ļ�������        content block area

ΪʲôҪ����������ƣ���Ҫ��Ϊ��ѹ�����ǣ�һС��һС���ѹ�����������Ч��
��������˵��content areaҲ�����ļ���Ϣ����Ӧ��β����header֮��������ͨ˳һ�㣬������д��content info��ʱ�������ļ���д����������һ�β���
����ʵ��д���ļ���Ϣʱ�ֻ���ļ����ж�д����������ζ�д�ˡ�����Ϊ�˼����ļ���д��ʱ�䣬�Ƚ������ļ�����д�룬���ʱ�򱣴��ļ���Ϣ��д���ļ����ݺ�
�ٽ����ļ���Ϣ���д�롣
�ļ���Ĵ���Ҳ����ˣ�д��block��ʱ����Ҫ֪��ѹ����Ĵ�С�����������д��block��Ϣ�����ʱ�򻹲�֪���ļ����ݱ�ѹ���Ĵ�С���������ѹ��һ�β�֪����������
д���ļ����ݵ�ʱ��Ҫ����һ��ѹ�������������ѹ������������Ϊ�˽�ʡʱ�䣬�Ƚ����ļ����ݵ�д�룬д���ڼ��֪���ļ���С��blockѹ������Ϣ��֮��д���ļ���Ϣ��
block��Ϣ��ʱ��Ͳ����ٲ����ļ��ͽ���ѹ�������ˡ�
*/
	DexFile bigFile;
	DexFile readerForContent;
	bool ret = bigFile.NewFile(filename);
	if (!ret)
	{
		pak_log("save big file error! create file:%s failed!\n", filename);
		return false;
	}
	typedef map<DexMd5, DexBigFileHandle::stDexBigFileContent*> mapContent;
	mapContent contents;
	DexBigFileHandle::stDexBigFileHeader header;
	DUInt32 iBlockSize = DexBigFileHandle::s_iBlockSize;
	vector<DexBigFileHandle::stDexBigFileBlock> vecBlocks;
	char* readCache = new char[iBlockSize];
	char* writeCache = new char[iBlockSize];

	//������ļ������ļ�������Ϣ
	//ע�⣬�����header��ʱֻ��count��Ϣ�ǺϷ��ģ�û��blockcount��Ϣ����Ϊ���ʱ������ֻ֪�����ٸ��ļ�����֪�����ٸ��ڴ��
	header.count = g_vecFileName.size();
	bigFile.Write(&header, sizeof(header));
	
	uint64 time = GetTickCount64();
	//write content file data
	DUInt32 destSize = 0;
	float ratio = 0.0f;
	DUInt32 startIndex = 0;
	uint64 fileOffset = 0;
	uint64 fileSize = 0;
	DUInt32 fileBlockCount = 0;
	DUInt32 tailByte = 0;
	DUInt32 writeLength = 0;
	DUInt32 BigFileBlockCount = 0;
	DUInt32 zlibSrcLength = 0;
	uint64 bigFileCompressLength = 0;
	uint64 bigFileSrcLength = 0;
	DexBigFileHandle::stDexBigFileBlock block;
	bool		 bCompress = false;
	int          compressRet = 0;
	for (vector<string>::iterator nameIte = g_vecFileName.begin(); nameIte < g_vecFileName.end(); ++nameIte)
	{
		const char* filename = (*nameIte).c_str();
		string  md5Filename = filename;
		bool dealFile = dealFileName(md5Filename);
		if (!dealFile)
			continue;
		DexMd5 md5 = DexMd5Generator::sGetDexMd5Generator()->GenerateMd5(md5Filename.c_str());
		ret = readerForContent.OpenFile(filename);
		if (ret)
		{
			fileOffset = 0;
			fileSize = readerForContent.Length();
			fileBlockCount = fileSize / iBlockSize;
			tailByte = fileSize % iBlockSize;
			if (tailByte != 0)
				fileBlockCount++;
			DexBigFileHandle::stDexBigFileContent* newContent = new DexBigFileHandle::stDexBigFileContent;
			newContent->md5Key = md5;
			newContent->iBlockIndex = BigFileBlockCount;
			newContent->iBlockCount = fileBlockCount;
			newContent->iOrigionSize = fileSize;
			contents[md5] = newContent;
			for (DUInt32 b = 0; b < fileBlockCount; ++b)
			{
				writeLength = iBlockSize;
				if (b == fileBlockCount - 1 && tailByte != 0)
					//���һ������ && ����iBlockSize
					zlibSrcLength = tailByte;
				else
					zlibSrcLength = iBlockSize;
				readerForContent.Read(readCache, fileOffset, zlibSrcLength);
				compressRet = compress((Bytef*)writeCache, (uLongf*)&writeLength, (const Bytef*)readCache, zlibSrcLength);
				bCompress = compressRet == Z_OK;
				block.bCompress = bCompress;
				block.iByteOffset = bigFile.Tell();
				if (bCompress)
				{
					bigFile.Write(writeCache, writeLength);
					bigFileCompressLength += writeLength;
					block.iBlockSize = writeLength;
				}
				else
				{
					bigFile.Write(readCache, zlibSrcLength);
					bigFileCompressLength += zlibSrcLength;
					block.iBlockSize = zlibSrcLength;
				}
				vecBlocks.push_back(block);
				fileOffset += zlibSrcLength;
			}
			bigFileSrcLength += fileSize;
			BigFileBlockCount += fileBlockCount;
			pak_log("pack %s ok!\n", md5Filename.c_str());
			readerForContent.Close();
		}
		else
		{//log file open error
		}
	}
	//write content
	for (mapContent::iterator ite = contents.begin(); ite != contents.end(); ++ite)
	{
		DexBigFileHandle::stDexBigFileContent* pContent = ite->second;
		bigFile.Write(pContent, sizeof(DexBigFileHandle::stDexBigFileContent));
	}
	//write block info
	DUInt32 blockCount = vecBlocks.size();
	if (blockCount != 0)
		bigFile.Write(&vecBlocks[0], blockCount * sizeof(vecBlocks[0]));
	//д��block����
	header.blockCount = blockCount;
	bigFile.Seek(DexFile::DSEEK_SET, 0);
	bigFile.Write(&header, sizeof(header));
	uint64 resLength = bigFile.ReCalLength();
	bigFile.Close();
	ratio = (float)bigFileCompressLength / bigFileSrcLength;
	time = GetTickCount64() - time;
	delete[] readCache;
	delete[] writeCache;
	float speed = bigFileSrcLength / 1024.0f / 1024 / time * 1000;
    //����ĸ�ʽ������.f�������������������%f %d %f����ڶ���f�������ȷ����֪Ϊ��
	pak_log("%s compress complete, ratio:%.2f use speed:%.2f MB/s time:%d ms ", filename, ratio, speed, time);
	return ret;
}
DexFile readerForContent;
bool MultiThreadSaveBigFile(const char* filename)
{
	DexFile bigFile;
	
	bool ret = bigFile.NewFile(filename);
	if (!ret)
	{
		pak_log("save big file error! create file:%s failed!\n", filename);
		return false;
	}
	typedef map<DexMd5, DexBigFileHandle::stDexBigFileContent*> mapContent;
	mapContent contents;
	DexBigFileHandle::stDexBigFileHeader header;
	DUInt32 iBlockSize = DexBigFileHandle::s_iBlockSize;
	vector<DexBigFileHandle::stDexBigFileBlock> vecBlocks;
	char* readCache = new char[iBlockSize];
	char* writeCache = new char[iBlockSize];

	header.count = g_vecFileName.size();
	bigFile.Write(&header, sizeof(header));

	DUInt32 iContentByte = sizeof(DexBigFileHandle::stDexBigFileContent);
	uint64 time = GetTickCount64();
	//write content file data
	DUInt32 destSize = 0;
	float ratio = 0.0f;
	DUInt32 startIndex = 0;
	uint64 fileOffset = 0;
	uint64 fileSize = 0;
	DUInt32 fileBlockCount = 0;
	DUInt32 tailByte = 0;
	DUInt32 writeLength = 0;
	DUInt32 BigFileBlockCount = 0;
	DUInt32 zlibSrcLength = 0;
	uint64 bigFileCompressLength = 0;
	uint64 bigFileSrcLength = 0;
	DexBigFileHandle::stDexBigFileBlock block;
	bool		 bCompress = false;
	int          compressRet = 0;
	for (vector<string>::iterator nameIte = g_vecFileName.begin(); nameIte < g_vecFileName.end(); ++nameIte)
	{
		const char* filename = (*nameIte).c_str();
		string  md5Filename = filename;
		bool dealFile = dealFileName(md5Filename);
		if (!dealFile)
			continue;
		DexMd5 md5 = DexMd5Generator::sGetDexMd5Generator()->GenerateMd5(md5Filename.c_str());
		WaitForSingleObject(g_readEvent, INFINITE); //���Ƶ�һ���ļ�������֮���ٴ�����һ���ļ�
		ret = readerForContent.OpenFile(filename);
		if (ret)
		{
			fileOffset = 0;
			fileSize = readerForContent.Length();
			fileBlockCount = fileSize / iBlockSize;
			tailByte = fileSize % iBlockSize;
			if (tailByte != 0)
				fileBlockCount++;
			DexBigFileHandle::stDexBigFileContent* newContent = new DexBigFileHandle::stDexBigFileContent;
			newContent->md5Key = md5;
			newContent->iBlockIndex = BigFileBlockCount;
			newContent->iBlockCount = fileBlockCount;
			newContent->iOrigionSize = fileSize;
			contents[md5] = newContent;
			for (DUInt32 b = 0; b < fileBlockCount; ++b)
			{
				writeLength = iBlockSize;
				if (b == fileBlockCount - 1 && tailByte != 0)
					//���һ������ && ����iBlockSize
					zlibSrcLength = tailByte;
				else
					zlibSrcLength = iBlockSize;
				CPackTaskFlow* pTaskFlow = g_taskPool.applyObject("�½�����");
				pTaskFlow->m_pWriter = &bigFile;
				pTaskFlow->m_pReader = &readerForContent;
				pTaskFlow->m_iOffset = fileOffset;
				pTaskFlow->m_iCount = zlibSrcLength;
				pTaskFlow->m_iDestSize = iBlockSize;
				pTaskFlow->m_fileName = md5Filename;
				g_taskManager.AddReadTask(pTaskFlow);
				fileOffset += zlibSrcLength;
			}
			bigFileSrcLength += fileSize;
			BigFileBlockCount += fileBlockCount;
			//readerForContent.Close();
		}
		else
		{//log file open error
		}
	}
	WaitForSingleObject(g_pakOver, INFINITE);
	//write content
	for (mapContent::iterator ite = contents.begin(); ite != contents.end(); ++ite)
	{
		DexBigFileHandle::stDexBigFileContent* pContent = ite->second;
		bigFile.Write(pContent, sizeof(DexBigFileHandle::stDexBigFileContent));
	}
	//write block info
	DUInt32 blockCount = g_taskManager.vecBlocks.size();
	if (blockCount != 0)
		bigFile.Write(&g_taskManager.vecBlocks[0], blockCount * sizeof(g_taskManager.vecBlocks[0]));
	//д��block����
	header.blockCount = blockCount;
	bigFile.Seek(DexFile::DSEEK_SET, 0);
	bigFile.Write(&header, sizeof(header));
	uint64 resLength = bigFile.ReCalLength();
	bigFile.Close();
	ratio = (float)bigFileCompressLength / bigFileSrcLength;
	time = GetTickCount64() - time;
	delete[] readCache;
	delete[] writeCache;
	float speed = bigFileSrcLength / 1024.0f / 1024 / time * 1000;
	//����ĸ�ʽ������.f�������������������%f %d %f����ڶ���f�������ȷ����֪Ϊ��
	pak_log("%s compress complete, ratio:%.2f speed:%.2f MB/s use time:%d ms ", filename, ratio, speed, time);
	return ret;
}
/*���̴߳����Ч�������죬����bug��Ŀǰ�õ��̴߳��*/
int main(int argc, char* argv[])
{
	DexMd5Generator::sCreateDexMd5Generator();

	TiXmlDocument xml;
	pak_log("����xml...\n");
	if (argc == 1)
	{//�Ӵ���������
		if (!xml.LoadFile("build.xml"))
		{
			cout << "open xml error" << endl;
			return 0;
		}
	}
	else if (argc == 2)
	{//������������
		if (!xml.LoadFile((const char*)argv[1]))
		{
			cout << "open xml error" << endl;
			return 0;
		}
	}
	TiXmlNode* pXmlNode = xml.FirstChild();
	const char* pakName = nullptr;
	bool       bMultiThreadPack = 0;
	while (pXmlNode != nullptr)
	{
		if (pXmlNode->Type() == TiXmlNode::TINYXML_DECLARATION)
		{//do nothing
		}
		else if (pXmlNode->Type() == TiXmlNode::TINYXML_ELEMENT)
		{
			TiXmlElement* pXmlElement = pXmlNode->ToElement();
			if (strcmp(pXmlNode->Value(), "output") == 0)
				pakName = pXmlElement->GetText();
			else if (strcmp(pXmlNode->Value(), "log") == 0)
				strcpy_s(log_filename, pXmlElement->GetText());
			else if (strcmp(pXmlNode->Value(), "prefix") == 0)
				strcpy_s(prefix, pXmlElement->GetText());
			else if (strcmp(pXmlNode->Value(), "filter") == 0)
				vec_filters.push_back(pXmlElement->GetText());
			else if (strcmp(pXmlNode->Value(), "directory") == 0)
				addDirectoryFiles(pXmlElement->GetText());
			else if (strcmp(pXmlNode->Value(), "file") == 0)
				g_vecFileName.push_back(pXmlElement->GetText());
		}
		pXmlNode = pXmlNode->NextSibling();
	}
	pak_log("����xml���\n");
	logFile.NewFile(log_filename);
	if (bMultiThreadPack)
	{
		g_readEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
		g_pakOver = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_taskManager.m_pTaskPool = &g_taskPool;
		g_taskManager.m_eventRead = &g_readEvent;
		g_taskManager.m_eventPackOver = &g_pakOver;
		HANDLE handle;
		handle = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
		CloseHandle(handle);
		handle = CreateThread(NULL, 0, CompressThread, NULL, 0, NULL);
		CloseHandle(handle); 
		handle = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
		CloseHandle(handle);
		handle = CreateThread(NULL, 0, CheckThread, NULL, 0, NULL);
		CloseHandle(handle);
		MultiThreadSaveBigFile(pakName);
	}
	else
		SaveBigFile(pakName);
	logFile.Close();
	return 0;

}

