/*************************************************************
****DemoEx引擎文件写入类CFile.h
****简介：主要负责向文件中写入数据
****作者：阚正杰(BruceKen)
****日期：2012-11-1
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
	virtual bool CreateCFile(char* filename);    //与OpenCFile方法功能相同，但注重创建
	virtual bool OpenCFile(char * filename);	 //与CreateCFile方法功能相同，注重打开
	virtual void GoToEnd();                      //对已有文件进行数据添加时，先定位到文件尾部
	virtual void Close();                        //关闭当前文件

	virtual void InBlankLine(int count = 1);             //向文件写入空行,即换行,默认换1行
	virtual void InBlankChar(int count = 1);             //向文件写入空字符，默认写入一个空字符
	virtual void InNumber(int integer);                  //写入一个int整数
	virtual void InNumber(long _long);                   //写入一个long整数
	virtual void InNumber(int64 llong);                  //写入一个int64整数
	virtual void InNumber(float f, int precise = 6);     //写入一个浮点数, 保存浮点后面几位小数
	virtual void InString(char * word, ...);             //写入字符串
};

CFile* getFileHandler();


#endif