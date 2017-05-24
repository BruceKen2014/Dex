

#include "DexFile.h"


#define MAX_LINE_BYTE  2048
DexFile::DexFile()
{
	m_pFileHandle = nullptr;
	m_iLength = 0;
}

DexFile::~DexFile()
{
	Close();
}

bool DexFile::OpenFile(const char* filename)
{
	Close();
	m_pFileHandle = fopen(filename, "rb");
	if (m_pFileHandle == nullptr)
		return false;
	fseek(m_pFileHandle, 0, SEEK_END);
	m_iLength = ftell(m_pFileHandle);
	fseek(m_pFileHandle, 0, SEEK_SET);
	return true;
}

void DexFile::Close()
{
	m_iLength = 0;
	if (m_pFileHandle != nullptr)
		fclose(m_pFileHandle);
}

uint64 DexFile::Tell()
{
	uint64 ret = 0;
	if (m_pFileHandle == nullptr)
		return ret;
	ret = ftell(m_pFileHandle);
	return ret;
}

uint64 DexFile::Length()
{
	return m_iLength;
}

uint64 DexFile::ReCalLength()
{
	if (m_pFileHandle == nullptr)
		return 0;
	uint32 curPos = ftell(m_pFileHandle);
	fseek(m_pFileHandle, 0, SEEK_END);
	m_iLength = ftell(m_pFileHandle);
	fseek(m_pFileHandle, curPos, SEEK_SET);
	return m_iLength;
}

uint64 DexFile::Read(void* buffer, uint64 offset, int64 count)
{
	uint64 readOffset = offset > m_iLength ? m_iLength : offset;
	long long readCount = count == -1 ? m_iLength : count;
	readCount = readCount > m_iLength ? m_iLength : readCount;
	if (readOffset + readCount > m_iLength)
	{
		readCount = m_iLength - readOffset;
	}
	fseek(m_pFileHandle, readOffset, SEEK_SET);
	uint64 pos = ftell(m_pFileHandle);
	fread(buffer, 1, readCount, m_pFileHandle);
	return readCount;
}

bool DexFile::ReadLine(char* buffer)
{
	void* ret = fgets(buffer, MAX_LINE_BYTE, m_pFileHandle);
	return ret != nullptr;
}

bool DexFile::NewFile(const char* filename)
{
	Close();
	m_pFileHandle = fopen(filename, "wb+");
	if (m_pFileHandle == nullptr)
		return false;
	return true;
}

void DexFile::Write(void* buffer, uint64 count)
{
	if (m_pFileHandle == nullptr)
		return;
	fwrite(buffer, 1, count, m_pFileHandle);
}

bool DexFile::Seek(DexFile_Seek seek, int offset)
{
	if (m_pFileHandle == nullptr)
		return false;
	int win_seek = SEEK_CUR;
	switch (seek)
	{
	case DSEEK_CUR:
		win_seek = SEEK_CUR;
		break;
	case DSEEK_SET:
		win_seek = SEEK_SET;
		break;
	case DSEEK_END:
		win_seek = SEEK_END;
		break;
	default:
		win_seek = SEEK_CUR;
		break;
	}
	fseek(m_pFileHandle, offset, win_seek);
	return true;
}
