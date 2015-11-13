

#ifndef CDEX_OBJECT_H
#define CDEX_OBJECT_H

#include "typedefine.h"
#include "CDexTime.h"
#include "CEventHandler.h"
#include "CEventSender.h"
#include "DexMem.h"
#include "DexObjectFactory.h"
#include <list>
#include <string>

typedef struct _stTimer
{
	int id;
	int delay;
	int interval;
	_stTimer():id(0),delay(0),interval(0){};
	_stTimer(int _id, int _interval):id(_id),interval(_interval),delay(_interval){};
}stTimer;

#define Dex_DeclareClass(Object, count)						\
public:												\
	virtual std::string getType() const { return #Object;}\
	virtual int         getInst() const { return count;}  \
	static std::string getClassType()   { return #Object;} \
	static int         getClassInst()   { return count;}
class CDexObject:public CListener, public CEventSender, public CEventHandler
{
	Dex_DeclareClass(CDexObject,0)
protected:
	typedef std::list<stTimer> TlistTimer;
	TlistTimer m_timers;
	int32      m_iObjectId;  //һ��object��Ψһ��ID
	bool        m_bUpdate;  //�Ƿ����
	bool        m_bObjectValid;  //�Ƿ���Ч
	stArgs       m_param;
public:
	CDexObject();
	virtual ~CDexObject();
public:
	bool AddTimer(int id, int interval);
	bool RemoveTimer(int id);
	void SetUpdate(bool update);
	void SetValid(bool valid);
	void Release();  //һ��objectͨ�^factory���� ���Õr�{��Release����Ҫ�ք�Delete
	bool GetValid();
	int32 getObjectId();
	bool Archive(DexMem& mem, bool mode);
	stArgs& getArgs();
public:
	//����̓����
	virtual void Reset(); //reset�e��ă����ǘ�������������ļ����w����ؓ؟���������Þ��ʼ��B��Ҳؓ؟��������Ո�ăȴ�ጷ�
	virtual bool Update(int delta);
	virtual void OnTimer(const stTimer& timer);
	virtual bool OnEvent(stEvent event){ return false;};	 //�������¼���
	virtual bool EventValid(stEvent event){ return false;};   //�����Ƿ�����¼�public:
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
};

#endif