


#include <time.h>
//#include "stdafx.h"
#include "CFunction.h"


//================================================================
//void _Message(char * message, ...)
//{
//	va_list v_p;
//	va_start(v_p, message);
//	TCHAR szText[1024];
//	wvsprintf(szText, message,v_p);
//	va_end(v_p);
//	MessageBox(NULL, szText, TEXT("提示"), MB_OK);	
//};
//----------------------------------------------------------------


//----------------------------------------------------------------
void  _GetTime(_TIME * t)
{
	time_t Time;
	Time = time(NULL);
	struct tm *temp;
	temp= localtime(&Time);	
	//asctime(t) 返回值char *，用于将时间以固定格式输出
	t->m_year  = temp->tm_year;
	t->m_month = temp->tm_mon;
	t->m_day   = temp->tm_mday;
	t->m_hour  = temp->tm_hour;
	t->m_min   = temp->tm_min;
	t->m_second= temp->tm_sec;
	t->m_week  = temp->tm_wday;
}