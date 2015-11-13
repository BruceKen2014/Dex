

#pragma once
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include "typedefine.h"
enum DexLogType
{
	log_ok,		//������
	log_allert, //���棡
	log_error,  //����
	log_count,
};
//����̨������ɫ
#define log_font_front_blue		 (FOREGROUND_BLUE)   //��
#define log_font_front_red		 (FOREGROUND_RED)    //��
#define log_font_front_green	 (FOREGROUND_GREEN)  //��
#define log_font_front_yellow    (log_font_front_red | log_font_front_green) //��
#define log_font_front_cyan      (log_font_front_blue | log_font_front_green ) //��
#define log_font_front_purple    (log_font_front_red | log_font_front_blue) //��
#define log_font_front_while     (log_font_front_red | log_font_front_blue | log_font_front_green) //��
#define log_font_front_highlight (FOREGROUND_INTENSITY)	
//����̨���ֱ���ɫ
#define log_font_back_blue		 (BACKGROUND_BLUE)   //��
#define log_font_back_red		 (BACKGROUND_RED)    //��
#define log_font_back_green		 (BACKGROUND_GREEN)  //��
#define log_font_back_yellow     (log_font_back_red | log_font_back_green) //��
#define log_font_back_cyan       (log_font_back_blue | log_font_back_green ) //��
#define log_font_back_purple     (log_font_back_red | log_font_back_blue) //��
#define log_font_back_while      (log_font_back_red | log_font_back_blue | log_font_back_green) //��
#define log_font_backt_highlight (BACKGROUND_INTENSITY)	

#define log_font_underline       COMMON_LVB_UNDERSCORE  //��ӡ�»���

	 //������ɫ����
class CFile;
class CDexLog
{
private:
	bool m_OpenFontBack;  //�Ƿ�������̨����ɫ�Ĵ�ӡ����
	bool m_BackHighLight; //�����Ƿ����
	bool m_FrontHighlight[log_count]; 
	WORD m_consoleBackgroundColor;
	WORD m_consoleFontColor[log_count];

	HANDLE m_stdHandle;

	DexLogType m_lastLogType;//�ϴε�log���� �������û�� ��û��Ҫ�ٵ���SetConsoleTextAttribute
	int  m_logByte;  //�����Ѿ���ӡ���ֽ���,�������������ȷ�����ı����ո�(����Ѿ������˱���ɫ�Ļ�)
	int  m_wSize;
	CFile* m_logFile;
public:
	CDexLog();
	~CDexLog();
public:
	//w�����Ϊ����̨һ�п������ɵ��ַ��� h�����Ϊ����̨�ɼ�¼������ ����h�еĴ�ӡ������֮ǰ�ļ�¼
	void SetConsoleScreenSize(int w, int h);  //���ÿ���̨����
	void SetConsoleTitle(const char* title);  //���ñ���

	void SetFrontColor(DexLogType type, int color);
	//����log_count��ʾ�������Ͷ�����������ӡ
	void EnableFontFrontHighLight(DexLogType type = log_count, bool enable = true);  //�Ƿ���ǰ������
	
	void SetBackColor(int color);
	//�Ƿ������ֱ�������,���ﲻ��logtype��ԭ������Ϊ����ɫ��ӡ������Ϊ�����ۣ�͹��ĳЩ�ַ��Ĺ����Ѿ���������ɫ�����
	//��������logtype�ı���ɫ����һ����
	void EnableFontBackHighLight(bool enable = true);  

	void EnableBackColor(bool enable); //�Ƿ�������ɫ��ӡ

protected:
	void LogTime();
	void LogOK(char* msg);
	void LogAllert(char* msg);
	void LogError(char* msg);
public:
	void BeginLog();
	void Log(DexLogType log_type, char* msg, ...);
	void EndLog();
};
CDexLog* getLog();
#define dex_log_current_file(type)  getLog()->Log(type, "%s", __FILE__);
#define dex_log_current_line(type)  getLog()->Log(type, "%d", __LINE__);
#define dex_log_current_f_l(type) \
	dex_log_current_file(type)\
	dex_log_current_line(type)