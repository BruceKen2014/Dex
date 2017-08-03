/*************************************************************
****DemoEx����Ի�����
****���ߣ�������(BruceKen)
2012.12
*************************************************************/

#pragma  once

#include "../DexBase/typedefine.h"
#include "../Source/CMyFont.h"
#include "CNFrame.h"

#define  SPEAK_MAX_WORDS  1024

//�Ի���:ֻ�������rpg�Ի���
typedef struct _TalkStruct
{
	int   m_head;  //��Ӧ��ͷ������
	int   m_dir;   //��Ӧ��λ��
	char* m_words; //˵�Ļ�
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
	CMyFont * m_font;		    //�Ի�������
	CNFrame*  m_backPic;	    //����ͼ

	CTexture* m_pPersonHead;    //˵��������ͷ��,��ָ����ⲿ���룬���ⲿ�����ͷţ���Ϊ��̸��ͷ��һֱ�ڱ䶯
	short     m_headPos; //ͷ��λ�� 0:��� 1���ұ�
	
protected:
	char*  m_words;      //Ҫ��ʾ������
 
	short m_length;     //���ֳ���
	short m_CurLength;  //��ǰ����ʾ�ĳ���

	short m_xOffset;    //���ֿ�ͷƫ���������㽫������ʾ��������ڱ���ͼƬ��
	short m_yOffset;

	short m_startX;  //��ʼλ��
	short m_startY;
	short m_width;	 //���
	short m_height; 

	short m_speed;      //������ʾ�ٶ�,��λ����

	int   m_thisTime;   //����ʱ��
	int   m_lastTime;   //�ϴ�ʱ��

public:
	CSpeak(LPDIRECT3DDEVICE9 device);
	~CSpeak();

public:
	virtual void ShutDown();
	virtual void LoadIni(char *inifile){};//�������ļ��г�ʼ���Ի�������

public:
	virtual bool LoadBackPic(DInt16 index, char* filename);
	virtual void Show();
	virtual void SetHead(CTexture* head, short pos = 0);  //����ͷ��,ͬʱ����ͷ���λ��

	virtual void SetWords(char * words);
	virtual void AddWords(char * words); //��ԭ�ĺ���׷���ַ���
	virtual void SetSpeed(short speed);	 

	virtual void SetWordsOffsetX(short x);
	virtual void SetWordsOffsetY(short y);
	virtual void SetWordsOffset(short x, short y);

	virtual void SetStartPos(short x, short y);	  
	virtual void SetSize(short w, short h);
	virtual void ReSetCurrLength();      //���õ�ǰ����
	virtual bool IsComplete();           //��ǰ�Ƿ���ʾ��(������ ��ǰ���� == ���ֳ���)
	virtual void SetEnd();     //������ʾ��������ֱ����ʾ�����ַ���
};