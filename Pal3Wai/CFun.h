

/*
��Ϸ���к���
*/
#pragma  once

#include "../Source/CEngine_h.h"
#include "../Geometry/CCollideShpere.h"
#include "../Geometry/CCollideBox.h"
#include "CRoad.h"
#include "CLadder.h"
#include "CNpc.h"
#include "CEnemy.h"
#include "PalDefine.h"



extern int64 g_currentTime;
extern int64 g_lastTime;
extern int   g_fps;
extern int   g_counter;
extern int   g_clickId;
extern size_t g_clickVIndex;
extern bool  g_isTownNpc;

extern LPDIRECT3DDEVICE9 g_D3DDevice;
extern lua_State *L;
extern CCamera  g_camera;
extern CTerrain*     g_terrain ;	    //����
extern std::vector<TalkStruct*> g_talk;
extern CSpeak*	g_speakFrame;
extern int g_task; 
extern bool g_OpenFog; 
extern CSkyBox  *g_pSky;  //��պ�
extern CRainSystem * g_SlowWeather; 

extern std::vector<CModel*>  g_modelVector;   //���������е�ģ������
extern std::vector<CModelAlpha*>  g_alphaModelVector; 
extern std::vector<CModelXAni*>  g_aniModelVector;	
extern std::vector<CEnemy*> g_enemyVector;    //ս��ϵͳ�еĵ���
extern std::vector<CNpc*>  g_npcVector;   
extern std::vector<CNpc*>  g_npcVector2;   
extern std::vector<CCollideSphere*>  g_collideSphere;
extern std::vector<CCollideBox*>  g_collideBox; 
extern std::vector<CSoundObject*> g_sndObject; 
extern std::vector<CParticalSystem*> g_partical; 

bool DoLuaFile(char* file);

int  SetCameraPos(lua_State *L);
int  SetCameraFox(lua_State* L);
int  SetCameraDest(lua_State *L);
int  EnableCameraMove(lua_State *L);
int  SetCameraVel(lua_State *L);

int  LuaMessageBox(lua_State* L);
int  LuaAddTalk(lua_State* L);

int  CreateObject(lua_State* L);   //��ӷ��ݡ���ľ��û���������Եľ�̬����
int  CreateAlphaObject(lua_State* L);   //��Ӵ���alphaͨ��������
int  CreateAniObject(lua_State* L);	  //��Ӿ��ж��������壬������NPC���紫�͵�
int  CreateSphere(lua_State* L);   //�����ײ��
int  CreateBox(lua_State* L);   //�����ײ������
int  CreateRoad(lua_State* L);   //��ӵ��桢��·
int  CreateLadder(lua_State* L);  //�������

int  CreateNpc(lua_State* L);      //���NPC�ȴ��ж�̬���������¼����Ե�����
int  AddNpcNode(lua_State* L);	   //���NPC����·����(�Ը���ӽ���NPC��Ч)

int  AddEnemy(lua_State* L);//ս��״̬��ӵ���

int  CreateWeather(lua_State* L);  //�����/ѩ   
int  CreateFire(lua_State* L);//��ӻ���
int  CreateTerrain(lua_State* L);

int  OpenFog(lua_State* L); //����Ч
int  CloseFog(lua_State* L);//�ر���Ч

int  PlayMusic(lua_State* L);

int  GetClickId(lua_State* L);     //ȡ�õ�������ID
int  ReSetClick(lua_State* L);	   //��ѡ����
int  GetTask(lua_State* L);
int  SetTask(lua_State* L);

int  SetSkyPra(lua_State* L);  //������պв���
int  SetSkyFace(lua_State* L);	 //������պ���

void LuaRegisterFun(lua_State *L);
void GetFps();