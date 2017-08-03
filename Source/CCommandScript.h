/*************************************************************
****DemoEx引擎空命令脚本类
****作者：阚正杰(BruceKen)
*************************************************************/							 



#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "../DexBase/typedefine.h"

#define            MAX_LINE_SIZE          4096
#define            MAX_COMMAND_SIZE       64
#define            MAX_PARAM_SIZE         1024

#define            SCRIPT_ERROR           0xFFFFFFFF

class CommandScript
{
protected:
	bool m_bUsing;
    char ** m_ppScript;     //指向当前脚本的指针，第一层指针指向当前所在行，第二层指针指向所在行的第几个字符
	int m_iScriptLine ;     //脚本总共的行数
	int m_iCurrScriptLine;  //脚本当前行
	int m_iCurrChar;        //当前行的当前第几个字符
	BOOL m_bPause ;         //脚本是否暂停  
	int m_iPauseEndTime ;   //脚本暂停时间，比如脚本先控制一个sprite向上移动10个像素，然后pause 100 ms,那么sprite会等100ms后再执行下一个动作
	                        //在此期间脚本的所在的当前行数不做改变

public:
    CommandScript();
    ~CommandScript();
	virtual BOOL OpenScript(const char * filename);
	virtual void CloseScript();	              //关闭当前文件

	bool   GetUsing()				  { return m_bUsing;};
	char** GetScript()				  { return m_ppScript ;};
	int    GetScriptLine()		      { return m_iScriptLine;};
	int    GetCurrLine()              { return m_iCurrScriptLine;};
	int    GetCurrChar()              { return m_iCurrChar;};	 
	void   SetCurrLine( int i = 0)	  { m_iCurrScriptLine = i;};		   //转到第i行，默认为0
	void   SetCurrChar(int i = 0)     { m_iCurrChar = i;};			       //转到第i个字符，默认为0
	void   MoveCurrLine(int i = 1)    { m_iCurrScriptLine += i;};	       //向下移动i行，默认移动1行
	void   MoveCurrChar(int i = 1)    { m_iCurrChar += i;};                //向后移动i个字符，默认移动1个字符


    virtual void  GetCommand(char * pCommandString);
	virtual void  GetTotalLine(char* lineContent);  //将一整行字符读入lineContent
    virtual int   GetIntParam();
	virtual long  GetLongParam();
	virtual DInt64 GetLLongParam();	
	virtual float GetFloatParam();
    virtual void  GetStringParam(char * pParamString);
	virtual void  GetTextguiString(char * pString);

};

//所有用CommandScript来操作文件的地方，只须要用同一个对象即可
CommandScript* getComandScript();

#endif
