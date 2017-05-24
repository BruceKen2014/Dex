
#include "DexMem.h"
#include "DexStreamFile.h"
#include "DexDefine.h"
#include "DexFile.h"
#pragma warning(disable:4996) 
DexMem::DexMem()
{
	m_mode = 0;
	m_bMemory = true;
	m_iMemorySize = 0;
	m_curr = 0;
	m_length = 0;
	m_data = DexNull;

}
DexMem::DexMem(const char* filename)
{
	m_mode = 0;
	m_bMemory = true;
	m_iMemorySize = 0;
	m_curr = 0;
	m_length = 0;
	m_data = DexNull;
	IniFromFile(filename);
}
DexMem::~DexMem()
{
	Reset();
}

void DexMem::Reset()
{
	if (m_bMemory && m_data != DexNull)
		free(m_data);
	m_data = DexNull;
	m_mode = 0;
	m_bMemory = true;
	m_iMemorySize = 0;
	m_curr = 0;
	m_length = 0;
}

void DexMem::SetMemoryFlag(bool selfMemory /* = true */, uint32 size /* = 0 */, void* buffer /* = DexNull */)
{
	m_bMemory = selfMemory;
	m_iMemorySize = size;
	m_length = size;
	if (selfMemory)
	{
		m_data = (char*)malloc(sizeof(char)*size);
		memset(m_data, 0, sizeof(char)*size);
	}
	else
	{
		m_data = (char*)buffer; 
	}
}
void DexMem::BeginWrite()
{
	memset(m_data, 0, m_iMemorySize);
	m_curr = 0;
	m_length = 0;
	m_mode = MEM_WRITE;
}
void DexMem::BeginRead()
{
	m_curr = 0;
	m_mode = MEM_READ;
}

bool DexMem::End()
{
	return m_curr >= m_length;
}
void DexMem::IniFromBuff(char* buff, int length, bool bSelfMemory)
{
	Reset();
	m_curr = 0;
	m_length = 0;
	m_iMemorySize = length;
	if (bSelfMemory)
	{
		m_data = (char*)malloc(sizeof(char)* length);
		memcpy(m_data, buff, length);
	}
	else
		m_data = buff;
	m_bMemory = bSelfMemory;
}

void DexMem::GetData(int offset, int size, DexMem& _out)
{
	_out.IniFromBuff(&m_data[offset], size, true);
}
bool DexMem::IniFromFile(const char* filename)
{
	Reset();
	bool ret = DexStreamFile::sGetStreamFile()->OpenFile(filename);
	DEX_ENSURE_B(ret);
	uint64 fileSize = DexStreamFile::sGetStreamFile()->FileSize();
	DEX_ENSURE_B(fileSize != 0);
	m_bMemory = true;
	m_data = (char*)malloc(sizeof(char)* fileSize);
	m_curr = 0;
	m_iMemorySize = fileSize;
	m_length = fileSize;
	DexStreamFile::sGetStreamFile()->ReadData(m_data);
	return true;
}

void DexMem::SaveToFile(const char* filename)
{
	DexFile file;
	bool ret = file.NewFile(filename);
	DEX_ENSURE(ret);
	file.Write(m_data, m_length);
}
void DexMem::AddBuffToEnd(char* buff, int length)
{//�����Ѿ�������BeginWrite
	//cout<<"�򻺳���׷��buff��׷�ӳ���"<<length<<endl;
	memcpy(&m_data[m_curr], buff, length);
	m_length += length;
	m_curr += length;
	//cout<<"׷�����!"<<endl;
}

void DexMem::Write(void* _In, int length)
{
	memcpy(&m_data[m_curr], _In, length);
	m_length += length;
	m_curr += length;
}
void DexMem::Read(void* _Out, int length)
{
	memcpy(_Out, &m_data[m_curr], length);
	m_curr += length;
}
void DexMem::ReadLine(char* _Out)
{
	while (m_curr < m_length)
	{
		if (m_data[m_curr] == '\n' || m_data[m_curr] == '\r')
		{
			if (++m_curr < m_length)
			{
				if (m_data[m_curr] == '\n' || m_data[m_curr] == '\r')
				{
					m_curr++;
				}
			}
			break;
		}
		else
		{
			*_Out++ = m_data[m_curr++];
		}
	}
	*_Out = '\0';
	/*
	while (m_curr < m_length)
	{
		if (m_data[m_curr] != '\n' && m_data[m_curr] != '\r')
			*_Out++ = m_data[m_curr++];
		else
		{
			m_curr++;
			break;
		}
	}
	*_Out = '\0';
	*/
}
void DexMem::WriteToBuff(char* buff)
{
	BeginRead();
	memcpy(buff, m_data, m_length);
}

void DexMem::WriteString(char* ch)
{
	//cout<<"WriteStringд�룺"<<ch<<endl;

	strcpy(&m_data[m_curr], ch);
	//��������chԴ��С��&m_data[m_curr],���Բ��Ὣ\0����ȥ��
	
	m_curr += strlen(ch);
	m_data[m_curr++] = '\0'; //�����ַ����ָ�������������д���ַ����Ļ����Ὣ�����ִ�����һ��
	
	m_length += strlen(ch) + 1;
	//cout<<"WriteStringд����ϣ�"<<endl;
}

void DexMem::ReadString(char* ch)
{
	//cout<<"ReadString������..."<<endl;

	strcpy(ch, &m_data[m_curr]); //��ʱch�Ĵ�Сһ������m_data[m_curr],�ڿ�����ʱ�����������ֶ�
	//�����\0,��ֹͣ����
	m_curr += strlen(ch) + 1; //Ҫ������һ��\0

	//cout<<"ReadString������ϣ�ֵΪ��"<<ch<<endl;
}
