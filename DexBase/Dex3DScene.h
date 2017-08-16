

#ifndef _DEX_SCENE_H
#define _DEX_SCENE_H

#include <map>
#include "DexObject.h"
#include "DexColor.h"

class CDexSceneObject;
class CDextModel;
class DexCollideObject;
class CDexTerrain;
extern int Lua_Scene_SetFogColor(lua_State* L);
extern int Lua_Scene_SetFog(lua_State* L);
extern int Lua_Scene_SetClearColor(lua_State* L);
extern int Lua_Scene_AddEffect(lua_State* L);
extern int Lua_Scene_AddFlower(lua_State* L);
extern int Lua_Scene_AddModel(lua_State* L);//只Add 靜態模型
extern int Lua_Scene_SetPos(lua_State* L);
class CDexScene:public DexObject
{
	Dex_DeclareClass(CDexScene, DexObject, 1)
public:
	CDexScene();
	virtual ~CDexScene();

protected:
	//以下數據根據劇情不同，需要從腳本中動態加載
	typedef std::map<DInt32,CDexSceneObject*> TMapObject;
	TMapObject  m_listObject;
	TMapObject  m_listObjectAlpha;   //需要alpha排序的object	
	bool m_bRender;
	bool m_bRenderCollideObject;     
	bool m_bFog;
	DexColor    m_clearColor;  //当前场景的背景清楚色      
	stFog       m_stFog;

	typedef std::list<DexCollideObject*>  TListCollideObject;
	TListCollideObject m_listCollideObject;
	CDexTerrain* m_pTerrain; //當前場景的地圖指針
protected:
	void _sortAlphaObject();
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
public:	
	virtual bool BeginScene() { return true;};  //设置相关参数之后，调用此函数
	virtual bool Update(int delta);
	virtual bool Render();
	virtual void Reset();
	static void RegisterLuaFunction(lua_State* L);
	bool registerObject(CDexSceneObject* object);
	bool unRegisterObject(CDexSceneObject* object); 
	void setRender(bool render);
	void setClearColor(const DexColor& color);
	const DexColor& getClearColor() const;
	void addCollideObject(DexCollideObject* object);
	void setTerrain(CDexTerrain* terrain);
	CDexTerrain* getTerrain();
	bool LoadTerrainData(const char* terrainfile);
	
	//fog
	void DisableFog();
	void EnableFog();
	void SetFogColor(const DexColor& color);
	void SetFogMode(DEMOEX_FOG_MODE mode);
	void SetFogStart(float start);
	void SetFogEnd(float end);
	void SetFogDensity(float density);
	void DisableRangeFog();
	void EnableRangeFog();

	bool ExeScript(const char* scriptfile) ;
	bool LoadModelInit(const char* initfile);       //加載該場景要用到的模型
	bool LoadCollideInit(const char* initfile)    ; //加載該場景的碰撞信息
	bool SaveCollideInit(const char* initfile)    ; //將碰撞信息保存到文件中

public:
	CDexSceneObject* m_bAddObject; //剛剛添加的object 用作調試添加object使用
};

#endif