
#ifndef _DEX_OBJECT_FACTORY_H
#define _DEX_OBJECT_FACTORY_H

#include "typedefine.h"
#include <map>
#include <list>

class DexObject;

/*
object factory 是所有object的老家，所有object从这里出生，生命的最后会回到这里，
但是过程怎么折腾，由object所在的相关系统去折腾。
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
	typedef std::map<std::string, std::list<DexObject*>> TMapObject; //string為類名 list是已創建的該類的實例鏈錶
	TMapObject g_Objects;
	void Initialize();  
private:
	DexObject* createObject(std::string classType);
public:
	static CDexObjectFactroy* getObjectFactory();
	DexObject* queryObject(std::string classType);
	void allocateObject(DexObject* object);
	DInt32 getObjectId();
};

#define Dex_QueryObject(clas)  (clas*)CDexObjectFactroy::getObjectFactory()->queryObject(clas::getClassType())
#endif