

#include "stdafx.h"
#include "DexLog.h"
#include "CFile.h"
#include "CDexTime.h"
#include "DexDefine.h"

#ifdef _DEBUG
#define DEX_LOG
#endif

#define LOG_THREAD  //启用专用线程输出log

using namespace std;
SINGLETON_IMPLEMENT(DexLog, DexLog)
DexLog::DexLog() :m_bLogFile(true)
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
	sprintf(filename, "log/%d-%d-%d-%d-%d-%d.log", DexTime::getSingleton()->getTime().year, DexTime::getSingleton()->getTime().month, DexTime::getSingleton()->getTime().day,
		DexTime::getSingleton()->getTime().hour, DexTime::getSingleton()->getTime().minute, DexTime::getSingleton()->getTime().second);
	m_logFile->CreateCFile(filename);
	m_eventMutex = CreateEvent(NULL, FALSE, TRUE, NULL);
#endif
}

DexLog::~DexLog()
{
#ifdef DEX_LOG
	m_logFile->Close();
	delete m_logFile;
#endif
}

void DexLog::SetConsoleScreenSize(int w, int h)
{
	m_wSize = w;
	COORD sz = { w, h};
	SetConsoleScreenBufferSize(m_stdHandle,sz);
}

void DexLog::SetConsoleTitle(const char* title)
{
	::SetConsoleTitle(title);
}

void DexLog::SetFrontColor(DexLogType type, int color)
{
	if(type == log_count)
		return;
	m_consoleFontColor[type] = color;

	//如果之前已经设定了该种类型的输出为高亮，这里要设置一下
	if(m_FrontHighlight[type])
		m_consoleFontColor[type] |= log_font_front_highlight; 
}

void DexLog::EnableFontFrontHighLight(DexLogType type, bool enable)
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

void DexLog::SetBackColor(int color)
{
	m_consoleBackgroundColor = color;
	if(m_BackHighLight)
		m_consoleBackgroundColor |= log_font_backt_highlight;
	else
		m_consoleBackgroundColor ^= log_font_backt_highlight;
}

void DexLog::EnableFontBackHighLight(bool enable)
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

void DexLog::EnableBackColor(bool enable)
{
	m_OpenFontBack = enable;
	for(int i = log_ok; i < log_count; i++)
	{
		m_consoleFontColor[i] |= m_consoleBackgroundColor;
	}
}

void DexLog::DoLog()
{
	DEX_ENSURE(m_logTasks.size() != 0);

	WaitForSingleObject(m_eventMutex, DEXINFINITE);
	if (m_logTasks.size() == 0)
	{
		SetEvent(m_eventMutex);
		return;
	}
	stLogTask log = m_logTasks.back();
	m_logTasks.pop_back();
	SetEvent(m_eventMutex);
	switch (log.type)
	{
		case log_begin:
		{
			m_logByte = 0;	  
		}
		break;
		case log_end:
		{
			if (m_bLogFile)
				m_logFile->InBlankLine();
			OutputDebugString("\n");
			//此处处理控制抬的log
			if (m_OpenFontBack)
			{
				int leftBlank = m_wSize - m_logByte;
				string blank;
				for (int i = 0; i < leftBlank; i++)
				{
					blank += " ";
				}
				cout << blank.c_str();//endl;
			}
			else
			{//未开启背景打印则无需打印剩下的空格 只须换行即可
				cout << endl;
			}
		}
		break;
		default:
		{
			if (m_lastLogType != log.type)
			{
				m_lastLogType = log.type;
				SetConsoleTextAttribute(m_stdHandle, m_consoleFontColor[log.type]);
			}
			m_logByte += log.msg.length();
			if (log.type == log_ok)
				LogOK((DChar*)log.msg.c_str());
			else if (log.type == log_allert)
				LogAllert((DChar*)log.msg.c_str());
			else if (log.type == log_error)
				LogError((DChar*)log.msg.c_str());  
		}
		break;
	}
}
void DexLog::SetLogFile(bool bLogFile)
{
	m_bLogFile = bLogFile;
}
void DexLog::BeginLog()
{
#ifdef DEX_LOG
#ifdef LOG_THREAD
	WaitForSingleObject(m_eventMutex, DEXINFINITE);
	m_logTasks.emplace_front(log_begin);
	SetEvent(m_eventMutex);
#else
	m_logByte = 0;
#endif
#endif
}
void DexLog::LogTime()
{
	char str[128];
	sprintf(str, "[%d-%d-%d-%d:%d:%d]", DexTime::getSingleton()->getTime().year, DexTime::getSingleton()->getTime().month, DexTime::getSingleton()->getTime().day,
		DexTime::getSingleton()->getTime().hour, DexTime::getSingleton()->getTime().minute, DexTime::getSingleton()->getTime().second);
	m_logByte += strlen(str);
	cout << str;
	if (m_bLogFile)
		m_logFile->InString(str);
}
void DexLog::LogOK(char* msg)
{
	LogTime();
	cout<<"[  log_OK  ]"<<msg;
	OutputDebugString(msg);
	if (m_bLogFile)
		m_logFile->InString("[  log_OK  ]%s", msg);
	m_logByte += 12;
}
void DexLog::LogAllert(char* msg)
{
	LogTime();
	cout<<"[log_Allert]"<<msg;
	OutputDebugString(msg);
	if (m_bLogFile)
		m_logFile->InString("[log_Allert]%s", msg);
	m_logByte += 12;
}
void DexLog::LogError(char* msg)
{
	LogTime();
	cout<<"[log_Error ]"<<msg;
	OutputDebugString(msg);
	if (m_bLogFile)
		m_logFile->InString("[log_Error ]%s", msg);
	m_logByte += 12;
}
void DexLog::Log(DexLogType log_type, char* msg, ...)
{	
#ifdef DEX_LOG 
#ifdef LOG_THREAD
	va_list v_p;
	va_start(v_p, msg);
	TCHAR szText[256];
	wvsprintf(szText, msg,v_p);
	va_end(v_p);

	WaitForSingleObject(m_eventMutex,DEXINFINITE);
	m_logTasks.emplace_front(log_type, szText);
	SetEvent(m_eventMutex);
#else
	va_list v_p;
	va_start(v_p, msg);
	TCHAR szText[256];
	wvsprintf(szText, msg, v_p);
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
#endif

}

void DexLog::LogLine(DexLogType log_type, char* msg, ...)
{
#ifdef DEX_LOG
#ifdef LOG_THREAD
	va_list v_p;
	va_start(v_p, msg);
	TCHAR szText[256];
	wvsprintf(szText, msg, v_p);
	va_end(v_p);

	WaitForSingleObject(m_eventMutex, DEXINFINITE);
	m_logTasks.emplace_front(log_begin);
	m_logTasks.emplace_front(log_type, szText);
	m_logTasks.emplace_front(log_end);
	SetEvent(m_eventMutex);
#else
	BeginLog();
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
	EndLog();
#endif
#endif
}
void DexLog::EndLog()
{
#ifdef DEX_LOG
#ifdef LOG_THREAD
	WaitForSingleObject(m_eventMutex, DEXINFINITE);
	m_logTasks.emplace_front(log_end);
	SetEvent(m_eventMutex);
#else
	if (m_bLogFile)
		m_logFile->InBlankLine();
	OutputDebugString("\n");
	//此处处理控制抬的log
	if (!m_OpenFontBack)
	{//未开启背景打印则无需打印剩下的空格 只须换行即可
		cout << endl;
		return;
	}
	int leftBlank = m_wSize - m_logByte;
	string blank;
	for (int i = 0; i < leftBlank; i++)
	{
		blank += " ";
	}
	cout<<blank.c_str();//endl;
#endif
#endif
}