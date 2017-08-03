/*************************************************************
****DemoEx引擎对话框类
****作者：阚正杰(BruceKen)
2012.12
*************************************************************/

#pragma  once

#include "../DexBase/typedefine.h"
#include "../Source/CMyFont.h"
#include "CNFrame.h"

#define  SPEAK_MAX_WORDS  1024

//对话框:只针对中文rpg对话框
typedef struct _TalkStruct
{
	int   m_head;  //对应的头像索引
	int   m_dir;   //对应的位置
	char* m_words; //说的话
	_TalkStruct()
	{
		m_head  = 0;
		m_dir = 0;
		m_words = NULL;
	}
	~_TalkStruct()
	{
		_SafeFree(m_words);
	}
}TalkStruct;
class CSpeak
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	CMyFont * m_font;		    //对话框字体
	CNFrame*  m_backPic;	    //背景图

	CTexture* m_pPersonHead;    //说话的人物头像,此指针从外部传入，在外部自行释放，因为交谈中头像一直在变动
	short     m_headPos; //头像位置 0:左边 1：右边
	
protected:
	char*  m_words;      //要显示的文字
 
	short m_length;     //文字长度
	short m_CurLength;  //当前已显示的长度

	short m_xOffset;    //文字开头偏移量，方便将文字显示区域放置在背景图片中
	short m_yOffset;

	short m_startX;  //起始位置
	short m_startY;
	short m_width;	 //宽高
	short m_height; 

	short m_speed;      //文字显示速度,单位毫秒

	int   m_thisTime;   //本次时间
	int   m_lastTime;   //上次时间

public:
	CSpeak(LPDIRECT3DDEVICE9 device);
	~CSpeak();

public:
	virtual void ShutDown();
	virtual void LoadIni(char *inifile){};//从配置文件中初始化对话框数据

public:
	virtual bool LoadBackPic(DInt16 index, char* filename);
	virtual void Show();
	virtual void SetHead(CTexture* head, short pos = 0);  //设置头像,同时传入头像的位置

	virtual void SetWords(char * words);
	virtual void AddWords(char * words); //在原文后再追加字符串
	virtual void SetSpeed(short speed);	 

	virtual void SetWordsOffsetX(short x);
	virtual void SetWordsOffsetY(short y);
	virtual void SetWordsOffset(short x, short y);

	virtual void SetStartPos(short x, short y);	  
	virtual void SetSize(short w, short h);
	virtual void ReSetCurrLength();      //重置当前长度
	virtual bool IsComplete();           //当前是否显示完(条件是 当前长度 == 文字长度)
	virtual void SetEnd();     //设置显示结束，即直接显示所有字符串
};