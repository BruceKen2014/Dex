

#include "typedefine.h"
#include "DexLog.h"
#include "Dex3DScene.h"
#include "DexCollideObject.h"
#include "DexObjectFactory.h"
#include "CDexPartical.h"
#include "DexSceneObject.h"
#include "DexSceneObjectFlower.h"
#include "DexSceneObjectModel.h"
#include "DexTerrain.h"
#include "../Source/CCommandScript.h"
#include "../state/DexGameEngine.h"
#include "../state/DexGameState.h"

int Lua_Scene_SetFogColor(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	double r =(double) lua_tonumber(L,1);
	double g =(double) lua_tonumber(L,2);
	double b =(double) lua_tonumber(L,3);
	double a =(double) lua_tonumber(L,4);
	pScene->SetFogColor(DexColor(r,g,b,a));
	return 0;
}
int Lua_Scene_SetFog(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	int b = lua_tointeger(L,1);
	if(b)
		pScene->EnableFog();
	else
		pScene->DisableFog();
	return 0;
}
int Lua_Scene_SetClearColor(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	double r =(double) lua_tonumber(L,1);
	double g =(double) lua_tonumber(L,2);
	double b =(double) lua_tonumber(L,3);
	double a =(double) lua_tonumber(L,4);
	pScene->setClearColor(DexColor(r,g,b,a));
	return 0;
}
int Lua_Scene_AddEffect(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	char str[64];
	int type = lua_tointeger(L,1);
	int posX = lua_tointeger(L,2);
	int posY = lua_tointeger(L,3);
	int posZ = lua_tointeger(L,4);
	strcpy(str, lua_tostring(L,5));
	
	if(type == 1)
	{//emitter instance
		CDexEffectEmitInstance* instance = Dex_QueryObject(CDexEffectEmitInstance);
		if(instance == NULL) return 0;
		if(!instance->LoadInitFile(str)) 
		{
			_SafeRelease(instance);
			return 0;
		}
		instance->setPosition(D3DXVECTOR3(posX, posY, posZ));
		pScene->registerObject(instance);
	}
	else if(type == 2)
	{//piece effect
		CDexPieceEffectInstance* instance = Dex_QueryObject(CDexPieceEffectInstance);
		if(instance == NULL) return 0;
		if(!instance->LoadInitFile(str)) 
		{
			_SafeRelease(instance);
			return 0;
		}
		instance->setPosition(D3DXVECTOR3(posX, posY, posZ));
		pScene->registerObject(instance);
	}
	return 0;
}
int Lua_Scene_AddFlower(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	char str[64];
	int posX = lua_tointeger(L,1);
	int posY = lua_tointeger(L,2);
	int posZ = lua_tointeger(L,3);
	double scaleX = (double)lua_tonumber(L,4);
	double scaleY = (double)lua_tonumber(L,5);
	double scaleZ = (double)lua_tonumber(L,6);
	strcpy(str, lua_tostring(L,7));

	CDexSceneObjectFlower* instance = Dex_QueryObject(CDexSceneObjectFlower);
	instance->SetTexture(str);
	instance->setPosition(D3DXVECTOR3(posX, posY, posZ));
	instance->scale(D3DXVECTOR3(scaleX,scaleY,scaleZ));
	pScene->registerObject(instance);
	return 0;
}
int Lua_Scene_SetPos(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	CDexSceneObject* object = pScene->m_bAddObject;
	if(object == NULL)	return 0;
	int posX = lua_tointeger(L,1);
	int posY = lua_tointeger(L,2);
	int posZ = lua_tointeger(L,3);

	object->setPosition(D3DXVECTOR3(posX, posY, posZ));
	return 0;
}
int Lua_Scene_AddModel(lua_State* L)
{
	CDexScene* pScene = DexGameEngine::getEngine()->GetCurrState()->getStateScene();
	if(pScene == NULL)	return 0;
	char str[64];
	char axisStr[16];
	strcpy(str, lua_tostring(L,1));
	int posX = lua_tointeger(L,2);
	int posY = lua_tointeger(L,3);
	int posZ = lua_tointeger(L,4);
	double scaleX = (double)lua_tonumber(L,5);
	double scaleY = (double)lua_tonumber(L,6);
	double scaleZ = (double)lua_tonumber(L,7);
	strcpy(axisStr, lua_tostring(L,8));
	double degree = (double)lua_tonumber(L,9);
	int alpha = lua_tointeger(L,10);
	CDexSceneObjectModel* model = Dex_QueryObject(CDexSceneObjectModel);
	if(model == NULL)	return 0;
	if(!(model->LoadModelFile(str, false)))	return 0;
	CDexSceneObject::AXIS axis = CDexSceneObject::AXIS_NONE;
	if(strcmp(axisStr, "X") == 0)
		axis = CDexSceneObject::AXIS_X;
	else if(strcmp(axisStr, "Y") == 0)
		axis = CDexSceneObject::AXIS_Y;
	else if(strcmp(axisStr, "Z") == 0)
		axis = CDexSceneObject::AXIS_Z;
	model->setCurrMatrix(D3DXVECTOR3(posX,posY,posZ),D3DXVECTOR3(scaleX,scaleY,scaleZ),axis,_getRadian(degree));
	model->setAlphaSort(alpha);
	pScene->registerObject(model);
	return 0;
}
void CDexScene::RegisterLuaFunction(lua_State* L)
{
	lua_register(L, "SetFog", Lua_Scene_SetFog);
	lua_register(L, "SetFogColor", Lua_Scene_SetFogColor);
	lua_register(L, "SetClearColor", Lua_Scene_SetClearColor);
	lua_register(L, "AddEffect", Lua_Scene_AddEffect);
	lua_register(L, "AddFlower", Lua_Scene_AddFlower);
	lua_register(L, "SetPos", Lua_Scene_SetPos);
	lua_register(L, "AddModel", Lua_Scene_AddModel);
	

}
CDexScene::CDexScene()
{
	m_bRender = true;
	m_bRenderCollideObject = true;
	m_bFog = false;
	m_clearColor = DexColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_bAddObject = NULL;
	m_pTerrain = NULL;
}
CDexScene::~CDexScene()
{

}

void CDexScene::_sortAlphaObject()
{

}

bool CDexScene::Update(int delta)
{
	DEX_ENSURE_B(CDexObject::Update(delta));
	_sortAlphaObject();
	{
		foreach(TMapObject, ite, m_listObject)
		{
			if(ite->second != NULL )
				ite->second->Update(delta);
		}
	}
	{
		foreach(TMapObject, ite, m_listObjectAlpha)
		{
			if(ite->second != NULL )
				ite->second->Update(delta);
		}
	}
	return true;
}
bool CDexScene::Render()
{
	DEX_ENSURE_B(m_bRender);
	if(m_bRenderCollideObject)
	{
		foreach(TListCollideObject, ite, m_listCollideObject)
		{
			if((*ite) != NULL )
				(*ite)->RenderMesh();
		}
	}
	{
		foreach(TMapObject, ite, m_listObject)
		{
			if(ite->second != NULL )
				ite->second->Render();
		}
	}
	{
		foreach(TMapObject, ite, m_listObjectAlpha)
		{
			if(ite->second != NULL )
				ite->second->Render();
		}
	}
	return true;
}

void CDexScene::Reset()
{
	m_bRender = true;
	m_bRenderCollideObject = true;
	m_bFog = false;
	m_clearColor = DexColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_bAddObject = NULL;
	m_pTerrain = NULL;
	CDexObject::Reset();
	{
		foreach(TListCollideObject, ite, m_listCollideObject)
		{
			_SafeRelease(*ite);
		}
	}
	{
		foreach(TMapObject, ite, m_listObject)
		{
			_SafeRelease(ite->second);
		}
	}
	{
		foreach(TMapObject, ite, m_listObjectAlpha)
		{
			_SafeRelease(ite->second);
		}
	}
	if(m_pTerrain != NULL)
		m_pTerrain->Reset();
}

bool CDexScene::registerObject(CDexSceneObject* object)
{
	DEX_ENSURE_B(object);
	m_bAddObject = object;
	{//处理所有attach object
		foreach(CDexSceneObject::TlistObjects, ite, object->m_listAttachObjects)
		{
			registerObject((*ite));
		}
	}
	if(object->getAlphaSort())
	{
		TMapObject::iterator ite= m_listObjectAlpha.find(object->getObjectId());
		if(ite != m_listObjectAlpha.end())
		{
			DexLog::getSingleton()->BeginLog();
			dex_log_current_f_l(log_error);
			DexLog::getSingleton()->Log(log_error, "CDexScene::registerObject试图注册一个已经存在于场景的object!");
			DexLog::getSingleton()->EndLog();
			return false;
		}
		m_listObjectAlpha[object->getObjectId()] =  object;
	}
	else
	{
		TMapObject::iterator ite= m_listObject.find(object->getObjectId());
		if(ite != m_listObject.end())
		{
			DexLog::getSingleton()->BeginLog();
			dex_log_current_f_l(log_error);
			DexLog::getSingleton()->Log(log_error, "CDexScene::registerObject试图注册一个已经存在于场景的object!");
			DexLog::getSingleton()->EndLog();
			return false;
		}
		m_listObject[object->getObjectId()] =  object;
	}
	object->m_pScene = this;
	return true;
}


bool CDexScene::unRegisterObject(CDexSceneObject* object)
{
	DEX_ENSURE_B(object);
	{//处理所有attach object
		foreach(CDexSceneObject::TlistObjects, ite, object->m_listAttachObjects)
		{
			unRegisterObject((*ite));
		}
	}
	
	if(object->getAlphaSort())
	{
		TMapObject::iterator ite= m_listObjectAlpha.find(object->getObjectId());
		if(ite == m_listObjectAlpha.end())
		{
			DexLog::getSingleton()->BeginLog();
			dex_log_current_f_l(log_allert);
			DexLog::getSingleton()->Log(log_allert, "CDexScene::unRegisterObject试图反注册一个不存在于场景的object!");
			DexLog::getSingleton()->EndLog();
			return false;
		}
		m_listObjectAlpha.erase(ite);
	}
	else
	{
		TMapObject::iterator ite= m_listObject.find(object->getObjectId());
		if(ite == m_listObject.end())
		{
			DexLog::getSingleton()->BeginLog();
			dex_log_current_f_l(log_allert);
			DexLog::getSingleton()->Log(log_allert, "CDexScene::unRegisterObject试图反注册一个不存在于场景的object!");
			DexLog::getSingleton()->EndLog();
			return false;
		}
		m_listObject.erase(ite);
	}
	object->m_pScene = NULL;
	return true;
}

void CDexScene::setRender(bool render)
{
	m_bRender = render;
}

void CDexScene::setClearColor(const DexColor& color)
{
	m_clearColor = color;
}


const DexColor& CDexScene::getClearColor() const
{
	return m_clearColor;
}

void CDexScene::DisableFog()
{
	m_bFog = false;
	DexGameEngine::getEngine()->DisableFog();
}

void CDexScene::EnableFog()
{
	m_bFog = true;
	DexGameEngine::getEngine()->SetFogColor(m_stFog.m_fogColor);
	DexGameEngine::getEngine()->SetFogMode(m_stFog.m_fogMode);
	DexGameEngine::getEngine()->SetFogStart(m_stFog.m_fogStart);
	DexGameEngine::getEngine()->SetFogEnd(m_stFog.m_fogEnd);
	DexGameEngine::getEngine()->SetFogDensity(m_stFog.m_fogdensity);
	if(m_stFog.m_rangerFog)
		DexGameEngine::getEngine()->EnableRangeFog();
	else
		DexGameEngine::getEngine()->DisableRangeFog();
	DexGameEngine::getEngine()->EnableFog();
}

void CDexScene::SetFogColor(const DexColor& color)
{
	m_stFog.m_fogColor = color;
	DexGameEngine::getEngine()->SetFogColor(color);
}

void CDexScene::SetFogMode(DEMOEX_FOG_MODE mode)
{
	m_stFog.m_fogMode = mode;
	DexGameEngine::getEngine()->SetFogMode(mode);
}

void CDexScene::SetFogStart(float start)
{
	m_stFog.m_fogStart = start;
	DexGameEngine::getEngine()->SetFogStart(start);
}

void CDexScene::SetFogEnd(float end)
{
	m_stFog.m_fogEnd = end;
	DexGameEngine::getEngine()->SetFogEnd(end);
}

void CDexScene::SetFogDensity(float density)
{
	m_stFog.m_fogdensity = density;
	DexGameEngine::getEngine()->SetFogDensity(density);
}

void CDexScene::DisableRangeFog()
{
	m_stFog.m_rangerFog = false;
	DexGameEngine::getEngine()->DisableRangeFog();
}

void CDexScene::EnableRangeFog()
{
	m_stFog.m_rangerFog = true;
	DexGameEngine::getEngine()->EnableRangeFog();
}

void CDexScene::addCollideObject(DexCollideObject* object)
{
	m_listCollideObject.push_back(object);
}

void CDexScene::setTerrain(CDexTerrain* terrain)
{
	DEX_ENSURE(m_pTerrain != NULL);
	m_pTerrain = terrain;
}
CDexTerrain* CDexScene::getTerrain()
{
	return m_pTerrain;
}
bool CDexScene::LoadTerrainData(const char* terrainfile)
{
	DEX_ENSURE_B(m_pTerrain != NULL);
	m_pTerrain->Reset();
	return m_pTerrain->InitFromFile(terrainfile);
}

bool CDexScene::ArchiveIn(DexMem& mem)
{
	return true;
}

bool CDexScene::ArchiveOut(DexMem& mem) const
{
	return true;
}

bool CDexScene::ExeScript(const char* scriptfile)
{
	return true;
}

bool CDexScene::LoadModelInit(const char* initfile)
{
	return true;
}

bool CDexScene::LoadCollideInit(const char* initfile)
{
	DexLog::getSingleton()->BeginLog();
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(initfile))
	{
		DexLog::getSingleton()->Log(log_allert, "open scene collide init file %s failed!\n",initfile );
		DexLog::getSingleton()->EndLog();
		pScript->CloseScript();
		return false;
	}
	int type, posX, posY, posZ, width, height, depth, radius;
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		type = pScript->GetIntParam();
		if(type == 0)
		{//sphere
			posX = pScript->GetIntParam();	
			posY = pScript->GetIntParam();
			posZ = pScript->GetIntParam();
			radius = pScript->GetIntParam();
			DexCollideObjectSphere* sphere = Dex_QueryObject(DexCollideObjectSphere);
			sphere->Initialize(D3DXVECTOR3(posX, posY, posZ), radius);
			m_listCollideObject.push_back(sphere);
		}
		else
		{//box
			posX = pScript->GetIntParam();	
			posY = pScript->GetIntParam();
			posZ = pScript->GetIntParam();
			width = pScript->GetIntParam();
			height = pScript->GetIntParam();
			depth = pScript->GetIntParam();
			DexCollideObjectBox* box = Dex_QueryObject(DexCollideObjectBox);
			box->Initialize(D3DXVECTOR3(posX, posY, posZ),D3DXVECTOR3(width, height, depth));
			m_listCollideObject.push_back(box);
		}
	}
	pScript->CloseScript();
	DexLog::getSingleton()->Log(log_ok, "open scene collide init file %s ok !\n ",initfile );
	DexLog::getSingleton()->EndLog();
	return true;
}

bool CDexScene::SaveCollideInit(const char* initfile)
{
	return true;
}