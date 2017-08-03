/*************************************************************
****DemoEx���������ű���
****���ߣ�������(BruceKen)
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
    char ** m_ppScript;     //ָ��ǰ�ű���ָ�룬��һ��ָ��ָ��ǰ�����У��ڶ���ָ��ָ�������еĵڼ����ַ�
	int m_iScriptLine ;     //�ű��ܹ�������
	int m_iCurrScriptLine;  //�ű���ǰ��
	int m_iCurrChar;        //��ǰ�еĵ�ǰ�ڼ����ַ�
	BOOL m_bPause ;         //�ű��Ƿ���ͣ  
	int m_iPauseEndTime ;   //�ű���ͣʱ�䣬����ű��ȿ���һ��sprite�����ƶ�10�����أ�Ȼ��pause 100 ms,��ôsprite���100ms����ִ����һ������
	                        //�ڴ��ڼ�ű������ڵĵ�ǰ���������ı�

public:
    CommandScript();
    ~CommandScript();
	virtual BOOL OpenScript(const char * filename);
	virtual void CloseScript();	              //�رյ�ǰ�ļ�

	bool   GetUsing()				  { return m_bUsing;};
	char** GetScript()				  { return m_ppScript ;};
	int    GetScriptLine()		      { return m_iScriptLine;};
	int    GetCurrLine()              { return m_iCurrScriptLine;};
	int    GetCurrChar()              { return m_iCurrChar;};	 
	void   SetCurrLine( int i = 0)	  { m_iCurrScriptLine = i;};		   //ת����i�У�Ĭ��Ϊ0
	void   SetCurrChar(int i = 0)     { m_iCurrChar = i;};			       //ת����i���ַ���Ĭ��Ϊ0
	void   MoveCurrLine(int i = 1)    { m_iCurrScriptLine += i;};	       //�����ƶ�i�У�Ĭ���ƶ�1��
	void   MoveCurrChar(int i = 1)    { m_iCurrChar += i;};                //����ƶ�i���ַ���Ĭ���ƶ�1���ַ�


    virtual void  GetCommand(char * pCommandString);
	virtual void  GetTotalLine(char* lineContent);  //��һ�����ַ�����lineContent
    virtual int   GetIntParam();
	virtual long  GetLongParam();
	virtual DInt64 GetLLongParam();	
	virtual float GetFloatParam();
    virtual void  GetStringParam(char * pParamString);
	virtual void  GetTextguiString(char * pString);

};

//������CommandScript�������ļ��ĵط���ֻ��Ҫ��ͬһ�����󼴿�
CommandScript* getComandScript();

#endif
