

#include <sys/timeb.h>
#include <time.h>
#include "CDexTime.h"


SINGLETON_IMPLEMENT(DexTime, DexTime)
DexTime::DexTime()
{
	m_iDelta = 0;
	m_fTimeScale = 1.0f;
	UpdateTime();
}
DexTime::~DexTime()
{
}
int DexTime::getDeltaTime()
{
	return m_iDelta * m_fTimeScale;
}
int DexTime::getDeltaTimeReal()
{
	return m_iDelta;
}
float DexTime::getTimeScale()
{
	return m_fTimeScale;
}
void DexTime::setTimeScale(float timeScale)
{
	m_fTimeScale = timeScale;
}
void DexTime::UpdateTime()
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

void DexTime::PrintInfo()
{
}

signed __int64 DexTime::GetTotalMillSeconds()
{
	return m_time.totalMillSeconds;
}

void DexTime::BeginGameCycle()
{
	UpdateTime();
	m_iGameBeginTick = m_time.totalMillSeconds;
}

void DexTime::EndGameCycle()
{
	UpdateTime();
	m_iDelta = m_time.totalMillSeconds - m_iGameBeginTick;
}

stTime& DexTime::getTime()
{
	return m_time;
}

DexTimeCheck::DexTimeCheck(signed __int64& param) :m_pParam(param)
{
	m_iTick = DexTime::getSingleton()->GetTotalMillSeconds();
}

DexTimeCheck::~DexTimeCheck()
{
	m_pParam = DexTime::getSingleton()->GetTotalMillSeconds() - m_iTick;
}