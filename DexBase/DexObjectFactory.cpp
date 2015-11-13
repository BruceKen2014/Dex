

#include "DexObjectFactory.h"
#include "DexSceneObject.h"
#include "DexCollideObject.h"
#include "DexSceneObjectModel.h"
#include "DexSceneObjectAction.h"
#include "DexSceneObjectFlower.h"
#include "DexTerrain.h"
#include "CDexPartical.h"
#include "DexLog.h"


CDexObjectFactroy* CDexObjectFactroy::g_pObjectFactory = NULL;

CDexObjectFactroy::CDexObjectFactroy()
{
	g_iObjectCount = 0;
}

 CDexObjectFactroy* CDexObjectFactroy::getObjectFactory()
 {
	 if(g_pObjectFactory == NULL)
	 {
		 g_pObjectFactory = new CDexObjectFactroy;
		 g_pObjectFactory->Initialize();
	 } 
	 return g_pObjectFactory;
 }
#define			registerObject(Object) \
 init_count = Object::getClassInst();\
 for(int i = 0 ; i < init_count/2; i++)\
 {\
 Object * obj = new Object();\
 g_Objects[Object::getClassType()].push_back(obj);\
 }\
 getLog()->Log(log_ok, "init object %s total_count:%d pre_inicount: %d ok\n", Object::getClassType().c_str(), init_count, init_count/2);

#define check_new_object(clas)	if(classType == clas::getClassType())	{ object = new clas();goto create_over;  }
 void CDexObjectFactroy::Initialize()
 {
	 int init_count= 0;
	getLog()->BeginLog();
	//scene object action
	registerObject(DexSceneObjectActionCatmull);
	registerObject(DexSceneObjectActionRotate);
	//scene collide object
	registerObject(DexCollideObjectBox);
	registerObject(DexCollideObjectSphere);
	//scene object
	registerObject(CDexParticalEmit);
	registerObject(CDexTerrain);
	registerObject(CDexPieceEffectInstance);
	registerObject(CDexEffectEmitInstance);
	registerObject(CDexSceneObjectFlower);
	registerObject(CDexSceneObjectModel);
	//other object
	registerObject(CDexModelX);
	registerObject(CDexModelXAni);
	registerObject(CDexModelFactory);
	getLog()->EndLog();
 }
CDexObjectFactroy::~CDexObjectFactroy()
{

}

CDexObject* CDexObjectFactroy::queryObject(std::string classType)
{
	getLog()->BeginLog();
	if(g_Objects.find(classType) != g_Objects.end())
	{//工廠中有實例
		foreach(std::list<CDexObject*>, ite, g_Objects[classType])
		{
			if(!(*ite)->GetValid())
			{
				(*ite)->SetValid(true);
				getLog()->Log(log_ok, "query Object %s ok\n", classType.c_str());
				getLog()->EndLog();
				return (*ite);
			}
		}
	};
	//沒有實例或者實例都有效,重新新建一個
	getLog()->Log(log_ok, "query Object %s failed, try to create a new one!\n", classType.c_str());
	getLog()->EndLog();
	return createObject(classType);
}

void CDexObjectFactroy::allocateObject(CDexObject* object)
{
	DEX_ENSURE(object != NULL);
	getLog()->BeginLog();
	string type = object->getType();
	if(g_Objects.find(object->getType()) == g_Objects.end())
	{
		getLog()->Log(log_allert, "object factory allocate a unknown object %s, now delete it! \n",type.c_str());
		_SafeDelete(object);
		getLog()->EndLog();
		return;
	};
	foreach(std::list<CDexObject*>, ite, g_Objects[type])
	{
		if((*ite) == object)
		{
			if(g_Objects[type].size() > object->getInst())
			{
				_SafeDelete(*ite);
				g_Objects[type].erase(ite);
				getLog()->Log(log_ok, "object factory allocate  object %s ok , two many objects , now delete it!\n",type.c_str());
				getLog()->EndLog();
			}
			else
			{
				(*ite)->Reset();
				getLog()->Log(log_ok, "object factory allocate  object %s ok\n",type.c_str());
				getLog()->EndLog();
			}
			return;
		}
	}
	getLog()->Log(log_allert, "object factory allocater find a wild object %s ! now delete it!\n",type.c_str());
	_SafeDelete(object);
	getLog()->EndLog();
}

CDexObject* CDexObjectFactroy::createObject(std::string classType)
{
	CDexObject* object = NULL;
	getLog()->BeginLog();
	//scene object action
	check_new_object(DexSceneObjectActionCatmull);
	//scene collide object
	check_new_object(DexCollideObjectBox);
	check_new_object(DexCollideObjectSphere);
	//scene object
	check_new_object(CDexParticalEmit);
	check_new_object(CDexTerrain);
	check_new_object(CDexPieceEffectInstance);
	check_new_object(CDexEffectEmitInstance);
	check_new_object(CDexSceneObjectFlower);
	check_new_object(CDexSceneObjectModel);
	//other object
	check_new_object(CDexModelX);
	check_new_object(CDexModelXAni);
	check_new_object(CDexModelFactory);
create_over:
	if(object == NULL)
	{
		getLog()->Log(log_error, "		create object %s failed!\n", classType.c_str());
		getLog()->EndLog();
		return NULL;
	}
	getLog()->Log(log_ok, "		create object %s ok!\n", classType.c_str());
	getLog()->EndLog();
	object->SetValid(true);
	g_Objects[classType].push_back(object);
	return object;
}

int32 CDexObjectFactroy::getObjectId()
{
	return g_iObjectCount++;
}