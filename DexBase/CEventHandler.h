

#ifndef _DEX_EVENT_HANDLER_H
#define _DEX_EVENT_HANDLER_H
#include "CListener.h"
class CEventHandler
{
	//事件处理系统，所有处理事件的模块都从此派生
	//拓展新模块时，只须要继承这个类，并且将事件挂接到响应方法上，在方法内部做响应处理
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