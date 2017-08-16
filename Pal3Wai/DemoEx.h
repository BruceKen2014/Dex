

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


HWND       g_hwnd;						//主窗口句柄
HINSTANCE  g_hInstance;				    //实例

HANDLE       g_LoadThread;    //单独使用一个线程来进行资源的加载 与此同时显示加载界面
DWORD        g_LoadThreadId;  //load thread id
DWORD WINAPI LoadThread1(LPVOID lpParam);	//here load resouce


LPDIRECT3D9 g_D3D = NULL;				//D3D
LPDIRECT3DDEVICE9 g_D3DDevice = NULL;   //设备

D3DXMATRIX g_projection;
D3DXMATRIX g_ViewMatrix;
D3DXMATRIX g_worldMatrix;  //世界坐标矩阵
CCamera g_camera;
lua_State *L = NULL;

PAL_STATE   g_palState; //主状态
PAL2_STATE	g_State2;	//副状态
DexVector3    g_crossPoint = DexVector3(0, 0, 0);
CEnemy  *  g_Jintian = NULL;
CTerrain*     g_terrain = NULL;

CPick*         g_pPick = NULL;
int         g_clickId;  //鼠标点击物体的ID
int  g_iXMouse, g_iYMouse;
size_t      g_clickVIndex;  //鼠标点击的NPC在向量中的索引
int         g_task = 0;	//临时调试用

CGuiSystem   *g_pGuiInterface = NULL; //界面GUI
CGuiSystem   *g_pGuiTown = NULL;	 //城镇GUI()
//CGuiSystem   *g_pGuiOurdoors = NULL;	 //野外GUI
CGuiSystem   *g_pGuiFight = NULL; //战斗GUI

CGuiSystem*   g_pGuiFightInoBack = NULL;  //战斗GUI信息显示，如血条、气条等

CWaterF*      g_water = NULL;
CDexScriptMgr*  g_pScriptMgr = NULL;

bool          g_isTownNpc = false;

HCURSOR       g_Cursors[3];



std::vector<CModel*>  g_modelVector;   //城镇中所有的模型向量
std::vector<CModelAlpha*>  g_alphaModelVector;   //带有alpha属性的模型(树、竹子等)，Z排序将根据这些物体离摄像机的距离进行排序
std::vector<CModelXAni*>  g_aniModelVector;	    //具有动态属性的物体
std::vector<CNpc*>  g_npcVector;	   //npc
std::vector<CCollideSphere*>  g_collideSphere; //替代碰撞球
std::vector<CCollideBox*>  g_collideBox; //替代碰撞立方体
std::vector<CSoundObject*> g_sndObject;  //发声物体(地面、道路、桥、梯子)
std::vector<CParticalSystem*> g_partical;  //粒子系统如雨雪、火焰

std::vector<CNumberAni*>  g_FightNumbers;   //战斗系统中的数字，外部负责添加，内部时间到了之后自动删除


CBloodLabel * g_BloodLabel = NULL;
CTextureMove*   g_LoseTex = NULL;


std::vector<CNpc*>  g_npcVector2;	   //野外敌人   TODO
std::vector<CEnemy*> g_enemyVector;    //战斗系统中的敌人

CSpell * spell = NULL;

CSkyBox       *g_pSky = NULL;

std::map<int, CTextureEx*>  g_headmap ;
bool             g_OpenFog = false;  //是否开启雾效
bool             g_isFightComplete = true;  //施法是否结束，这将决定gui是否显示及是否响应消息

D3DXVECTOR3      g_playPos;   //切换到战斗状态时保存景天位置，战斗结束时切换回来
D3DXVECTOR3      g_playDir;   //保存朝向

D3DXVECTOR3      g_cameraPos; //保存摄像机位置

int g_fps = 0;
int g_counter = 0;
DInt64 g_currentTime = 0.0f;
DInt64 g_lastTime    = 0.0f;

bool  g_isSelectEnemy = false;	 //是否正在选择攻击的敌人
bool  g_isAttacking = false;    //是否正在攻击

int   g_FightStage = FIGHT_FLAG;  //战斗阶段

int   g_FightEndDelay = 0;   //战斗失败后的时间延迟

CTextureEx* g_WinTex = NULL;
CEnemy* g_SelectEnemy = NULL;  //指向正在攻击的敌人指针,由他出自动释放
CRainSystem * g_SlowWeather = NULL; //指向粒子系统中的雨雪指针，不自己初始释放，由别处传入

CFightLabel* g_FightLabel = NULL;  //战斗进度条类

CDexTex*       g_FileTex = NULL;
CBillBoardAni* g_FireBill = NULL;

std::vector<CSkill*> g_Skills;

CButtonGui* btn = NULL;


CTextureEx*  g_Ex = NULL;


LPDIRECT3DVERTEXBUFFER9 g_vertexBuff = NULL;    //测试正交投影的顶点缓冲
CSkillAddBlood* g_2DSkill = NULL;   //测试法术渲染到2D平面上
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

HWND m_hWndPropDlg;  //属性dialog 窗口
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
int g_iLoadRate = 0;	 //加载资源的进度
string g_strLoadDes;     //当前正在加载的说明
void  RenderLoading();
void  RenderIntroduce();
void  RenderInterFace();
void  RenderTown();
void  RenderOutdoors();
void  RenderSpeak();
void  RenderFight();


//introduce采用和interface一套gui，每个状态都弄个gui,我真TM累了~状态切换之间对应释放资源就行
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
	_Message("确定按钮接受到消息，正在处理！");
}

//给有alpha属性的物体进行z排序，采用指针排序
CModelAlpha *g_tempAlpha = NULL;   //指针排序交换的中间变量(不需要申请内存空间也不需要释放内存空间)
void  SortAlphaModel(); 

bool HandleCommandArgu(char* argus)
{
	return true;
}
