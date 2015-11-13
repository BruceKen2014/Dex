
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
	//ע�����м���
	for(std::list<CEventSender*>::iterator it = m_vSender.begin(); it != m_vSender.end(); it++)
	{
		if(*it == NULL)
			continue;
		(*it)->RemoveListener(this);
	}
	//�ͷ���Դ
}