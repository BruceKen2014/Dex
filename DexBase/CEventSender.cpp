
#include "typedefine.h"
#include "CEventSender.h"

CEventSender::CEventSender()
{

}

CEventSender::~CEventSender()
{

}

void CEventSender::AddListener(CListener* listener)
{
	if(listener == NULL)
		return;
	m_Listeners.push_back(listener);
}

void CEventSender::RemoveListener(CListener* listener)
{
	if(listener == NULL)
		return;
	for(std::list<CListener*>::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
	{
		if(*it == listener)
		{
			m_Listeners.erase(it);
			return;
		}
	}
}

void CEventSender::NotifyEvent(stEvent event)
{
	for(std::list<CListener*>::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
	{
		if(*it != NULL)
		{
			(*it)->OnEvent(event);
		}
	}
}
