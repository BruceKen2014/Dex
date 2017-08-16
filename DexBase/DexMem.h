

/*
Dex������Զ���ṹ�������л�
*/
#ifndef _DEX_MEM_H
#define _DEX_MEM_H
#include "DexString.h"
#include "DexType.h"

class DexMem
{
public:
	enum
	{
		MEM_READ = 0,
		MEM_WRITE = 1
	};
protected:
	bool    m_mode;    //0:�������� 1���������
	bool    m_bMemory; //true:DexMem�Լ������ڴ� �Լ��ͷ� false:�ڴ���Դ���ⲿ��DexMemֻ�����м䴦��
	char*   m_data;
	DUInt64  m_iMemorySize; //�ڴ�ռ��С
	DUInt64  m_length;   //�ڴ�ʵ��ʹ�õĿռ��С����������һ��10M���ڴ棬��ֻ������ڴ�д����1M�����ݣ�
	                      //��m_iMemorySize= 10M��m_length= 1M
	DUInt64  m_curr;     //�ڴ�ָ��λ�ã����ڶ�дʱ�Ķ�λ

public:
	DexMem();
	DexMem(const char* fileName);
	~DexMem();
	void Reset();
public:
	
	//friend DexMem& operator << (bool args);
	//friend DexMem& operator << (int args);
	//friend DexMem& operator << (short args);
	//friend DexMem& operator << (char args);
	//friend DexMem& operator << (string args);
	//friend DexMem& operator >> (bool args);
	//friend DexMem& operator >> (int args);
	//friend DexMem& operator >> (short args);
	//friend DexMem& operator >> (char args);
	//friend DexMem& operator >> (string args);
	void SetMemoryFlag(bool selfMemory = true, DUInt32 size = 0, void* buffer = DexNull);
	void* GetData()		{return m_data;}
	void  GetData(int offset, int size, DexMem& _out); //��ָ��ƫ�ƴ�ȡ��ָ�����ȵ����ݵ�_out��
	int   GetLength()	{return (int)m_length;}
	void BeginWrite();
	void BeginRead();
	bool End();
	void IniFromBuff(char* buff, int length, bool bSelfMemory=true);
	bool IniFromFile(const char* filename); //���������ļ�
	void SaveToFile(const char* filename);
	void AddBuffToEnd(char* buff, int length); //׷��buff����
	void WriteToBuff(char* buff);	
	void WriteString(const char* ch);
	void WriteString(DString str);
	void WriteLine(const char* str);
	void WriteLine(DString str);
	void ReadString(char* ch);

	//����ģ�庯����ʵ�ֲ��ܷ���cpp�ļ��У���˱㲻��������ϸ��
	//�������ģ��Ļ�������Ҫ���ض��Write����
	//����պ�Ҫ����������ʹ�ã��ɿ������صķ���
	//TODO:�������������ʱ����һЩ���⴦���Ѵﵽ���ܵ�Ŀ��
	template<typename T>
	void Write(const T& _In)
	{
		size_t Tsize = sizeof(T);
		memcpy(&m_data[m_curr], &_In, Tsize);
		m_length += Tsize;
		m_curr += Tsize;
	}
	void Write(void* _In, int length);
	template<typename T>
	void Read(T& _Out)
	{
		size_t Tsize = sizeof(T);
		memcpy((void*)&_Out, &m_data[m_curr], Tsize);
		m_curr += Tsize;
	}
	void Read(void* _Out, int length);
	void ReadLine(char* _Out);//��data��ȡһ�����ݵ�_Out�У���������ֹͣ
	template<typename T>
	DexMem& operator << (const T& _In)
	{
		Write(_In);
		return *this;
	}
	template<typename T>
	DexMem& operator >> (T& _Out)
	{
		Read(_Out);
		return *this;
	}

};
/*
ע�⣺
1�����ڴ����struct ���ṹ�����������ݶ���Ҫarchive��struct,�����������<< >>
   ֱ�ӵ���mem<<param �� mem<<param ���ɣ���Ϊ<< >>��ֱ�Ӷ��ڴ���в���
2�����ܶ�char*���ͽ���<< >> ���� ��Ϊsizeofȥ�ĽṹΪ4 ��һ��ָ��Ĵ�С���Ӷ��ַ�������
   ��ʧ�ܣ�������ReadString WriteString
*/

#endif