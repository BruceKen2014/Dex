

/*
Dex引擎对自定义结构进行序列化
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
	bool    m_mode;    //0:读入数据 1：输出数据
	bool    m_bMemory; //true:DexMem自己申请内存 自己释放 false:内存来源于外部，DexMem只负责中间处理
	char*   m_data;
	DUInt64  m_iMemorySize; //内存空间大小
	DUInt64  m_length;   //内存实际使用的空间大小，如申请了一块10M的内存，但只向这个内存写入了1M的数据，
	                      //则m_iMemorySize= 10M，m_length= 1M
	DUInt64  m_curr;     //内存指针位置，用于读写时的定位

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
	void  GetData(int offset, int size, DexMem& _out); //从指定偏移处取出指定长度的数据到_out中
	int   GetLength()	{return (int)m_length;}
	void BeginWrite();
	void BeginRead();
	bool End();
	void IniFromBuff(char* buff, int length, bool bSelfMemory=true);
	bool IniFromFile(const char* filename); //加载整个文件
	void SaveToFile(const char* filename);
	void AddBuffToEnd(char* buff, int length); //追加buff内容
	void WriteToBuff(char* buff);	
	void WriteString(const char* ch);
	void WriteString(DString str);
	void WriteLine(const char* str);
	void WriteLine(DString str);
	void ReadString(char* ch);

	//这里模板函数的实现不能放在cpp文件中，如此便不利于隐藏细节
	//如果不用模板的话，可能要重载多个Write函数
	//如果日后要发布给他人使用，可考虑重载的方法
	//TODO:可以在输入输出时进行一些特殊处理，已达到加密的目的
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
	void ReadLine(char* _Out);//从data读取一行数据到_Out中，遇见换行停止
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
注意：
1、对于纯粹的struct 即结构体中所有数据都需要archive的struct,无需进行重载<< >>
   直接调用mem<<param 或 mem<<param 即可，因为<< >>会直接对内存进行操作
2、不能对char*类型进行<< >> 操作 因为sizeof去的结构为4 是一个指针的大小，从而字符串操作
   会失败，必须用ReadString WriteString
*/

#endif