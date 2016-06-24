
#ifndef _DEXGAMEENGINE_H
#define _DEXGAMEENGINE_H

#include "../DexBase/typedefine.h"
#include "../DexBase/DexDVector.h"
#include "../DexBase/DexString.h"
#include "../DexBase/DexMem.h"
#include "../DexBase/DexPrimitive.h"
#include "../DexBase/DexMaterial.h"
#include "../DexBase/DexLight.h"
#include "../Source/CCamera.h"
#include "../widget/DexGuiStruct.h"
#include "../Geometry/CRay.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexMatrix.h"
#include <string>
#include <map>
class DexGameState;
class DexGlobal;
class CDexTex;
class DexModelBase;
class DexSkinMesh;
class IDexModelLoader;
class IDexVertexDecl;
class IDexRender;
// typedef enum
// {
// 	DEXRS_ZENABLE					,   //是否开启Z buff
// 	DEXRS_ZWRITEENABLE              ,   //是否开启深度写入
// 	DEXRS_ALPHATESTENABLE           ,   //是否开启alpha测试
// 	DEXRS_SRCBLEND                  ,   
// 	DEXRS_DESTBLEND                 ,   
// 	DEXRS_CULLMODE                  ,   // CULL mode 
// 	DEXRS_ALPHABLENDENABLE          ,   //是否开启alpha混合
// 	//DEXRS_FOGENABLE                 ,   //是否开启雾效
// 	//DEXRS_FOGCOLOR                  ,   // fog color 
// 	//DEXRS_FOGTABLEMODE              ,   // fog mode
// 	//DEXRS_FOGSTART                  ,   // fog start (for both vertex and pixel fog) 
// 	//DEXRS_FOGEND                    ,   // Fog end      
// 	//DEXRS_FOGDENSITY                ,   // Fog density  
// 	//DEXRS_RANGEFOGENABLE            ,   // Enables range-based fog
// 	//DEXRS_FOGVERTEXMODE             ,   // fog mode
// 	
// 	DEXRS_STENCILENABLE             ,   // BOOL enable/disable stenciling 
// 	DEXRS_LIGHTING                  ,   // light
// 	DEXRS_AMBIENT                   ,   //ambient color
// 	DEXRS_POINTSIZE                 ,   /* float point size */
// 	DEXRS_POINTSIZE_MIN             ,   /* float point size min threshold */
// 	DEXRS_POINTSPRITEENABLE         ,   /* BOOL point texture coord control */
// 	DEXRS_POINTSCALEENABLE          ,   /* BOOL point size scale enable */
// 	DEXRS_POINTSCALE_A              ,   /* float point attenuation A value */
// 	DEXRS_POINTSCALE_B              ,   /* float point attenuation B value */
// 	DEXRS_POINTSCALE_C              ,   /* float point attenuation C value */
// 	DEXRS_POINTSIZE_MAX             ,   /* float point size max threshold */
// 	DEXRS_INDEXEDVERTEXBLENDENABLE  ,   //顶点颜色混合
// 	DEXRS_BLENDOP                   ,   //BLENDOP setting
// }Dex_RenderState;
// 
// typedef enum _DEXBLEND {
// 	DEXBLEND_ZERO               = 1,
// 	DEXBLEND_ONE                = 2,
// 	DEXBLEND_SRCCOLOR           = 3,
// 	DEXBLEND_INVSRCCOLOR        = 4,
// 	DEXBLEND_SRCALPHA           = 5,
// 	DEXBLEND_INVSRCALPHA        = 6,
// 	DEXBLEND_DESTALPHA          = 7,
// 	DEXBLEND_INVDESTALPHA       = 8,
// 	DEXBLEND_DESTCOLOR          = 9,
// 	DEXBLEND_INVDESTCOLOR       = 10,
// 	DEXBLEND_SRCALPHASAT        = 11,
// 	DEXBLEND_BOTHSRCALPHA       = 12,
// 	DEXBLEND_BOTHINVSRCALPHA    = 13,
// 	DEXBLEND_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
// } DEXBLEND;
class DexGameEngine
{
private:
	static DexGameEngine*    g_pGameEngine;

	HWND			  g_hWnd;
	HINSTANCE		  g_hInstance;
	WNDPROC  g_msgPro; //外界传入为了初始化窗口调用
	int               g_iIconRes;
	std::string       g_strWindowName;
	std::string       g_strWindowClass;
	LPDIRECT3D9       g_D3D ;				//D3D
	LPDIRECT3DDEVICE9 g_D3DDevice ;         //设备
	
	lua_State*		  L;
	bool			  g_bFullscreen;
	int               g_iWindowPosX;
	int               g_iWindowPosY;
	int               g_iWindowWidth;
	int               g_iWinddowHeight;

	HANDLE       g_LoadThread;    //加載資源時 顯示加載界面的線程
	DWORD        g_LoadThreadId;  //

	//以上变量游戏运行后就不必关心了

	int               g_iFps;  
	int				  g_iCounter;
	int64			  g_iCurrentTime;
	int64		      g_iLastTime;

	CCamera*		  g_camera;
	DexMem            g_mem;  //一个处理序列化的公共变量
	int               g_iMouseX;
	int               g_iMouseY;
	int               g_iMouseXLastFrame;
	int               g_iMouseYLastFrame;
	bool              m_bRenderCollideMesh;
	bool              m_bRenderNodeCube;
	bool              m_bRenderActionRouteLine;    //object action 軌跡
	bool              m_bRenderPieceEffectRoute;   //麵片軌跡 
	bool              m_bLoading;
	bool              m_bLightEffect; //是否开启灯光效果
	bool              m_bLightEnable; //开启灯光效果后，灯光是否开启

	std::map<string, DexGameState*> g_States;  //引擎管理多个游戏状态

	DexGameState*     g_pCurrState;
	DexGameState*     g_pLoadingState;
	/*g_pGlobalVariable，只用于保存全局变量，引擎并不实现，这里只提供一个变量
	  供其他state通过engine调用， 具体用g_pGlobalVariable的什么，引擎并不知道，内容由主程序实现
	*/
	DexGlobal*     g_pGlobalVariable;  
	D3DMATERIAL9 g_material;  //用于给Mesh上材质 主要用于改变颜色
	LPD3DXMESH  g_cube;   //辅助立方体 将这个立方体渲染出来 方便查看Node位置
	LPD3DXMESH  g_sphere;
	string         g_nextStateName;
	IDexModelLoader* ms3dLoader;
	IDexModelLoader* objLoader;
	IDexModelLoader* daeLoader;

	IDexRender*      m_pRender;
	//light data
	DVector<DexLight> g_vecLight;
	DexColor		 g_ambientColor;
	float			 g_pointLightData[100];
	float			 g_DirLightData[100];
	int				 g_lightDataSize;
private:
	DexGameEngine();
	~DexGameEngine();
	
	void UpdateFps();
public:
	D3DXMATRIX g_projection;   
	D3DXMATRIX g_ViewMatrix;
	D3DXMATRIX g_worldMatrix;  //世界坐标矩阵

	D3DXMATRIX g_scaleMatrix;
	D3DXMATRIX g_transMatrix;
	D3DXMATRIX g_rotateMatrix;

	LPDIRECT3DTEXTURE9 m_pBufferFontTexture; //中间texture size 1024x1024用于将文字渲染上去
											 //只渲染文字，所以不需要切换视图矩阵等参数
	LPDIRECT3DSURFACE9 m_pBufferFontTextureSurface;
	LPDIRECT3DSURFACE9 m_pBackSurface;  //最终渲染的Surface
	DexRenderMode  m_RenderMode;
public:

public:
	static DexGameEngine* getEngine()
	{
		if(g_pGameEngine == NULL)
			g_pGameEngine = new DexGameEngine();
		return g_pGameEngine;
	}
	static void ShutDownEngine()
	{
		if(g_pGameEngine != NULL)
		{
			delete g_pGameEngine;
			g_pGameEngine = NULL;
		}
	}
public:
	HWND GetHwnd()		{ return g_hWnd;}
	void SetHInstance(HINSTANCE instance)	  { g_hInstance = instance;}
	HINSTANCE GetHInstance()				  { return g_hInstance;}
	void SetWindowSize(int width, int height) { g_iWindowWidth = width; g_iWinddowHeight = height;}
	void SetWIndowPos(int posX, int posY)     { g_iWindowPosX = posX; g_iWindowPosY = posY;}
	void SetFullScreen(bool fullscreen)       { g_bFullscreen = fullscreen;}	  
	void SetIcon(int icon)  				  { g_iIconRes = icon;}
	void SetMsgPro(WNDPROC msgpro)		      { g_msgPro= msgpro;}
	void SetLoading(string state_name)		;
	bool GetLoading();
	lua_State* GetLuaState()	{ return L;};
	DexGameState* GetLoadingState();
	DexGameState* GetCurrState();
	void SetLoadingState(DexGameState* pLoadingState);
	
	bool Initialize();
	
	int  getFps()		{ return g_iFps;}

	//game state
	bool AddState(DexGameState* state);
	DexGameState* getGameState(string type);
	void SetCurrState(string state_name);

	//这两个函数只是权宜之计。等到完全将引擎机制代替switch作为游戏机制就可以去掉了
	void SetDevice(LPDIRECT3DDEVICE9 device) { g_D3DDevice = device;	D3DXCreateBox(g_D3DDevice, 1, 1, 1, &g_cube, NULL);
	D3DXCreateSphere(g_D3DDevice, 1, 10, 10, &g_sphere, NULL);}
	void SetHwnd(HWND hwnd) { g_hWnd = hwnd;}

	LPDIRECT3DDEVICE9 GetDevice()             { return g_D3DDevice;}
	void SetGlobalVariable(DexGlobal* global) { g_pGlobalVariable = global;}
	DexGlobal* GetGlobalVariable()            { return g_pGlobalVariable;}
	IDexRender* GetRender()					  { return m_pRender; };
	void Run();
	void Update();
	void Render();
// 	void SetRenderState(Dex_RenderState state, DWORD value);

	//render mode
	void SetRenderMode(DexRenderMode mode);
	DexRenderMode	GetRenderMode();
	//fog
	void DisableFog();
	bool OpenFog();
	void EnableFog();
	void SetFogColor(const DexColor& color);
	void SetFogMode(DEMOEX_FOG_MODE mode);
	void SetFogStart(float start);
	void SetFogEnd(float end);
	void SetFogDensity(float density);
	void DisableRangeFog();
	void EnableRangeFog();

	//灯光：这里负责赋参数，具体灯光管理应由各个场景管理
	void SetWorldAmbient(const DexColor& color); //世界环境光，默认是黑色
	void SetLightEffect(bool enable); //设置是否使用灯光效果
	void SetLightEnable(bool enable);//控制全部灯光是否有效
	void SetLightIdEnable(int32 lightId, bool enable);
	void AddLight(const DexLight& light);
	//const float* GetPointLightData()const;
	DexLight* GetLight(int32 lightId);
	bool GetLightEnable();
	void CalculateLightData();//如果使用DexLight* GetLight(int32 lightId);改变灯光数据，需要手动调用此函数更新灯光数据

	//render line cube sphere
	void RenderCoorLines();
	void SetMeshColor(const DexColor& color);
	void RenderCube(const DexVector3& pos, const DexVector3& scale = DexVector3(1.0f, 1.0f, 1.0f));
	void RenderCube(const D3DXMATRIX& matrix);
	void RenderCube(const DexMatrix4x4& matrix);
	void RenderSphere(const DexVector3& pos, float scale = 1.0f);
	void Render3DLine(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff); 
	void Render3DLine(const DexVector3& p0, const DexVector3& p1, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	//从点p出发，朝vec方向绘制直线 color1:颜色 length:线段长度 color2末断点颜色
	void Render3DLine(const D3DXVECTOR3& p, const D3DXVECTOR3& vec, const DexColor& color1 = 0xffffffff, float length = 100, const DexColor& color2 = 0xffffffff);
	void Render3DLine(const DexVector3& p, const DexVector3& vec, const DexColor& color1 = 0xffffffff, float length = 100, const DexColor& color2 = 0xffffffff);

	void SetMaterial(const DexMaterial& material);
	void SetTexture(int32 stage, CDexTex* texture);
	//for固定管线，需要设置FVF
	void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 fvf, int32 stridesize);
	//for shader，不需要设置FVF
	void DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize);
	DexModelBase* CreateModel(const char* filename, int flag=0);
	bool ReadActInfoFxii(DexSkinMesh* pDexSkinMesh, const char* filename);
	bool ReadFFSkeletonInfo(DexSkinMesh* pDexSkinMesh, DString filename);
	
	void CreateFFMap(DVector<DexSkinMesh*>& vecSkinMesh, const char* filename);
	void LookAtLH(const DexVector3 *pEye, const DexVector3 *pAt, const DexVector3 *pUp);
	
	//輔助調試函數
	void setRendeCollideMesh(bool b);
	bool getRendeCollideMesh();
	void setRenderNodeCube(bool b);
	bool getRenderNodeCube();
	void setRenderActionRouteLine(bool b);
	bool getRenderActionRouteLine();
	void setRenderPieceEffectRoute(bool b);
	bool getRenderPieceEffectRoute();
public:
	void updateViewMatrix();
	void setDexVertexDecl(IDexVertexDecl* decl);
public:	
	//lua
	bool DoLuaFile(const char* filename);
	bool DoLuaString(const char* str);
	//mem
	DexMem&  getMem();
	
	//camera
	CCamera* getCamera();
	void SetProjectArgus(float fov, float aspect, float near_distance, float far_distance);
	void MoveCamera(DexVector3 vec, float amount, bool move_focus = false);
	void MoveCamera(CCamera::MOVE_DIRECTION direction, float amount, bool move_focus = false);
	void SetCameraPos(const DexVector3& pos);
	void SetCameraLookAt(const DexVector3& lookAt);
	void RotateCameraByFocus(float value);

	//ray
	void GetRay(int x, int y, stRay& ray);//x, y 屏幕上的點，得到射線
	DexGUI::DexPoint GetXY(const D3DXVECTOR3& world_point);//傳入世界坐標 返回屏幕對應的屏幕坐標
};
//该define 必须由主程序调用
#define DEXENGINE_INITGLOBAL(clas)  \
     (clas*) gl = new clas;       \
	 gl->Init()			;			\
	 DexGameEngine::getEngine()->SetGlobalVariable(gl) 


#endif