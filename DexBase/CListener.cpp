
#include "typedefine.h"
#include "CListener.h"
#include "CEventSender.h"

CListener::CListener()
{
}

CListener::~CListener()
{

}
void CListener::ShutDown()
{
	//注销所有监听
	for(std::list<CEventSender*>::iterator it = m_vSender.begin(); it != m_vSender.end(); it++)
	{
		if(*it == NULL)
			continue;
		(*it)->RemoveListener(this);
	}
	//释放资源
}