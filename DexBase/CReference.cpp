


#include "CReference.h"

CRefCount::CRefCount()
{
	m_count = 1;
}

CRefCount::~CRefCount()
{
	
}

void CRefCount::ShutDown()
{
	//สอทลืสิด
}

void CRefCount::AddRef()
{
	m_count++;
}

void CRefCount::ReduceRef()
{
	if(m_count == 0)
		return;
	if(--m_count == 0)
	{
		ShutDown();
	}
}