

/*
游戏特有函数
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
extern CTerrain*     g_terrain ;	    //地形
extern std::vector<TalkStruct*> g_talk;
extern CSpeak*	g_speakFrame;
extern int g_task; 
extern bool g_OpenFog; 
extern CSkyBox  *g_pSky;  //天空盒
extern CRainSystem * g_SlowWeather; 

extern std::vector<CModel*>  g_modelVector;   //城镇中所有的模型向量
extern std::vector<CModelAlpha*>  g_alphaModelVector; 
extern std::vector<CModelXAni*>  g_aniModelVector;	
extern std::vector<CEnemy*> g_enemyVector;    //战斗系统中的敌人
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

int  CreateObject(lua_State* L);   //添加房屋、树木等没有任务属性的静态物体
int  CreateAlphaObject(lua_State* L);   //添加带有alpha通道的物体
int  CreateAniObject(lua_State* L);	  //添加具有动画的物体，但不是NPC，如传送点
int  CreateSphere(lua_State* L);   //添加碰撞球
int  CreateBox(lua_State* L);   //添加碰撞立方体
int  CreateRoad(lua_State* L);   //添加地面、道路
int  CreateLadder(lua_State* L);  //添加梯子

int  CreateNpc(lua_State* L);      //添加NPC等带有动态带有任务事件属性的物体
int  AddNpcNode(lua_State* L);	   //添加NPC行走路径点(对刚添加进的NPC有效)

int  AddEnemy(lua_State* L);//战斗状态添加敌人

int  CreateWeather(lua_State* L);  //添加雨/雪   
int  CreateFire(lua_State* L);//添加火焰
int  CreateTerrain(lua_State* L);

int  OpenFog(lua_State* L); //打开雾效
int  CloseFog(lua_State* L);//关闭雾效

int  PlayMusic(lua_State* L);

int  GetClickId(lua_State* L);     //取得点击物体的ID
int  ReSetClick(lua_State* L);	   //点选重置
int  GetTask(lua_State* L);
int  SetTask(lua_State* L);

int  SetSkyPra(lua_State* L);  //设置天空盒参数
int  SetSkyFace(lua_State* L);	 //设置天空盒面

void LuaRegisterFun(lua_State *L);
void GetFps();