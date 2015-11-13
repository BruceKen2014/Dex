
#include "DexMem.h"
#pragma warning(disable:4996) 
DexMem::DexMem()
{
	Reset();

}
DexMem::~DexMem()
{
	free(m_data);
}

void DexMem::Reset()
{
	m_mode = 0;
	m_data = (char*)malloc(sizeof(char)*MAX_BUFF);
	memset(m_data, 0, sizeof(char)*MAX_BUFF);
	m_curr = 0;
	m_length = 0;
}
void DexMem::BeginWrite()
{
	memset(m_data, 0, sizeof(char)*MAX_BUFF);
	m_curr = 0;
}
void DexMem::BeginRead()
{
	m_curr = 0;
}

bool DexMem::End()
{
	return m_curr == m_length;
}
void DexMem::IniFromBuff(char* buff, int length)
{
	BeginWrite();
	memcpy(m_data, buff, length);
}

void DexMem::GetData(int offset, int size, DexMem& _out)
{
	if(offset + size > m_length)
		return;
	char ch[64]; //不能分配过大 否则会提示栈溢出的错误
	memset(ch, 0, 64);
	while(size > 0)
	{
		if(size > 64)
		{
			memcpy((void*)&ch, &m_data[offset], 64 * sizeof(char));
			_out.AddBuffToEnd(ch, 64);
		}
		else
		{
			memcpy((void*)&ch, &m_data[offset], size * sizeof(char));
			_out.AddBuffToEnd(ch, size);
		}
		memset(ch, 0, 64);
		offset += 64;
		size -= 64;
	}
}
bool DexMem::IniFromFile(const char* filename)
{
	BeginWrite();
	FILE* hFile = NULL;
	hFile = fopen(filename, "rb");
	if(hFile == NULL)
	{
		return false;
	}
	fseek(hFile, 0, SEEK_END);
	m_length = ftell(hFile);
	fseek(hFile, 0, SEEK_SET);
	fread(m_data, m_length, 1, hFile);
	fclose(hFile);
	return true;
}

bool DexMem::IniFromFile(const char* filename, int offset, int size)
{
	BeginWrite();
	FILE* hFile = NULL;
	hFile = fopen(filename, "rb");
	if(hFile == NULL)
	{
		return false;
	}
	m_length = size;
	//下一版本加入检错机制
	fseek(hFile, offset, SEEK_SET);
	fread(m_data, m_length, 1, hFile);
	fclose(hFile);
	return true;
}

void DexMem::SaveToFile(const char* filename)
{
	BeginRead();
	FILE* hFile = NULL;
	hFile = fopen(filename, "w+");
	if(hFile == NULL)
	{
		return;
	}
	fseek(hFile, 0, SEEK_SET);
	fwrite((const void*)m_data, m_length, 1, hFile);
	fclose(hFile);
}
void DexMem::AddBuffToEnd(char* buff, int length)
{//必须已经调用了BeginWrite
	//cout<<"向缓冲区追加buff：追加长度"<<length<<endl;
	memcpy(&m_data[m_curr], buff, length);
	m_length += length;
	m_curr += length;
	//cout<<"追加完成!"<<endl;
}
void DexMem::WriteToBuff(char* buff)
{
	BeginRead();
	memcpy(buff, m_data, m_length);
}

void DexMem::WriteString(char* ch)
{
	//cout<<"WriteString写入："<<ch<<endl;

	strcpy(&m_data[m_curr], ch);
	//这里由于ch源长小于&m_data[m_curr],所以不会将\0拷进去！
	
	m_curr += strlen(ch);
	m_data[m_curr++] = '\0'; //加入字符串分隔符，否则连续写入字符串的话，会将两个字串连在一起
	
	m_length += strlen(ch) + 1;
	//cout<<"WriteString写入完毕！"<<endl;
}

void DexMem::ReadString(char* ch)
{
	//cout<<"ReadString读出中..."<<endl;

	strcpy(ch, &m_data[m_curr]); //此时ch的大小一定大于m_data[m_curr],在拷贝的时候，遇到我们手动
	//加入的\0,会停止拷贝
	m_curr += strlen(ch) + 1; //要跳过上一个\0

	//cout<<"ReadString读出完毕！值为："<<ch<<endl;
}
