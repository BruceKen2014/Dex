

#include "DexStreamFile.h"
#include "DexBigFileHandle.h"
#include "DexFile.h"
#include "DexLog.h"


DexStreamFile* DexStreamFile::s_pStreamFile = nullptr;

DexStreamFile* DexStreamFile::sCreateStreamFile()
{
	if (s_pStreamFile == nullptr)
		s_pStreamFile = new DexStreamFile;
	return s_pStreamFile;
}
DexStreamFile* DexStreamFile::sGetStreamFile()
{
	return s_pStreamFile;
}
DexStreamFile::DexStreamFile()
{
	m_pDexFile = new DexFile;
	m_pBigFileHandler = nullptr;
	for (int i = 0; i < s_iBigFileCount; ++i)
		m_arrBigFile[i] = new DexBigFileHandle;
}
DexStreamFile::~DexStreamFile()
{
	for (int i = 0; i < s_iBigFileCount; ++i)
		delete m_arrBigFile[i];
	if (m_pDexFile != nullptr)
		delete m_pDexFile;
}

bool DexStreamFile::AddBigFile(const char* filename)
{
	bool ret = true;
	for (int i = 0; i < s_iBigFileCount; ++i)
	{
		if (m_arrBigFile[i]->ContentCount() == 0)
		{
			ret = m_arrBigFile[i]->InitBigFile(filename);
			break;
		}
	}
	if(ret)
	{//log init big file successfully
	}
	else
	{//log init big file failed
	}
	return ret;
}
DUInt64 DexStreamFile::ReadData(void* buffer)
{
	DUInt64 ret = 0;
#ifdef USE_BIG_FILE
	if (m_pBigFileHandler != nullptr)
	{
		ret = m_pBigFileHandler->Read(buffer);
		return ret;
	}
#endif
	ret = m_pDexFile->Read(buffer, 0, -1);
	return ret;
}

bool DexStreamFile::OpenFile(const char* filename)
{
#ifdef USE_BIG_FILE
	//first check if curr handler can find file
	if(m_pBigFileHandler != nullptr && m_pBigFileHandler->OpenFile(filename))
	{
		//log big file find filename
		DexLog::getSingleton()->LogLine(log_ok, "BigFile:open %s success!", filename);
		return true;
	}
	else
	{
		for(int i = 0; i < s_iBigFileCount; ++i)
		{
			bool find = m_arrBigFile[i]->OpenFile(filename);
			if (find)
			{
				m_pBigFileHandler = m_arrBigFile[i];
				DexLog::getSingleton()->LogLine(log_ok, "BigFile:open %s success!", filename);
				//log big file find filename
				return true;
			}
		}
	}
	DexLog::getSingleton()->LogLine(log_allert, "BigFile:open %s failed! try to open small file...", filename);
	m_pBigFileHandler = DexNull;
	//log big file not find file, try to find small file
#endif
	bool find = m_pDexFile->OpenFile(filename);
	if (find)
	{//log smal file find filename
		DexLog::getSingleton()->LogLine(log_ok, "SmallFile:open %s success!", filename);
	}
	else
	{//log small file not find filename
		DexLog::getSingleton()->LogLine(log_error, "SmallFile:open %s failed!", filename);
	}
	return find;
}

DUInt64 DexStreamFile::FileSize()
{
	DUInt64 ret = 0;
#ifdef USE_BIG_FILE
	if (m_pBigFileHandler != nullptr)
	{
		ret = m_pBigFileHandler->FileSize();
		return ret;
	}
#endif
	ret = m_pDexFile->Length();
	return ret;
}
void DexStreamFile::CloseFile()
{
#ifdef USE_BIG_FILE
	if(m_pBigFileHandler != nullptr)
	{
		m_pBigFileHandler->CloseFile();
		return;
	}
#endif
	if(m_pDexFile != nullptr)
		m_pDexFile->Close();
}