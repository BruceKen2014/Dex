

/*
Dex引擎对自定义结构进行序列化
*/
#ifndef _DEX_MEM_H
#define _DEX_MEM_H
#include <string>


//默认提供的内存大小为10M
#define  MAX_BUFF (10 * 1024*1024)
class DexMem
{
public:
	enum
	{
		MEM_READ = 0,
		MEM_WRITE = 1
	};
protected:
	bool    m_mode; //0:读入数据 1：输出数据
	char*   m_data;
	size_t     m_length;
	size_t     m_curr;

public:
	DexMem();
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
	char* GetData()		{return m_data;}
	void  GetData(int offset, int size, DexMem& _out); //从指定偏移处取出指定长度的数据到_out中
	int   GetLength()	{return (int)m_length;}
	void BeginWrite();
	void BeginRead();
	bool End();
	void IniFromBuff(char* buff, int length);
	bool IniFromFile(const char* filename); //加载整个文件
	bool IniFromFile(const char* filename, int offset, int size); //加载一个文件中的部分数据
	void SaveToFile(const char* filename);
	void AddBuffToEnd(char* buff, int length); //追加buff内容
	void WriteToBuff(char* buff);	
	void WriteString(char* ch);
	void ReadString(char* ch);

	//这里模板函数的实现不能放在cpp文件中，如此便不利于隐藏细节
	//如果不用模板的话，可能要重载多个Write函数
	//如果日后要发布给他人使用，可考虑重载的方法
	//TODO:可以在输入输出时进行一些特殊处理，已达到加密的目的
	template<typename T>
	void Write(const T& _In)
	{
		memcpy(&m_data[m_curr], &_In, sizeof(T));
		m_length += sizeof(T);
		m_curr += sizeof(T);
	}
	template<typename T>
	void Read(T& _Out)
	{
		memcpy((void*)&_Out, &m_data[m_curr], sizeof(T));
		m_curr += sizeof(T);
	}
	template<typename T>
	friend DexMem& operator << (DexMem& mem, const T& _In)
	{
		mem.Write(_In);
		return mem;
	}
	template<typename T>
	friend DexMem& operator >> (DexMem& mem, T& _Out)
	{
		mem.Read(_Out);
		return mem;
	}

};
/*
注意：
1、对于纯粹的struct 即结构体中所有数据都需要archive的struct,无需进行重载<< >>
   直接调用mem<<param 或 mem<<param 即可，因为<< >>会直接对内存进行操作
2、不能对char*类型进行<< >> 操作 因为sizeof去的结构为4 是一个指针的大小，从而字符串操作
   会失败，必须用ReadString WriteString
3、不能对string进行<< >> 也不能用ReadString WriteString，因为string的sizeof会随着string
   当前的字节数而改变
*/

#endif