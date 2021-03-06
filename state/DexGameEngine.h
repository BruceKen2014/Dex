
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
#include "../DexBase/DexDMap.h"
class DexGameState;
class DexGlobal;
class CDexTex;
class DexModelBase;
class DexSkinMesh;
class IDexModelLoader;
class IDexModelAniFileLoader;
class IDexVertexDecl;
class IDexRender;
class IDexDevice;
class DexTextureManager;
class DexWidgetEditBox;
class DexPanelCommand;
class CDexScriptMgr;
class DexGameEngine
{
public:
	typedef struct _stDexModuleInfo
	{
		DEXWORD iModuleAddress;  //模块入口地址
		DEXWORD iStartFunctionAddress; //入口函数地址
		DEXWORD iModuleSize;     //模块大小
	}stDexModuleInfo;
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
	IDexDevice* GetDDevice();
	HWND GetHwnd();
	void SetHInstance(HINSTANCE instance);
	HINSTANCE GetHInstance();
    stDexModuleInfo* GetModuleInfo();
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
	
	bool Initialize(int flag=0);
	
	int  getFps()		{ return g_iFps;}

	//texture manager
	DexTextureManager* GetTextureManager();

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
	DBool    FindCommandArgus(const DChar* str);  //运行命令行是否有指定参数
	DString  GetCommandString();				  //返回运行命令行
	void Run();
	void Update();
	void Render();
// 	void SetRenderState(Dex_RenderState state, DWORD value);

	//window
	DInt16 GetWindowWidth();
	DInt16 GetWindowHeight();
	//command
	void InitCommand();
	void ExecCommand(DString command);
	//render mode
	void SetRenderMode(DexRenderMode mode);
	DexRenderMode	GetRenderMode();
	//
	void OnDragFiles(const DVector<DString>& FileNames);
	void SetClearColor(DFloat32 r, DFloat32 g, DFloat32 b);
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
	void SetLightIdEnable(DInt32 lightId, bool enable);
	void AddLight(const DexLight& light);
	//const float* GetPointLightData()const;
	DexLight* GetLight(DInt32 lightId);
	bool GetLightEnable();
	void CalculateLightData();//如果使用DexLight* GetLight(DInt32 lightId);改变灯光数据，需要手动调用此函数更新灯光数据

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
	void SetTexture(DInt32 stage, CDexTex* texture);
	//for固定管线，需要设置FVF
	void DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 fvf, DInt32 stridesize);
	//for shader，不需要设置FVF
	void DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 stridesize);
	//本接口可以加载各种引擎支持的模型文件，包括引擎本身模型文件.dexmodel,加载.dexmodel以二进制数据读入，速度会很快，建议把其他模型文件用引擎工具MeshConvert转换为.dexmodel,然后加载转换后的二进制模型文件
	DexModelBase* CreateModel(const char* filename, int flag=0);
	bool SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int flag=0);
	bool ReadModelAnimation(DexSkinMesh* pDexSkinMesh, const char* filename);
	bool ReadFFSkeletonInfo(DexSkinMesh* pDexSkinMesh, DString filename);
	
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
private:
		DexGameEngine();
		~DexGameEngine();
private:
	IDexDevice* CreateDevice(DString deviceType);
	bool InitModuleInfo();
	void UpdateFps();
	void KeyUp();
private:
	static DexGameEngine*    g_pGameEngine;
private:
	//sizeof(string)=28   sizeof(vector)= 16  sizeof(map)=12
	DString         g_nextStateName;  
	DString         g_strWindowName;
	DString         g_strWindowClass;
	DString			g_strCommandLine;    //启动游戏的命令行
	DVector<IDexModelLoader*> vecModelLoader;   //模型加载器
	DVector<IDexModelAniFileLoader*> vecModelAniLoader;//模型动作文件加载器
	DVector<DexLight> g_vecLight;
	DMap<DString, DexGameState*> g_States;  //引擎管理多个游戏状态
	stDexModuleInfo			 g_stModuleInfo;


	LPDIRECT3D9				 g_D3D;				//D3D
	LPDIRECT3DDEVICE9		 g_D3DDevice;         //设备
	LPD3DXMESH				 g_cube;   //辅助立方体 将这个立方体渲染出来 方便查看Node位置
	LPD3DXMESH				 g_sphere;
	IDexDevice*				 g_pDevice;
	CCamera*				 g_camera;
	DexGameState*			 g_pCurrState;
	DexGameState*			 g_pLoadingState;
	/*g_pGlobalVariable，只用于保存全局变量，引擎并不实现，这里只提供一个变量
	供其他state通过engine调用， 具体用g_pGlobalVariable的什么，引擎并不知道，内容由主程序实现
	*/
	DexGlobal*				 g_pGlobalVariable;
	DexTextureManager*		 g_TextureManager;
	IDexRender*				 m_pRender;
	DexPanelCommand*		 g_pCommandPanel; //for input command 
	CDexScriptMgr*			 g_pScriptMgr;
	WNDPROC					 g_msgPro; //外界传入为了初始化窗口调用
	lua_State*				 L;
	HWND					 g_hWnd;
	HINSTANCE				 g_hInstance;
	
	D3DMATERIAL9			 g_material;  //用于给Mesh上材质 主要用于改变颜色
	DexMem					 g_mem;  //一个处理序列化的公共变量

	DInt64					 g_iCurrentTime;
	DInt64					 g_iLastTime;
	//light data
	DexColor				 g_ambientColor;
	DexColor				 g_clearColor;
	HANDLE					 g_LoadThread;    //加載資源時 顯示加載界面的線程
	HANDLE					 g_LogThread;
	DWORD					 g_LoadThreadId;
	DWORD					 g_LogThreadId;
	DInt32               g_iIconRes;   //icon res
	DInt16               g_iWindowPosX;
	DInt16               g_iWindowPosY;
	DInt16               g_iWindowWidth;
	DInt16               g_iWinddowHeight;
	DInt16               g_iFps;
	DInt16				 g_iCounter;
	DInt16               g_iMouseX;
	DInt16               g_iMouseY;
	DInt16               g_iMouseXLastFrame;
	DInt16               g_iMouseYLastFrame;
	DBool			   g_bFullscreen : 1;
	DBool              m_bRenderCollideMesh : 1;
	DBool              m_bRenderNodeCube : 1;
	DBool              m_bRenderActionRouteLine : 1;    //object action 軌跡
	DBool              m_bRenderPieceEffectRoute : 1;   //麵片軌跡 
	DBool              m_bLoading : 1;
	DBool              m_bLightEffect : 1; //是否开启灯光效果
	DBool              m_bLightEnable : 1; //开启灯光效果后，灯光是否开启
// 	float			 g_pointLightData[100];
// 	float			 g_DirLightData[100];
// 	int				 g_lightDataSize;
};
//该define 必须由主程序调用
#define DEXENGINE_INITGLOBAL(clas)  \
     (clas*) gl = new clas;       \
	 gl->Init()			;			\
	 DexGameEngine::getEngine()->SetGlobalVariable(gl) 


#endif