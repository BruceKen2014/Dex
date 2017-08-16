

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
extern int Lua_Scene_AddModel(lua_State* L);//ֻAdd �o�Bģ��
extern int Lua_Scene_SetPos(lua_State* L);
class CDexScene:public DexObject
{
	Dex_DeclareClass(CDexScene, DexObject, 1)
public:
	CDexScene();
	virtual ~CDexScene();

protected:
	//�������������鲻ͬ����Ҫ���_���ЄӑB���d
	typedef std::map<DInt32,CDexSceneObject*> TMapObject;
	TMapObject  m_listObject;
	TMapObject  m_listObjectAlpha;   //��Ҫalpha�����object	
	bool m_bRender;
	bool m_bRenderCollideObject;     
	bool m_bFog;
	DexColor    m_clearColor;  //��ǰ�����ı������ɫ      
	stFog       m_stFog;

	typedef std::list<DexCollideObject*>  TListCollideObject;
	TListCollideObject m_listCollideObject;
	CDexTerrain* m_pTerrain; //��ǰ�����ĵ؈Dָ�
protected:
	void _sortAlphaObject();
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
public:	
	virtual bool BeginScene() { return true;};  //������ز���֮�󣬵��ô˺���
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
	bool LoadModelInit(const char* initfile);       //���dԓ����Ҫ�õ���ģ��
	bool LoadCollideInit(const char* initfile)    ; //���dԓ��������ײ��Ϣ
	bool SaveCollideInit(const char* initfile)    ; //����ײ��Ϣ���浽�ļ���

public:
	CDexSceneObject* m_bAddObject; //������ӵ�object �����{ԇ���objectʹ��
};

#endif