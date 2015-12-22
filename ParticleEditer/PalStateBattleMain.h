
#ifndef _PALGAMESTATE_BATTLE_MAIN_H
#define _PALGAMESTATE_BATTLE_MAIN_H
#include <map>
#include "state/DexGameState.h"
#include "DexBase/DexLight.h"
#include "PalTypeDefine.h"

class PalPlayer;

//ս����״̬

class PalBattleStateBase;
class CDexScene;
class PalFightHeadManager;
class PalBatterContent;
class PalNumberManager;
class PalPanelInterface;
class PalBattleStateMachine;
class CDexWidgetImage;
class DexModelBase;
class DexSkinMesh;
class PalGameStateBattleMain: public DexGameState
{
	Dex_DeclareClass(PalGameStateBattleMain,1)
public:
	PalGameStateBattleMain();
	virtual ~PalGameStateBattleMain();

	typedef enum 
	{
		FIGHT_FLAG,         //û�в���
		FIGHT_MOVE_CAMERA,   //������ƶ��׶�
		FIGHT_LABEL,         //�������ƶ��׶�
		FIGHT_SELECT_ORDER, //ѡȡָ��׶�
		FIGHT_SELECT_TARGET,  //ѡȡĿ��׶�
		FIGHT_ATTACKING,    //���ڹ����׶Σ������Լ��������˺͵��˹����Լ���
		FIGHT_SKILLING,     //ʩչ�����׶�	 ��
		FIGHT_LOSE,         //ս��ʧ��
		FIGHT_WIN,           //ʤ��
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
	DexSkinMesh* ms3d;
	DexSkinMesh* objModel;
	DexSkinMesh* testMesh;
	DexLight light;
	void* test_primitive_vertex;
	void* test_primitive_indice;
	PalBattleStateMachine* m_pBattleMainMachine;
	PalPlayer* panda;
	PalPlayer* g_pJingtian;
	PalPlayer* m_pCurrOperationPlayer; //����ѡ��ս���׶�ʱ���������Ľ�ɫ
	LPDIRECT3DTEXTURE9 m_testTexture;
	LPDIRECT3DSURFACE9 m_testSurface;
	LPDIRECT3DSURFACE9 m_testBufferSurface;
	D3DXMATRIX m_testProjection, m_testProjectionOld;
	CDexWidgetImage* g_pImageBackgroud;
protected:
	//vertex shader
	LPDIRECT3DVERTEXSHADER9 BasicShader; //������ɫ��ָ��
	LPD3DXCONSTANTTABLE     BasicConstTable; //������ָ��
	D3DXHANDLE				WVPMatrixHandle;//���
	D3DXHANDLE				ColorHandle;
	void InitVertexShader();
	void UpdateVertexShader(int32 delta);
	void RenderVertexShader();
	//pixel shader
	LPDIRECT3DPIXELSHADER9 pixelShader;
	LPD3DXCONSTANTTABLE	   pixelConstTable;
	D3DXHANDLE			   ScalarHandle;
	D3DXHANDLE			   Samp0Handle;
	D3DXHANDLE			   Samp1Handle;
	D3DXCONSTANT_DESC	   Samp0Desc;//���������ṹ
	D3DXCONSTANT_DESC	   Samp1Desc;
	void InitPixelShader();
	void UpdatePixelShader(int32 delta);
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

	//����ҷ�ս����Ա��֮��ɸ��ݾ���ս�������ļ�����
	bool AddPlayers(PalPlayer* player);

	bool AddEnemy(PalPlayer* player);
};

#endif