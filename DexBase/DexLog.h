
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
	log_ok,		//正常！
	log_allert, //警告！
	log_error,  //错误！
	log_begin,  //BeginLog
	log_end,    //endlog
	log_count,
};
//控制台文字颜色
#define log_font_front_blue		 (FOREGROUND_BLUE)   //蓝
#define log_font_front_red		 (FOREGROUND_RED)    //红
#define log_font_front_green	 (FOREGROUND_GREEN)  //绿
#define log_font_front_yellow    (log_font_front_red | log_font_front_green) //黄
#define log_font_front_cyan      (log_font_front_blue | log_font_front_green ) //青
#define log_font_front_purple    (log_font_front_red | log_font_front_blue) //紫
#define log_font_front_while     (log_font_front_red | log_font_front_blue | log_font_front_green) //白
#define log_font_front_highlight (FOREGROUND_INTENSITY)	
//控制台文字背景色
#define log_font_back_blue		 (BACKGROUND_BLUE)   //蓝
#define log_font_back_red		 (BACKGROUND_RED)    //红
#define log_font_back_green		 (BACKGROUND_GREEN)  //绿
#define log_font_back_yellow     (log_font_back_red | log_font_back_green) //黄
#define log_font_back_cyan       (log_font_back_blue | log_font_back_green ) //青
#define log_font_back_purple     (log_font_back_red | log_font_back_blue) //紫
#define log_font_back_while      (log_font_back_red | log_font_back_blue | log_font_back_green) //白
#define log_font_backt_highlight (BACKGROUND_INTENSITY)	

#define log_font_underline       COMMON_LVB_UNDERSCORE  //打印下划线


	 //字体颜色高亮
class CFile;
class DexLog final
{
	SINGLETON_DECLARE(DexLog)
public:
	//w可理解为控制台一行可以容纳的字符数 h可理解为控制台可记录的行数 超过h行的打印将覆盖之前的记录
	void SetConsoleScreenSize(int w, int h);  //设置控制台缓存
	void SetConsoleTitle(const char* title);  //设置标题

	void SetFrontColor(DexLogType type, int color);
	//传入log_count表示所有类型都开启高亮打印
	void EnableFontFrontHighLight(DexLogType type = log_count, bool enable = true);  //是否开启前景高亮
	
	void SetBackColor(int color);
	//是否开启文字背景高亮,这里不加logtype的原因是因为背景色打印仅仅是为了美观，凸出某些字符的功能已经由字体颜色完成了
	//所以所用logtype的背景色都是一样的
	void EnableFontBackHighLight(bool enable = true);  

	void EnableBackColor(bool enable); //是否开启背景色打印
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
	bool m_OpenFontBack;  //是否开启控制台背景色的打印功能
	bool m_BackHighLight; //背景是否高亮
	bool m_bLogFile;	  //是否向文件输出log
	bool m_FrontHighlight[log_count];
	short m_consoleBackgroundColor;
	short m_consoleFontColor[log_count];

	void* m_stdHandle;

	DexLogType m_lastLogType;//上次的log类型 如果类型没变 则没必要再调用SetConsoleTextAttribute
	int  m_logByte;  //本次已经打印的字节数,根据这个变量来确定填充的背景空格(如果已经开启了背景色的话)
	int  m_wSize;
	CFile* m_logFile;

	HANDLE m_eventMutex; //用于互斥访问
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