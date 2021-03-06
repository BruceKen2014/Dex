
#ifndef _PALGAMESTATE_BATTLE_MAIN_H
#define _PALGAMESTATE_BATTLE_MAIN_H
#include <map>
#include "state/DexGameState.h"
#include "DexBase/DexLight.h"
#include "PalTypeDefine.h"
#include "../DexBase/DexDVector.h"

class PalPlayer;

//战斗主状态

class PalBattleStateBase;
class CDexScene;
class PalFightHeadManager;
class PalBatterContent;
class PalNumberManager;
class DexPanelInterface;
class PalBattleStateMachine;
class DexWidgetImage;
class DexModelBase;
class DexSkinMesh;
class DexWidgetProgressBar;
class DexWidgetEditBox;

class PalGameStateBattleMain: public DexGameState
{
	Dex_DeclareClass(PalGameStateBattleMain, DexGameState, 1)
public:
	PalGameStateBattleMain();
	virtual ~PalGameStateBattleMain();

	typedef enum 
	{
		FIGHT_FLAG,         //没有操作
		FIGHT_MOVE_CAMERA,   //摄像机移动阶段
		FIGHT_LABEL,         //进度条移动阶段
		FIGHT_SELECT_ORDER, //选取指令阶段
		FIGHT_SELECT_TARGET,  //选取目标阶段
		FIGHT_ATTACKING,    //正在攻击阶段（包括自己攻击敌人和敌人攻击自己）
		FIGHT_SKILLING,     //施展法术阶段	
		FIGHT_LOSE,         //战斗失败
		FIGHT_WIN,           //胜利
	} EBattleStateStage;

protected:
	//CDexScene* m_pScene;
	PalFightHeadManager* m_pFightHeadManager;
	PalNumberManager*    m_pNumberManager;
	PalBatterContent*   m_pBattleSkillContent;
	std::map<string, DexPanelInterface*> m_mapBattlePanels;
	typedef list<PalPlayer*> TlistPlayer;
	TlistPlayer   m_listEnemies;
	TlistPlayer   m_listPlayers;
	DexSkinMesh* ms3d;
	DexSkinMesh* objModel;
	DexSkinMesh* daeModel;
	DexSkinMesh* daeWeaponModel;
	DexSkinMesh* testMesh;
	DexSkinMesh* mwsModel;
	DexSkinMesh* groundModel;
	DVector<DexSkinMesh*> vecFFMap;
	DexLight light;
	void* test_primitive_vertex;
	void* test_primitive_indice;
	PalBattleStateMachine* m_pBattleMainMachine;
	PalPlayer* panda;
	PalPlayer* g_pJingtian;
	PalPlayer* m_pCurrOperationPlayer; //进入选择战斗阶段时候所操作的角色
	LPDIRECT3DTEXTURE9 m_testTexture;
	LPDIRECT3DSURFACE9 m_testSurface;
	LPDIRECT3DSURFACE9 m_testBufferSurface;
	D3DXMATRIX m_testProjection, m_testProjectionOld;
	DexWidgetImage* g_pImageBackgroud;
	DexWidgetProgressBar* g_pProgressBar;
protected:
	//vertex shader
	LPDIRECT3DVERTEXSHADER9 BasicShader; //顶点着色器指针
	LPD3DXCONSTANTTABLE     BasicConstTable; //常量表指针
	D3DXHANDLE				WVPMatrixHandle;//句柄
	D3DXHANDLE				ColorHandle;
	void InitVertexShader();
	void UpdateVertexShader(DInt32 delta);
	void RenderVertexShader();
	//pixel shader
	LPDIRECT3DPIXELSHADER9 pixelShader;
	LPD3DXCONSTANTTABLE	   pixelConstTable;
	D3DXHANDLE			   ScalarHandle;
	D3DXHANDLE			   Samp0Handle;
	D3DXHANDLE			   Samp1Handle;
	D3DXCONSTANT_DESC	   Samp0Desc;//常量描述结构
	D3DXCONSTANT_DESC	   Samp1Desc;
	void InitPixelShader();
	void UpdatePixelShader(DInt32 delta);
	void RenderPixelShader();

public:
	void setCurrOperationPlayer(PalPlayer* player);
	static PalGameStateBattleMain* getBattleMain();
	PalFightHeadManager* getFightHeadManager() { return m_pFightHeadManager;};
	PalNumberManager* getNumberManager() { return m_pNumberManager;};
	PalPlayer*  getCurrOperationPlayer() { return m_pCurrOperationPlayer;};
	PalBatterContent* getSkillContent() { return m_pBattleSkillContent;};
public:
public:
	bool Test_AddJingtian();
	bool Test_AddEnemy1();
public:		  
	void DrawPlayerBlood();
	void setVisible(string type, bool visible);
	bool getVisible(string type);
	void SetBackImageVisible(DBool visible);
	TlistPlayer getPlayers(EPlayerType type);
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
	virtual void  MouseRUp(int xPos, int yPos);
	virtual void  KeyUp();
	virtual void  KeyDown();
	virtual void  OnDragFiles(const DVector<DString>& FileNames);
	void SetApply(bool apply) { m_bApply = apply;}
	bool NeedApply()		  { return m_bApply;}

	//添加我方战斗人员，之后可根据具体战斗加载文件数据
	bool AddPlayers(PalPlayer* player);

	bool AddEnemy(PalPlayer* player);
};
extern PalGameStateBattleMain* pTestGlobal;
#endif