

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
	int32      m_iObjectId;  //一个object有唯一的ID
	bool        m_bUpdate;  //是否更新
	bool        m_bObjectValid;  //是否有效
	stArgs       m_param;
public:
	CDexObject();
	virtual ~CDexObject();
public:
	bool AddTimer(int id, int interval);
	bool RemoveTimer(int id);
	void SetUpdate(bool update);
	void SetValid(bool valid);
	void Release();  //一個object通過factory創建 不用時調用Release，不要手動Delete
	bool GetValid();
	int32 getObjectId();
	bool Archive(DexMem& mem, bool mode);
	stArgs& getArgs();
public:
	//以下為虛函數
	virtual void Reset(); //reset裡面的內容是構造和析構函數的集合體，既負責將數據重置為初始狀態，也負責將自身申請的內存釋放
	virtual bool Update(int delta);
	virtual void OnTimer(const stTimer& timer);
	virtual bool OnEvent(stEvent event){ return false;};	 //监听到事件，
	virtual bool EventValid(stEvent event){ return false;};   //决定是否处理该事件public:
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
};

#endif