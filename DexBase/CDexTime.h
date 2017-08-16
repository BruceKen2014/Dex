

#ifndef CDEXTIME_H
#define CDEXTIME_H
#include "DexSingleton.h"

typedef struct 
{
	int year;
	int month;
	int day;
	int weekday;
	int hour;
	int minute;
	int second;
	int millSecond;

	signed __int64 totalSeconds;
	signed __int64 totalMillSeconds;
}stTime;
class DexTime final
{
	SINGLETON_DECLARE(DexTime)
public:
	typedef enum
	{
		TIME_SUNDAY = 0,
		TIME_MONDAY,
		TIME_TUESDAY,
		TIME_WEDNESDAY,
		TIME_THURSDAY,
		TIME_FRIDAY,
		TIME_SATURDAY
	}EDEX_WEEK;
	typedef enum
	{
		TIME_JANUARY = 1,
		TIME_FEBUARY,
		TIME_MARCH,
		TIME_APRIL,
		TIME_MAY,
		TIME_JUN,
		TIME_JULY,
		TIME_AUGUST,
		TIME_SEPTEMBER,
		TIME_OCTOBER,
		TIME_NOVEMBER,
		TIME_DECEMBER
	}EDEX_MONTH;
public:
	void  UpdateTime();
	int getDeltaTimeReal(); //取得实际的每次游戏循环deltatime
	int getDeltaTime();//取得游戏循环时间 乘以timescale
	float getTimeScale();
	void setTimeScale(float timeScale);
	signed __int64 GetTotalMillSeconds();
	signed __int64 GetSeconds();
	void  PrintInfo();

public:
	void BeginGameCycle();//在游戏的一切渲染与逻辑等之前调用
	void EndGameCycle();//在游戏的一切渲染与逻辑等之后调用
	stTime& getTime();
private:
	stTime m_time;
	int m_iDelta;
	float m_fTimeScale;
	signed __int64 m_iGameBeginTick;
};

class DexTimeCheck
{
private:
	signed __int64& m_pParam;
	signed __int64 m_iTick;
public:
	DexTimeCheck(signed __int64& param);
	~DexTimeCheck();
};
#define TIME_CHECK_START(value)		signed __int64 value = DexTime::getSingleton()->GetTotalMillSeconds()
#define TIME_CHECK_END(value)					   value = DexTime::getSingleton()->GetTotalMillSeconds() - value
/*
关于time check的使用：
如果变量只在当前生命期内使用，那么使用TIME_CHECK_START & TIME_CHECK_END，如果要跨生命期使用，则使用DexTimeCheck
如：
DInt64 checkTime1 = 0;
{//A
	{//B
		TIME_CHECK_START(checkTime2);
		//C
		{//跨生命期使用,统计C区花费的时间
			DexTimeCheck TIMECHECK(checkTime1);
			..
		}
		...
		TIME_CHECK_END(checkTime2);
		log(checkTime2);//只在B区使用
	}
}
log(checkTime1);
*/
#endif