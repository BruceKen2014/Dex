/*************************************************************
****DemoEx引擎主引擎类CGameEngine
****作者：阚正杰(BruceKen)
****日期：2012-11-03
*************************************************************/
#ifndef _CGAMEENGINE_H
#define _CGAMEENGINE_H


#include "typedefine.h"
#include "../Source/CModelX.h"
#include "../Source/CLight.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow);
//
//LRESULT CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  //msg为消息ID

bool  InitializeD3D();
void  GameEnd();

void  GameCycle();

void  GetInput(); 
  void  MouseMove(int x, int y);
  void  MouseLDown(int x, int y);
  void  MouseLUp(int x, int y);

void  GameRender();
    void InterfaceRender();
	void LevelRender();
class CGameEngine
{
protected:
	LPDIRECT3D9       m_direct3D;
	LPDIRECT3DDEVICE9 m_device;

	HWND              m_window;
	int32             m_width;
	int32             m_height;
	int               m_fps;       

	D3DXMATRIX        m_projection;
	DexColor          m_clearColor;	 //背景清除色

	bool              m_sleep;	
	bool              m_canRender;
	bool              m_fullscreen;
	bool              m_openFog;     //是否开启雾效
	_stFog            m_fog;	     //雾效参数

	std::vector<CLight *>  m_Lights;     
	std::vector<CModelX *>  m_XModels; 

protected:

	//fps参数
	int               m_counter;
	int64             m_currentTime;
	int64             m_lastTime;
	


public:
	CGameEngine();
	virtual ~CGameEngine();
	virtual bool Check();
	
protected:
	D3DMULTISAMPLE_TYPE GetSampleType(DEMO_SAMPLE_TYPE _type, D3DDEVTYPE type, D3DFORMAT format, bool fullscreen);      //自定义类型到D3D类型转换

public:
	virtual bool Initialize(int32 width, int32 height, HWND hWnd, bool fullscreen, DEMO_SAMPLE_TYPE _type);
	virtual void SetSamState(DEMOEX_SAM_STATE sam_state, int flag = 0);

	virtual LPDIRECT3DDEVICE9 GetDevice();
	virtual int  GetFps();

	virtual void SetClearColor(DexColor color);

	virtual void ShutDown();

	virtual bool GetSleep();
	virtual void SetSleep(bool sleep);    

	virtual void StartRender(bool color = true, bool zbuffer = true, bool stencil = false);
	virtual void EndRender();

	virtual	void SetFog(DEMOEX_FOG_TYPE fog_type, DEMOEX_FOG_MODE fog_mode, float start,
		                float end, float density, const DexColor& color, bool range) ;	 //设置雾效参数
	virtual void SetFog(stFog _fog) ;	 //设置雾效参数
	virtual void EnableFog();			 //开启雾效
	virtual void DisableFog() ;			 //关闭雾效
	virtual bool GetOpenFog();			 //获得当前是否开启雾效

	virtual void AddLight(CLight * light);
	virtual void ClearLights() {};
	virtual void EnableLight();            //开启光照效果
	virtual void DisableLight()  ;         //关闭光照效果
	virtual void EnableLightId(int32 lightid) ;      //激活灯光，根据灯光自身ID进行设置
	virtual void EnableLightIndex(int32 index);      //激活灯光，根据灯光在D3D中的索引设置
	virtual void DisableLightId(int32 lightid);	     //关闭指定灯
	virtual void DisableLightIndex(int32 index);	 //关闭指定灯

	virtual void AddXmodel(CModelX * model);
	virtual void ClearXModels() {};
	virtual void RenderXModel();

	virtual void SaveScreenShot(char* filename, DEXIMAGE_FORMAT format){};	  //以指定格式保存截图

};


#endif