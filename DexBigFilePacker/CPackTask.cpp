
#include <memory.h>
#include "CPackTask.h"
#include "..\DexBase\DexFile.h"
#include "..\sdk\zlib\include\zlib.h"



extern void pak_log(const char* text, ...);
CPackTask::CPackTask()
{
}

CPackTaskRead::CPackTaskRead()
{
	m_pReader =  NULL;
	m_iOffset = 0;
	m_iCount = 0;
	m_pBuffer = NULL;
	m_bUse = true;
}
void CPackTaskRead::Initialize(DexFile* pFile, uint64 iOffset, DInt64 iCount, char*pBuffer)
{
	m_pReader = pFile;
	m_iOffset = iOffset;
	m_iCount = iCount;
	m_pBuffer = pBuffer;
	m_bUse = true;
}

bool CPackTaskRead::Exe()
{
	m_pReader->Read(m_pBuffer, m_iOffset, m_iCount);
	m_bUse = false;
	return true;
}
CPackTaskCompress::CPackTaskCompress()
{
	m_pDestBuffer = NULL;
	m_iDestSize = 0;
	m_pSrcBuffer = NULL;
	m_iSrcSize = 0;
	m_iRetSize = 0;
	m_bSuccess = false;
	m_bUse = false;
}
void CPackTaskCompress::Initialize(char* pDestBuffer, DUInt32 iDestSize, char* pSrcBuffer, DUInt32 iSrcSize)
{
	m_pDestBuffer = pDestBuffer;
	m_iDestSize = iDestSize;
	m_pSrcBuffer = pSrcBuffer;
	m_iSrcSize = iSrcSize;
	m_iRetSize = m_iDestSize;
	m_bSuccess = false;
	m_bUse = true;
}

bool CPackTaskCompress::Exe()
{
	int zRet = compress((Byte*)m_pDestBuffer, (uLongf*)&m_iDestSize, (Byte*)m_pSrcBuffer, m_iSrcSize);
	if (zRet == Z_OK)
	{
		m_iRetSize = m_iDestSize;
		m_bSuccess = true;
	}
	m_bUse = false;
	return true;
}

CPackTaskWrite::CPackTaskWrite()
{
	m_pFile = NULL;
	m_pWriteBuffer = NULL;
	m_iWriteSize = 0;
	m_bUse = false;
}

void CPackTaskWrite::Initialize(DexFile* pFile, char* pWriteBuffer, uint64 iWriteSize)
{
	m_pFile = pFile;
	m_pWriteBuffer = pWriteBuffer;
	m_iWriteSize = iWriteSize;
	m_bUse = true;
}

bool CPackTaskWrite::Exe()
{
	m_pFile->Write(m_pWriteBuffer, m_iWriteSize);
	m_bUse = false;
	return true;
}

void CPackTaskFlow::Read()
{
	m_iSrcSize = m_pReader->Read(m_pReaderBuffer, m_iOffset, m_iCount);
}
void CPackTaskFlow::Compress()
{
	int zRet = compress((Byte*)m_pWriteBuffer, (uLongf*)&m_iDestSize, (Byte*)m_pReaderBuffer, m_iSrcSize);
	if (zRet == Z_OK)
		m_bSuccess = true;
	else
		m_bSuccess = false;
}
void CPackTaskFlow::Write()
{
	if (m_bSuccess)
		m_pWriter->Write(m_pWriteBuffer, m_iDestSize);
	else
		m_pWriter->Write(m_pReaderBuffer, m_iSrcSize);
}
CPackTaskManager::CPackTaskManager()
{
}
CPackTaskManager::~CPackTaskManager()
{

}
CPackTaskFlow* pTestTask = NULL;
void CPackTaskManager::RunRead()
{
	CPackTaskFlow* pFlow = m_taskRead.applyObject("		读任务");
	pFlow->Read();
	m_taskCompress.returnObject(pFlow,"			压缩任务");
	pTestTask = pFlow;
}
void CPackTaskManager::RunCompress()
{
	CPackTaskFlow* pFlow = m_taskCompress.applyObject("			压缩任务");
	pFlow->Compress();
	DexBigFileHandle::stDexBigFileBlock block;
	block.bCompress = pFlow->m_bSuccess;
	if (block.bCompress)
		block.iBlockSize = pFlow->m_iDestSize;
	else
		block.iBlockSize = pFlow->m_iSrcSize;
	block.iByteOffset = pFlow->m_pWriter->Tell();
	vecBlocks.push_back(block);
	m_taskWrite.returnObject(pFlow,"				写任务");
}
void CPackTaskManager::RunWrite()
{
	if (m_taskRead.m_queue.size() == 0 &&
		m_taskCompress.m_queue.size() == 0 &&
		m_taskWrite.m_queue.size() == 0)
		SetEvent(m_eventPackOver);
	else
		ResetEvent(m_eventPackOver);
	CPackTaskFlow* pFlow = m_taskWrite.applyObject("				写任务");
	pFlow->Write();
	OnCompleteTask(pFlow);
}

void CPackTaskManager::OnCompleteTask(CPackTaskFlow* pFlow)
{
	if (pFlow->m_iCount != g_iBuffSize)
	{//set event to read next file
		pak_log("pack %s ok!\n", pFlow->m_fileName.c_str());
		SetEvent(*m_eventRead);
	}
	m_pTaskPool->returnObject(pFlow, "池中任务");
}
void CPackTaskManager::AddReadTask(CPackTaskFlow* pTaskFlow)
{
	m_taskRead.returnObject(pTaskFlow,"		读任务");
}