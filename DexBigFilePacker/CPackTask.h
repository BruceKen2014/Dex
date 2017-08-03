

#pragma once
#include <deque>
#include <windows.h>
#include <iostream>
#include "..\DexBase\DexBigFileHandle.h"
typedef unsigned long long uint64;
typedef long long DInt64;
typedef unsigned int DUDInt32;
typedef int DInt32;
const int g_iBuffSize = 1024 * 64;
using namespace std;
class CPackTask
{
public:
	bool  m_bUse;
	CPackTask();
public:
	virtual bool Exe() = 0;
};

class DexFile;
class CPackTaskRead :CPackTask
{
public:
	DexFile* m_pReader;
	uint64   m_iOffset;
	DInt64    m_iCount;
	char*    m_pBuffer;
public:
	CPackTaskRead();
	void Initialize(DexFile* pFile, uint64 iOffset, DInt64 iCount, char*pBuffer);
	virtual bool Exe();
};
class CPackTaskCompress :CPackTask
{
public:
	char*  m_pSrcBuffer;
	char*  m_pDestBuffer;
	DUDInt32 m_iSrcSize;
	DUDInt32 m_iDestSize;
	DUDInt32 m_iRetSize;
	bool   m_bSuccess;
public:
	CPackTaskCompress();
	void Initialize(char* pDestBuffer, DUDInt32 iDestSize, char* pSrcBuffer, DUDInt32 iSrcSize);
	virtual bool Exe();
};
class CPackTaskWrite :CPackTask
{
public:
	DexFile* m_pFile;
	char*    m_pWriteBuffer;
	uint64   m_iWriteSize;
public:
	CPackTaskWrite();
	void Initialize(DexFile* pFile, char* pWriteBuffer, uint64 iWriteSize);
	virtual bool Exe();
};

class CPackTaskFlow
{
public:
	DexFile* m_pReader;
	DexFile* m_pWriter;
	char m_pReaderBuffer[g_iBuffSize];
	char m_pWriteBuffer[g_iBuffSize];

	//for read
	uint64   m_iOffset; //read offset
	DInt64    m_iCount;  //read count
	//for compress
	DUDInt32 m_iSrcSize;  
	DUDInt32 m_iDestSize;
	bool   m_bSuccess;
	//
	string m_fileName;
public:
	CPackTaskRead m_read;
	CPackTaskCompress m_compress;
	CPackTaskWrite m_write;
public:
	void Read();
	void Compress();
	void Write();
};
template<typename T>
class CObjectPool
{
public:
	CObjectPool(int count=0) :m_iObjectTotal(count), m_iObjectUse(0)
	{
		m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_eventMutex = CreateEvent(NULL, FALSE, TRUE, NULL);
		if (count != 0)
		{
			m_pObject = new T[m_iObjectTotal];
			for (int i = 0; i < m_iObjectTotal; ++i)
				m_queue.push_front(&m_pObject[i]);
			SetEvent(m_event);
		}
	}
	~CObjectPool()
	{
		delete[] m_pObject;
	}
public:
	void Init(int count)
	{
		m_iObjectTotal= count;
		m_pObject = new T[m_iObjectTotal];
		for (int i = 0; i < m_iObjectTotal; ++i)
			m_queue.push_front(&m_pObject[i]);
	}
	T*   applyObject(const char* log)
	{
		WaitForSingleObject(m_event, INFINITE); //��֤��Դ����������Դ
		WaitForSingleObject(m_eventMutex, INFINITE); //��֤��returnObject�Ļ���
		//pak_log("ִ�� %s \n", log);
		if (m_queue.size() == 0)
			int tt = 0;
		T* ret = m_queue.back();
		m_queue.pop_back();
		m_iObjectUse++;
		if (m_queue.size() == 0)
		{
			//pak_log("		%s pool is empty. lock!\n", log);
			ResetEvent(m_event);//��Դ������Դ�����߳���ס
		}
		else
			SetEvent(m_event);  //����Դ������
		SetEvent(m_eventMutex); //�ͷź�returnObject�Ļ���
		return ret;
	}
	void returnObject(T* value, const char* log)
	{
		WaitForSingleObject(m_eventMutex, INFINITE); //��֤��applyObject�Ļ���
		//pak_log("���� %s \n", log);
		m_queue.push_front(value);
		SetEvent(m_eventMutex); //�ͷź�applyObject�Ļ���
		SetEvent(m_event);
	}
public:
	int  m_iObjectTotal;
	int  m_iObjectUse;
	HANDLE m_event;  //��������Դ������Ķ�������ʱ���̹߳���
	HANDLE m_eventMutex; //����apply ��return�Ļ������
	T*   m_pObject;
	std::deque<T*> m_queue;
};
class CPackTaskManager
{
public:
	CPackTaskManager();
	~CPackTaskManager();
public:
	typedef CObjectPool<CPackTaskFlow> TDeuqeTaskFlow;
	TDeuqeTaskFlow m_taskRead;
	TDeuqeTaskFlow m_taskCompress;
	TDeuqeTaskFlow m_taskWrite;
	TDeuqeTaskFlow* m_pTaskPool;
	HANDLE*  m_eventRead;
	HANDLE*  m_eventPackOver;
	vector<DexBigFileHandle::stDexBigFileBlock> vecBlocks;
public:
	void RunRead();
	void RunCompress();
	void RunWrite();
	void OnCompleteTask(CPackTaskFlow* pFlow);
public:
	void AddReadTask(CPackTaskFlow* pTaskFlow);
	void AddCompressTask(CPackTaskFlow* pTaskFlow);
	void AddWriteTask(CPackTaskFlow* pTaskFlow);

};
