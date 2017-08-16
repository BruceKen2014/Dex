


#include "DexObject.h"
#include "typedefine.h"
#include "DexObjectFactory.h"

DexObject::DexObject()
{
	m_iObjectId = CDexObjectFactroy::getObjectFactory()->getObjectId();
	m_bUpdate = true;
	m_bObjectValid = false;
}
DexObject::~DexObject()
{
}
bool DexObject::AddTimer(int id, int interval)
{
	for(std::list<stTimer>::iterator it = m_timers.begin(); it != m_timers.end(); it++)
	{
		if((*it).id == id)//已找到相同id,添加失败
			return false;
	}
	m_timers.push_back(stTimer(id, interval));
	return true;
}

void DexObject::SetUpdate(bool update)
{
	m_bUpdate = update;
}

void DexObject::SetValid(bool valid)
{
	m_bObjectValid = valid;
}

void DexObject::Release()
{
	CDexObjectFactroy::getObjectFactory()->allocateObject(this);
}

bool DexObject::GetValid()
{
	return m_bObjectValid;
}

DInt32 DexObject::getObjectId()
{
	return m_iObjectId;
}

bool DexObject::RemoveTimer(int id)
{
	for(std::list<stTimer>::iterator it = m_timers.begin(); it != m_timers.end(); it++)
	{
		if((*it).id == id)
		{
			m_timers.erase(it);
			return true;	  //删除成功
		}
	}
	return false;
}

void DexObject::Reset()
{
	m_bUpdate = true;
	m_bObjectValid = false;
	m_timers.clear();
}

bool DexObject::Update(int delta)
{
	if(!m_bUpdate)
		return false;
	foreach(TlistTimer, it, m_timers)
	{
		stTimer& timer = (*it);
		timer.delay += delta;
		if (timer.delay >= timer.interval)
		{
			OnTimer(timer);
			timer.delay = 0;
		}
	}
	return true;
}

void DexObject::OnTimer(const stTimer& timer)
{

}

DInt32 DexObject::Size() const
{//继承类要计算自己的size大小
	return sizeof(stTimer)*m_timers.size() + sizeof(m_iObjectId)+sizeof(m_bUpdate)+sizeof(m_bObjectValid)+sizeof(m_param);
}
bool DexObject::ArchiveIn(DexMem& mem)
{
	size_t timer_size = 0 ;
	mem>>timer_size;
	for(size_t i = 0 ; i < timer_size; i++)
	{
		stTimer t;
		mem>>t;
		m_timers.push_back(t);
	}
	mem>>m_bUpdate;
	return true;
}
bool DexObject::ArchiveOut(DexMem& mem) const
{
	mem<<m_timers.size();
	foreach_const(TlistTimer, ite, m_timers)
	{
		mem<<(*ite);
	}
	mem<<m_bUpdate;
	return true;
}
bool DexObject::Archive(DexMem& mem, bool mode)
{
	if(mode == DexMem::MEM_READ)
	{
		return ArchiveIn(mem);
	}
	else
		return ArchiveOut(mem);
}

stArgs& DexObject::getArgs()
{
	return m_param;
}