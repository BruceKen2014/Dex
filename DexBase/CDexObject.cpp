


#include "CDexObject.h"
#include "typedefine.h"
#include "DexObjectFactory.h"

CDexObject::CDexObject()
{
	m_iObjectId = CDexObjectFactroy::getObjectFactory()->getObjectId();
	m_bUpdate = true;
	m_bObjectValid = false;
}
CDexObject::~CDexObject()
{
}
bool CDexObject::AddTimer(int id, int interval)
{
	for(std::list<stTimer>::iterator it = m_timers.begin(); it != m_timers.end(); it++)
	{
		if((*it).id == id)//已找到相同id,添加失败
			return false;
	}
	m_timers.push_back(stTimer(id, interval));
	return true;
}

void CDexObject::SetUpdate(bool update)
{
	m_bUpdate = update;
}

void CDexObject::SetValid(bool valid)
{
	m_bObjectValid = valid;
}

void CDexObject::Release()
{
	CDexObjectFactroy::getObjectFactory()->allocateObject(this);
}

bool CDexObject::GetValid()
{
	return m_bObjectValid;
}

int32 CDexObject::getObjectId()
{
	return m_iObjectId;
}

bool CDexObject::RemoveTimer(int id)
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

void CDexObject::Reset()
{
	m_bUpdate = true;
	m_bObjectValid = false;
	m_timers.clear();
}

bool CDexObject::Update(int delta)
{
	if(!m_bUpdate)
		return false;
	for(std::list<stTimer>::iterator it = m_timers.begin(); it != m_timers.end(); it++)
	{
		(*it).delay += delta;
		if((*it).delay >= (*it).interval)
		{
			OnTimer(*it);
			(*it).delay = 0;
		}
	}
	return true;
}

void CDexObject::OnTimer(const stTimer& timer)
{

}

bool CDexObject::ArchiveIn(DexMem& mem)
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
bool CDexObject::ArchiveOut(DexMem& mem) const
{
	mem<<m_timers.size();
	foreach_const(TlistTimer, ite, m_timers)
	{
		mem<<(*ite);
	}
	mem<<m_bUpdate;
	return true;
}
bool CDexObject::Archive(DexMem& mem, bool mode)
{
	if(mode == DexMem::MEM_READ)
	{
		return ArchiveIn(mem);
	}
	else
		return ArchiveOut(mem);
}

stArgs& CDexObject::getArgs()
{
	return m_param;
}