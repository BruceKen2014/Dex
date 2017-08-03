

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
	int getDeltaTimeReal(); //ȡ��ʵ�ʵ�ÿ����Ϸѭ��deltatime
	int getDeltaTime();//ȡ����Ϸѭ��ʱ�� ����timescale
	float getTimeScale();
	void setTimeScale(float timeScale);
	signed __int64 GetTotalMillSeconds();
	signed __int64 GetSeconds();
	void  PrintInfo();

public:
	void BeginGameCycle();//����Ϸ��һ����Ⱦ���߼���֮ǰ����
	void EndGameCycle();//����Ϸ��һ����Ⱦ���߼���֮�����
};

extern CDexTime* getTime();

class DexTimeCheck
{
private:
	signed __int64& m_pParam;
	signed __int64 m_iTick;
public:
	DexTimeCheck(signed __int64& param);
	~DexTimeCheck();
};
#define TIME_CHECK_START(value)		signed __int64 value = getTime()->GetTotalMillSeconds()
#define TIME_CHECK_END(value)					   value = getTime()->GetTotalMillSeconds() - value
/*
����time check��ʹ�ã�
�������ֻ�ڵ�ǰ��������ʹ�ã���ôʹ��TIME_CHECK_START & TIME_CHECK_END�����Ҫ��������ʹ�ã���ʹ��DexTimeCheck
�磺
DInt64 checkTime1 = 0;
{//A
	{//B
		TIME_CHECK_START(checkTime2);
		//C
		{//��������ʹ��,ͳ��C�����ѵ�ʱ��
			DexTimeCheck TIMECHECK(checkTime1);
			..
		}
		...
		TIME_CHECK_END(checkTime2);
		log(checkTime2);//ֻ��B��ʹ��
	}
}
log(checkTime1);
*/
#endif