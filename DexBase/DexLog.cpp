

#include "stdafx.h"
#include "DexLog.h"
#include "CFile.h"
#include "CDexTime.h"

#ifdef _DEBUG
#define DEX_LOG
#endif
using namespace std;
CDexLog::CDexLog() :m_bLogFile(true)
{
#ifdef DEX_LOG
	AllocConsole();
	m_stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int out_handle = _open_osfhandle((long)m_stdHandle, _O_TEXT);
	FILE * fp = _fdopen(out_handle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	//设置默认颜色，并且不打开背景色打印和文字高亮功能
	m_consoleFontColor[log_ok] = log_font_front_green;
	m_consoleFontColor[log_allert] = log_font_front_yellow;
	m_consoleFontColor[log_error] = log_font_front_red;

	m_consoleBackgroundColor = log_font_back_cyan;

	m_BackHighLight = false;
	m_OpenFontBack = false;
	m_lastLogType = log_count;
	m_wSize = 0;

	m_logFile = new CFile;
	char filename[64];
	sprintf(filename, "log/%d-%d-%d-%d-%d-%d.log", getTime()->m_time.year, getTime()->m_time.month, getTime()->m_time.day, 
		getTime()->m_time.hour, getTime()->m_time.minute, getTime()->m_time.second);
	m_logFile->CreateCFile(filename);
#endif
}

CDexLog::~CDexLog()
{
#ifdef DEX_LOG
	m_logFile->Close();
	delete m_logFile;
#endif
}

void CDexLog::SetConsoleScreenSize(int w, int h)
{
	m_wSize = w;
	COORD sz = { w, h};
	SetConsoleScreenBufferSize(m_stdHandle,sz);
}

void CDexLog::SetConsoleTitle(const char* title)
{
	::SetConsoleTitle(title);
}

void CDexLog::SetFrontColor(DexLogType type, int color)
{
	if(type == log_count)
		return;
	m_consoleFontColor[type] = color;

	//如果之前已经设定了该种类型的输出为高亮，这里要设置一下
	if(m_FrontHighlight[type])
		m_consoleFontColor[type] |= log_font_front_highlight; 
}

void CDexLog::EnableFontFrontHighLight(DexLogType type, bool enable)
{	
	if(type == log_count)
	{
		for(int i = log_ok; i < log_count; i++)
		{
			if(enable)
				m_consoleFontColor[i] |= log_font_front_highlight;
			else
				m_consoleFontColor[i] ^= log_font_front_highlight;
			m_FrontHighlight[i] = enable;
		}
	}
	else
	{
		if(enable)
			m_consoleFontColor[type] |= log_font_front_highlight;
		else
			m_consoleFontColor[type] ^= log_font_front_highlight;
		m_FrontHighlight[type] = enable;
	}
}

void CDexLog::SetBackColor(int color)
{
	m_consoleBackgroundColor = color;
	if(m_BackHighLight)
		m_consoleBackgroundColor |= log_font_backt_highlight;
	else
		m_consoleBackgroundColor ^= log_font_backt_highlight;
}

void CDexLog::EnableFontBackHighLight(bool enable)
{
	if(enable)
	{
		m_BackHighLight = true;
		m_consoleBackgroundColor |= log_font_backt_highlight;
	}
	else
	{
		m_BackHighLight = false;
		m_consoleBackgroundColor ^= log_font_backt_highlight;
	}
}

void CDexLog::EnableBackColor(bool enable)
{
	m_OpenFontBack = enable;
	for(int i = log_ok; i < log_count; i++)
	{
		m_consoleFontColor[i] |= m_consoleBackgroundColor;
	}
}

void CDexLog::SetLogFile(bool bLogFile)
{
	m_bLogFile = bLogFile;
}
void CDexLog::BeginLog()
{
#ifdef DEX_LOG
	m_logByte = 0;
#endif
}
void CDexLog::LogTime()
{
	char str[128];
	sprintf(str, "[%d-%d-%d-%d:%d:%d]", getTime()->m_time.year, getTime()->m_time.month, getTime()->m_time.day,
		getTime()->m_time.hour, getTime()->m_time.minute, getTime()->m_time.second);
	m_logByte += strlen(str);
	cout << str;
	if (m_bLogFile)
		m_logFile->InString(str);
}
void CDexLog::LogOK(char* msg)
{
	LogTime();
	cout<<"[  log_OK  ]"<<msg;
	if (m_bLogFile)
		m_logFile->InString("[  log_OK  ]%s", msg);
	m_logByte += 12;
}
void CDexLog::LogAllert(char* msg)
{
	LogTime();
	cout<<"[log_Allert]"<<msg;
	if (m_bLogFile)
		m_logFile->InString("[log_Allert]%s", msg);
	m_logByte += 12;
}
void CDexLog::LogError(char* msg)
{
	LogTime();
	cout<<"[log_Error ]"<<msg;
	if (m_bLogFile)
		m_logFile->InString("[log_Error ]%s", msg);
	m_logByte += 12;
}
void CDexLog::Log(DexLogType log_type, char* msg, ...)
{	
#ifdef DEX_LOG
	va_list v_p;
	va_start(v_p, msg);
	TCHAR szText[512];
	wvsprintf(szText, msg,v_p);
	va_end(v_p);

	if(m_lastLogType != log_type)
	{
		m_lastLogType = log_type;
		SetConsoleTextAttribute(m_stdHandle, m_consoleFontColor[log_type]);
	}
	m_logByte += strlen(szText);
	if(log_type == log_ok)
		LogOK(szText);
	else if(log_type == log_allert)
		LogAllert(szText);
	else if(log_type == log_error)
		LogError(szText);
#endif

}

void CDexLog::LogLine(DexLogType log_type, char* msg, ...)
{
	BeginLog();
#ifdef DEX_LOG
	va_list v_p;
	va_start(v_p, msg);
	TCHAR szText[512];
	wvsprintf(szText, msg, v_p);
	va_end(v_p);

	if (m_lastLogType != log_type)
	{
		m_lastLogType = log_type;
		SetConsoleTextAttribute(m_stdHandle, m_consoleFontColor[log_type]);
	}
	m_logByte += strlen(szText);
	if (log_type == log_ok)
		LogOK(szText);
	else if (log_type == log_allert)
		LogAllert(szText);
	else if (log_type == log_error)
		LogError(szText);
#endif
	EndLog();
}
void CDexLog::EndLog()
{
#ifdef DEX_LOG
	if (m_bLogFile)
		m_logFile->InBlankLine();
	//此处处理控制抬的log
	if(!m_OpenFontBack)
	{//未开启背景打印则无需打印剩下的空格 只须换行即可
		cout<<endl;
		return;
	}
	int leftBlank = m_wSize - m_logByte;
	string blank;
	for(int i = 0 ; i < leftBlank; i++)
	{
		blank += " ";
	}
	cout<<blank.c_str();//endl;
#endif
}

CDexLog* getLog()
{
	static CDexLog* g_log = new CDexLog();
	return g_log;
}