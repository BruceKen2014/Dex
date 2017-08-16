
#ifndef _PAL2GAMESTATE_BATTLE_H
#define _PAL2GAMESTATE_BATTLE_H

#include "DexBase/typedefine.h"
#include "DexBase/DexLog.h"
#include "state/DexGameState.h"
#include "widget/DexGuiStruct.h"
#include "DexBase/DexTerrain.h"

#include "PalNpc.h"
//#include "DexBase/CEventHandler.h"
#include <vector>


class CDexEffectEmitInstance;
class CDexParticalEmit;
class CDexSceneObject;
class DexSceneObjectActionCatmull;
class CDexPieceEffectInstance;
class CDexSceneObjectFlower;
class CDexScene;

class DexObject;
class DexWidgetEditBox;

class CDexModelXAni;
class TestPanel;

class DexWidgetImage;
class CDexWidgetImageSequence;
class CLoadingState: public DexGameState
{
	Dex_DeclareClass(CLoadingState, DexGameState,1)
public:
	DexWidgetImage*         g_pImageBackgroud;
	CDexWidgetImageSequence* g_pImage;

public:
	CLoadingState();
	virtual ~CLoadingState();

public:
	virtual bool ApplyRes() ;  
	virtual void ReleaseRes();  

	virtual bool Update(int delta);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();

};

class ParticalEditState: public DexGameState
{
	Dex_DeclareClass(ParticalEditState, DexGameState,1)
	friend class TestPanel;
public:
	ParticalEditState();
	virtual ~ParticalEditState();

private:
	std::vector<D3DXVECTOR3>  m_points; //测试 catmullrom 曲线
	float factor;
	float curr_delta;  

protected:
	typedef list<CPalNpc*> TlistNpc;
	TlistNpc      m_listSceneNpc;
public:
	bool AddNpc(CPalNpc* npc);
	CPalNpc* getClickNpc(const stRay& ray); //传入一条射线 传出该射线射到的NPC

protected:
	CDexEffectEmitInstance *g_pEmittInstance;
	CDexParticalEmit* particalMgr; 
	CDexPieceEffectInstance* pieceEffect;
	CDexSceneObject* g_Object;
	CDexSceneObjectFlower* m_pFlower;

	DexSceneObjectActionCatmull* action ;
	CDexTerrain* g_pTerrain;

	DexWidgetEditBox* m_editBox;

	D3DXVECTOR3 g_TerrainClickPoint;
	int g_currRow;
	int g_currCol;
	CDexSceneObjectModel* g_pJingtian;

	//CDexModelXAni* g_modelJingtian;

	


public:		  
	//virtual void OnEvent(stEvent event);
	virtual bool ApplyRes() ;  
	virtual void ReleaseRes();  

	virtual bool Update(int delta);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();
	virtual void  MouseWheelFront();
	virtual void  MouseWheelBack();
	virtual void  MouseMove(int xPos, int yPos);
	virtual void  MouseLDown(int xPos, int yPos);
	virtual void  MouseLUp(int xPos, int yPos);
	virtual void  MouseRDown(int xPos, int yPos);
	virtual void  MouseRUp(int xPos, int yPos);
	virtual void  KeyUp();
	virtual void  KeyDown();
	void OnEditTextUpdate(CEventHandler* sys, DexObject* object,stEvent event);

	void OnEventClickTalkSystem(CEventHandler* sys, DexObject* object,stEvent event);//测试事件系统

	void SetApply(bool apply) { m_bApply = apply;}
	bool NeedApply()		  { return m_bApply;}

	

public:
	HWND m_hWndPropDlg;  //属性dialog 窗口
};

class TestPanel:public CEventHandler
{

public:
	TestPanel();
	virtual ~TestPanel();
	ParticalEditState* m_State;
	DexWidgetEditBox* m_editBox;
	void OnEditTextUpdate(DexObject* object,stEvent event);

	bool Init();
	void Render();
	void Update(int delta);
};
#endif