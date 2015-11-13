

#ifndef CDEXTIME_H
#define CDEXTIME_H

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
class CDexTime
{
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
	stTime m_time;
private:
	int m_iDelta;
	float m_fTimeScale;
	signed __int64 m_iGameBeginTick;
public:
	CDexTime();
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
};

extern CDexTime* getTime();

#endif