/*************************************************************
****DemoEx�����ļ�д����CFile.h
****��飺��Ҫ�������ļ���д������
****���ߣ�������(BruceKen)
****���ڣ�2012-11-1
*************************************************************/
#ifndef _CFILE_H
#define _CFILE_H

#include "DexType.h"

class CFile
{
protected:
	HANDLE  m_file;
public:
	CFile();
	virtual ~CFile();
	virtual bool Check();

public:
	virtual bool CreateCFile(char* filename);    //��OpenCFile����������ͬ����ע�ش���
	virtual bool OpenCFile(char * filename);	 //��CreateCFile����������ͬ��ע�ش�
	virtual void GoToEnd();                      //�������ļ������������ʱ���ȶ�λ���ļ�β��
	virtual void Close();                        //�رյ�ǰ�ļ�

	virtual void InBlankLine(int count = 1);             //���ļ�д�����,������,Ĭ�ϻ�1��
	virtual void InBlankChar(int count = 1);             //���ļ�д����ַ���Ĭ��д��һ�����ַ�
	virtual void InNumber(int integer);                  //д��һ��int����
	virtual void InNumber(long _long);                   //д��һ��long����
	virtual void InNumber(int64 llong);                  //д��һ��int64����
	virtual void InNumber(float f, int precise = 6);     //д��һ��������, ���渡����漸λС��
	virtual void InString(char * word, ...);             //д���ַ���
};

CFile* getFileHandler();


#endif