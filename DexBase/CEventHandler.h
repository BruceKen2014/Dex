

#ifndef _DEX_EVENT_HANDLER_H
#define _DEX_EVENT_HANDLER_H
#include "CListener.h"
class CEventHandler
{
	//�¼�����ϵͳ�����д����¼���ģ�鶼�Ӵ�����
	//��չ��ģ��ʱ��ֻ��Ҫ�̳�����࣬���ҽ��¼��ҽӵ���Ӧ�����ϣ��ڷ����ڲ�����Ӧ����
protected:
	string m_name;

public:
	CEventHandler();
	virtual ~CEventHandler();

//public:
//	virtual bool Init() = 0;
//	virtual void Render()=0;
//	virtual void Update(int delta) = 0;
//	virtual void ShutDown()= 0;
};
#endif