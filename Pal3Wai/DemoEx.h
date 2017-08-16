

#pragma  once 

#include "../Source/CEngine_h.h"	
#include "../Geometry/CCollideShpere.h"
#include "../Geometry/CCollideBox.h"
#include "CRoad.h"
#include "CLadder.h"
#include "CFun.h"
#include "CEnemy.h"
#include "PalDefine.h"
#include "CFightLabel.h"
#include "CSkill.h"
#include "CBloodFlag.h"
#include "CTestPanel.h"
#include "../Expression/DexScriptMgr.h"
#include "PalScriptFun.h"
#include "CDexQuad.h"
#include "CTestSprite.h"
#include "../DexBase/DexCollideObject.h"

#include <map>

int viewX = 0;
int viewY = 0;


HWND       g_hwnd;						//�����ھ��
HINSTANCE  g_hInstance;				    //ʵ��

HANDLE       g_LoadThread;    //����ʹ��һ���߳���������Դ�ļ��� ���ͬʱ��ʾ���ؽ���
DWORD        g_LoadThreadId;  //load thread id
DWORD WINAPI LoadThread1(LPVOID lpParam);	//here load resouce


LPDIRECT3D9 g_D3D = NULL;				//D3D
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;   //�豸

D3DXMATRIX g_projection;
D3DXMATRIX g_ViewMatrix;
D3DXMATRIX g_worldMatrix;  //�����������
CCamera g_camera;
lua_State *L = NULL;

PAL_STATE   g_palState; //��״̬
PAL2_STATE	g_State2;	//��״̬
DexVector3    g_crossPoint = DexVector3(0, 0, 0);
CEnemy  *  g_Jintian = NULL;
CTerrain*     g_terrain = NULL;

CPick*         g_pPick = NULL;
int         g_clickId;  //����������ID
int  g_iXMouse, g_iYMouse;
size_t      g_clickVIndex;  //�������NPC�������е�����
int         g_task = 0;	//��ʱ������

CGuiSystem   *g_pGuiInterface = NULL; //����GUI
CGuiSystem   *g_pGuiTown = NULL;	 //����GUI()
//CGuiSystem   *g_pGuiOurdoors = NULL;	 //Ұ��GUI
CGuiSystem   *g_pGuiFight = NULL; //ս��GUI

CGuiSystem*   g_pGuiFightInoBack = NULL;  //ս��GUI��Ϣ��ʾ����Ѫ����������

CWaterF*      g_water = NULL;
CDexScriptMgr*  g_pScriptMgr = NULL;

bool          g_isTownNpc = false;

HCURSOR       g_Cursors[3];



std::vector<CModel*>  g_modelVector;   //���������е�ģ������
std::vector<CModelAlpha*>  g_alphaModelVector;   //����alpha���Ե�ģ��(�������ӵ�)��Z���򽫸�����Щ������������ľ����������
std::vector<CModelXAni*>  g_aniModelVector;	    //���ж�̬���Ե�����
std::vector<CNpc*>  g_npcVector;	   //npc
std::vector<CCollideSphere*>  g_collideSphere; //�����ײ��
std::vector<CCollideBox*>  g_collideBox; //�����ײ������
std::vector<CSoundObject*> g_sndObject;  //��������(���桢��·���š�����)
std::vector<CParticalSystem*> g_partical;  //����ϵͳ����ѩ������

std::vector<CNumberAni*>  g_FightNumbers;   //ս��ϵͳ�е����֣��ⲿ������ӣ��ڲ�ʱ�䵽��֮���Զ�ɾ��


CBloodLabel * g_BloodLabel = NULL;
CTextureMove*   g_LoseTex = NULL;


std::vector<CNpc*>  g_npcVector2;	   //Ұ�����   TODO
std::vector<CEnemy*> g_enemyVector;    //ս��ϵͳ�еĵ���

CSpell * spell = NULL;

CSkyBox       *g_pSky = NULL;

std::map<int, CTextureEx*>  g_headmap ;
bool             g_OpenFog = false;  //�Ƿ�����Ч
bool             g_isFightComplete = true;  //ʩ���Ƿ�������⽫����gui�Ƿ���ʾ���Ƿ���Ӧ��Ϣ

D3DXVECTOR3      g_playPos;   //�л���ս��״̬ʱ���澰��λ�ã�ս������ʱ�л�����
D3DXVECTOR3      g_playDir;   //���泯��

D3DXVECTOR3      g_cameraPos; //���������λ��

int g_fps = 0;
int g_counter = 0;
DInt64 g_currentTime = 0.0f;
DInt64 g_lastTime    = 0.0f;

bool  g_isSelectEnemy = false;	 //�Ƿ�����ѡ�񹥻��ĵ���
bool  g_isAttacking = false;    //�Ƿ����ڹ���

int   g_FightStage = FIGHT_FLAG;  //ս���׶�

int   g_FightEndDelay = 0;   //ս��ʧ�ܺ��ʱ���ӳ�

CTextureEx* g_WinTex = NULL;
CEnemy* g_SelectEnemy = NULL;  //ָ�����ڹ����ĵ���ָ��,�������Զ��ͷ�
CRainSystem * g_SlowWeather = NULL; //ָ������ϵͳ�е���ѩָ�룬���Լ���ʼ�ͷţ��ɱ𴦴���

CFightLabel* g_FightLabel = NULL;  //ս����������

CDexTex*       g_FileTex = NULL;
CBillBoardAni* g_FireBill = NULL;

std::vector<CSkill*> g_Skills;

CButtonGui* btn = NULL;


CTextureEx*  g_Ex = NULL;


LPDIRECT3DVERTEXBUFFER9 g_vertexBuff = NULL;    //��������ͶӰ�Ķ��㻺��
CSkillAddBlood* g_2DSkill = NULL;   //���Է�����Ⱦ��2Dƽ����
stVertex1* vertexs = NULL;


CDexWidgetContainer widget_container;
CTestPanel* g_testPanel = NULL;
DInt64 g_lastTickTime = 0;
int  g_delta = 0;

CDexParticalEmit* particalMgr = NULL;
CDexEffectEmitInstance* g_pEmittInstance = NULL;
CDexSceneObject* g_sceneNode = NULL;

CDexQuad* g_pQuadTree = NULL;

string g_imeString;

D3DMATERIAL9 g_material;
D3DMATERIAL9 g_material2;
D3DMATERIAL9 g_material3;
LPD3DXMESH g_sphere = NULL;
LPD3DXMESH g_lightShere = NULL;
LPD3DXMESH g_cube = NULL;
LPD3DXMESH g_teapot = NULL;
D3DLIGHT9  g_light;
D3DXVECTOR3 g_lightPos = D3DXVECTOR3(0,0,0);
LPD3DXEFFECT g_pEffect = NULL;

HWND m_hWndPropDlg;  //����dialog ����
LRESULT CALLBACK PropDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_INITDIALOG: 
		{
			int i = 1;
			break;
		}
	case WM_COMMAND:
		{
			int t = 1;
			break;
		}
	}
	return 0;
}
bool  InitializeOnce(HWND hWnd, bool fullscreen); 
bool  InitializeD3D(HWND hWnd, bool fullscreen);
void Shutdown();


void  GameEnd();

void  GameCycle();


void  MouseMove(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLDown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLUp(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseRUp(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  KeyUp();
void  KeyDown();

void  MouseMoveIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseMoveInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseMoveTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseMoveOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseMoveFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);

void  MouseLDownIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLDownInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLDownTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLDownOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLDowhFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);

void  MouseLUpIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLUpInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLUpTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLUpOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseLUpFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);

void  MouseRUpInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseRUpTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseRUpOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
void  MouseRUpFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);

void  KeyUpTown();
void  KeyUpOutdoors();
void  KeyUpFight();

void  KeyDownTown();
void  KeyDownOutdoors();
void  KeyDownFight();

void  UpdateInput(); 
void  GameRender();
int g_iLoadRate = 0;	 //������Դ�Ľ���
string g_strLoadDes;     //��ǰ���ڼ��ص�˵��
void  RenderLoading();
void  RenderIntroduce();
void  RenderInterFace();
void  RenderTown();
void  RenderOutdoors();
void  RenderSpeak();
void  RenderFight();


//introduce���ú�interfaceһ��gui��ÿ��״̬��Ū��gui,����TM����~״̬�л�֮���Ӧ�ͷ���Դ����
bool  ApplyIntroduce();
bool  ApplyInterface();
bool  ApplyTown();
bool  ApplyOutdoors();
bool  ApplyLoading();
bool  ApplySpeak();
bool  ApplyFight();

void  ReleaseIntroduce();
void  ReleaseInterface();
void  ReleaseTown();
void  ReleaseOutdoors();
void  ReleaseLoading();
void  ReleaseSpeak();
void  ReleaseFight();

void IniMesh();
void HandleMesh();
void RenderLightMesh();
void  InterfaceUiOkButton(DexObject* object, stEvent event)
{
	_Message("ȷ����ť���ܵ���Ϣ�����ڴ���");
}

//����alpha���Ե��������z���򣬲���ָ������
CModelAlpha *g_tempAlpha = NULL;   //ָ�����򽻻����м����(����Ҫ�����ڴ�ռ�Ҳ����Ҫ�ͷ��ڴ�ռ�)
void  SortAlphaModel(); 

bool HandleCommandArgu(char* argus)
{
	return true;
}
