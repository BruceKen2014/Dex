

#include <sys/timeb.h>
#include <time.h>
#include "CDexTime.h"

CDexTime::CDexTime()
{
	m_iDelta = 0;
	m_fTimeScale = 1.0f;
	UpdateTime();
}
int CDexTime::getDeltaTime()
{
	return m_iDelta * m_fTimeScale;
}
int CDexTime::getDeltaTimeReal()
{
	return m_iDelta;
}
float CDexTime::getTimeScale()
{
	return m_fTimeScale;
}
void CDexTime::setTimeScale(float timeScale)
{
	m_fTimeScale = timeScale;
}
void CDexTime::UpdateTime()
{
	timeb   tp;
	ftime(&tp);	
	m_time.totalSeconds = tp.time;
	m_time.millSecond = tp.millitm;
	m_time.totalMillSeconds = tp.time*1000 + tp.millitm;

	time_t Time;
	Time = time(NULL);
	tm *temp;
	temp= localtime(&Time);	
	m_time.year    = temp->tm_year + 1900;
	m_time.month   = temp->tm_mon + 1;
	m_time.day     = temp->tm_mday;
	m_time.hour    = temp->tm_hour;
	m_time.minute  = temp->tm_min;
	m_time.second  = temp->tm_sec;
	m_time.weekday = temp->tm_wday;
}

void CDexTime::PrintInfo()
{
}

signed __int64 CDexTime::GetTotalMillSeconds()
{
	return m_time.totalMillSeconds;
}

void CDexTime::BeginGameCycle()
{
	UpdateTime();
	m_iGameBeginTick = m_time.totalMillSeconds;
}

void CDexTime::EndGameCycle()
{
	UpdateTime();
	m_iDelta = m_time.totalMillSeconds - m_iGameBeginTick;
}

CDexTime* getTime()
{
	static CDexTime* g_time = new CDexTime;
	g_time->UpdateTime();
	return g_time;
}
