
#ifndef _PALGAMESTATE_BATTLE_MAIN_H
#define _PALGAMESTATE_BATTLE_MAIN_H
#include <map>
#include "state/DexGameState.h"
#include "PalTypeDefine.h"

class PalPlayer;

//战斗主状态

class PalBattleStateBase;
class CDexScene;
class PalFightHeadManager;
class PalBatterContent;
class PalNumberManager;
class PalPanelInterface;
class PalBattleStateMachine;
class CDexWidgetImage;
class DexModelBase;
class TestSkinMesh;
class PalGameStateBattleMain: public DexGameState
{
	Dex_DeclareClass(PalGameStateBattleMain,1)
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
		FIGHT_SKILLING,     //施展法术阶段	 、
		FIGHT_LOSE,         //战斗失败
		FIGHT_WIN,           //胜利
	} EBattleStateStage;

protected:
	//CDexScene* m_pScene;
	PalFightHeadManager* m_pFightHeadManager;
	PalNumberManager*    m_pNumberManager;
	PalBatterContent*   m_pBattleSkillContent;
	std::map<string, PalPanelInterface*> m_mapBattlePanels;
	typedef list<PalPlayer*> TlistPlayer;
	TlistPlayer   m_listEnemies;
	TlistPlayer   m_listPlayers;
	DexModelBase* ms3d;
	TestSkinMesh* testMesh;
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
	CDexWidgetImage* g_pImageBackgroud;
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

	void SetApply(bool apply) { m_bApply = apply;}
	bool NeedApply()		  { return m_bApply;}

	//添加我方战斗人员，之后可根据具体战斗加载文件数据
	bool AddPlayers(PalPlayer* player);

	bool AddEnemy(PalPlayer* player);
};

#endif