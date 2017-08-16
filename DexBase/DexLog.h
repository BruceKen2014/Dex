
#include "DexCompiler.h"

#ifdef _DEX_PLATFORM_WINDOWS

#ifndef _DEX_LOG_H
#define _DEX_LOG_H
#include <wtypes.h>
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include "DDeque.h"
#include "DexDVector.h"
#include "DexString.h"
#include "DexSingleton.h"

enum DexLogType
{
	log_ok,		//������
	log_allert, //���棡
	log_error,  //����
	log_begin,  //BeginLog
	log_end,    //endlog
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
class DexLog final
{
	SINGLETON_DECLARE(DexLog)
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
public:
	void DoLog();
	void SetLogFile(bool bLogFile);
	void BeginLog();
	void Log(DexLogType log_type, char* msg, ...);
	void LogLine(DexLogType log_type, char* msg, ...);
	void EndLog();
protected:
	void LogTime();
	void LogOK(char* msg);
	void LogAllert(char* msg);
	void LogError(char* msg);
private:
	bool m_OpenFontBack;  //�Ƿ�������̨����ɫ�Ĵ�ӡ����
	bool m_BackHighLight; //�����Ƿ����
	bool m_bLogFile;	  //�Ƿ����ļ����log
	bool m_FrontHighlight[log_count];
	short m_consoleBackgroundColor;
	short m_consoleFontColor[log_count];

	void* m_stdHandle;

	DexLogType m_lastLogType;//�ϴε�log���� �������û�� ��û��Ҫ�ٵ���SetConsoleTextAttribute
	int  m_logByte;  //�����Ѿ���ӡ���ֽ���,�������������ȷ�����ı����ո�(����Ѿ������˱���ɫ�Ļ�)
	int  m_wSize;
	CFile* m_logFile;

	HANDLE m_eventMutex; //���ڻ������
	typedef struct _stLogTask
	{
		DexLogType type;
		DString    msg;
		_stLogTask() = default;
		_stLogTask(DexLogType _type) :type(_type)
		{};
		_stLogTask(DexLogType _type, char* _msg) :type(_type), msg(_msg)
		{};
	}stLogTask;
	DDeque<stLogTask>  m_logTasks;
};
#define dex_log_current_file(type)  DexLog::getSingleton()->Log(type, "%s ", __FILE__);
#define dex_log_current_line(type)  DexLog::getSingleton()->Log(type, "%d ", __LINE__);
#define dex_log_current_f_l(type)   DexLog::getSingleton()->LogLine(type, "%s %d ", __FILE__, __LINE__);

#endif //_DEX_LOG_H
#endif //_DEX_PLATFORM_WINDOWS