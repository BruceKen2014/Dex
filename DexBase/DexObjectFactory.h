
#ifndef _DEX_OBJECT_FACTORY_H
#define _DEX_OBJECT_FACTORY_H

#include "typedefine.h"
#include <map>
#include <list>

class CDexObject;

/*
object factory ������object���ϼң�����object���������������������ص����
���ǹ�����ô���ڣ���object���ڵ����ϵͳȥ���ڡ�
*/

#define Dex_registerClass(clas)
class CDexObjectFactroy
{
private:
	CDexObjectFactroy();
	~CDexObjectFactroy();

private:
	static CDexObjectFactroy* g_pObjectFactory;
	int    g_iObjectCount;
	typedef std::map<std::string, std::list<CDexObject*>> TMapObject; //string����� list���ф�����ԓČ�����l
	TMapObject g_Objects;
	void Initialize();  
private:
	CDexObject* createObject(std::string classType);
public:
	static CDexObjectFactroy* getObjectFactory();
	CDexObject* queryObject(std::string classType);
	void allocateObject(CDexObject* object);
	int32 getObjectId();
};

#define Dex_QueryObject(clas)  (clas*)CDexObjectFactroy::getObjectFactory()->queryObject(clas::getClassType())
#endif