
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
class IDexModelAniFileLoader;
class IDexVertexDecl;
class IDexRender;
class IDexDevice;
class DexTextureManager;
class DexGameEngine
{
public:
	typedef struct _stDexModuleInfo
	{
		DEXWORD iModuleAddress;  //ģ����ڵ�ַ
		DEXWORD iStartFunctionAddress; //��ں�����ַ
		DEXWORD iModuleSize;     //ģ���С
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

	//����������ֻ��Ȩ��֮�ơ��ȵ���ȫ��������ƴ���switch��Ϊ��Ϸ���ƾͿ���ȥ����
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
	//
	void OnDragFiles(const DVector<DString>& FileNames);
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

	//�ƹ⣺���︺�𸳲���������ƹ����Ӧ�ɸ�����������
	void SetWorldAmbient(const DexColor& color); //���绷���⣬Ĭ���Ǻ�ɫ
	void SetLightEffect(bool enable); //�����Ƿ�ʹ�õƹ�Ч��
	void SetLightEnable(bool enable);//����ȫ���ƹ��Ƿ���Ч
	void SetLightIdEnable(DInt32 lightId, bool enable);
	void AddLight(const DexLight& light);
	//const float* GetPointLightData()const;
	DexLight* GetLight(DInt32 lightId);
	bool GetLightEnable();
	void CalculateLightData();//���ʹ��DexLight* GetLight(DInt32 lightId);�ı�ƹ����ݣ���Ҫ�ֶ����ô˺������µƹ�����

	//render line cube sphere
	void RenderCoorLines();
	void SetMeshColor(const DexColor& color);
	void RenderCube(const DexVector3& pos, const DexVector3& scale = DexVector3(1.0f, 1.0f, 1.0f));
	void RenderCube(const D3DXMATRIX& matrix);
	void RenderCube(const DexMatrix4x4& matrix);
	void RenderSphere(const DexVector3& pos, float scale = 1.0f);
	void Render3DLine(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff); 
	void Render3DLine(const DexVector3& p0, const DexVector3& p1, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	//�ӵ�p��������vec�������ֱ�� color1:��ɫ length:�߶γ��� color2ĩ�ϵ���ɫ
	void Render3DLine(const D3DXVECTOR3& p, const D3DXVECTOR3& vec, const DexColor& color1 = 0xffffffff, float length = 100, const DexColor& color2 = 0xffffffff);
	void Render3DLine(const DexVector3& p, const DexVector3& vec, const DexColor& color1 = 0xffffffff, float length = 100, const DexColor& color2 = 0xffffffff);

	void SetMaterial(const DexMaterial& material);
	void SetTexture(DInt32 stage, CDexTex* texture);
	//for�̶����ߣ���Ҫ����FVF
	void DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 fvf, DInt32 stridesize);
	//for shader������Ҫ����FVF
	void DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 stridesize);
	//���ӿڿ��Լ��ظ�������֧�ֵ�ģ���ļ����������汾��ģ���ļ�.dexmodel,����.dexmodel�Զ��������ݶ��룬�ٶȻ�ܿ죬���������ģ���ļ������湤��MeshConvertת��Ϊ.dexmodel,Ȼ�����ת����Ķ�����ģ���ļ�
	DexModelBase* CreateModel(const char* filename, int flag=0);
	bool SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int flag=0);
	bool ReadModelAnimation(DexSkinMesh* pDexSkinMesh, const char* filename);
	bool ReadFFSkeletonInfo(DexSkinMesh* pDexSkinMesh, DString filename);
	
	void LookAtLH(const DexVector3 *pEye, const DexVector3 *pAt, const DexVector3 *pUp);
	
	//�o���{ԇ����
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
	void GetRay(int x, int y, stRay& ray);//x, y ��Ļ�ϵ��c���õ��侀
	DexGUI::DexPoint GetXY(const D3DXVECTOR3& world_point);//������������ ������Ļ��������Ļ����

public:
	D3DXMATRIX g_projection;
	D3DXMATRIX g_ViewMatrix;
	D3DXMATRIX g_worldMatrix;  //�����������

	D3DXMATRIX g_scaleMatrix;
	D3DXMATRIX g_transMatrix;
	D3DXMATRIX g_rotateMatrix;
	LPDIRECT3DTEXTURE9 m_pBufferFontTexture; //�м�texture size 1024x1024���ڽ�������Ⱦ��ȥ
	//ֻ��Ⱦ���֣����Բ���Ҫ�л���ͼ����Ȳ���
	LPDIRECT3DSURFACE9 m_pBufferFontTextureSurface;
	LPDIRECT3DSURFACE9 m_pBackSurface;  //������Ⱦ��Surface
	DexRenderMode  m_RenderMode;
private:
		DexGameEngine();
		~DexGameEngine();
private:
	IDexDevice* CreateDevice(DString deviceType);
	bool InitModuleInfo();
	void UpdateFps();
private:
	static DexGameEngine*    g_pGameEngine;
	IDexDevice*				 g_pDevice;
	HWND			  g_hWnd;
	HINSTANCE		  g_hInstance;
	stDexModuleInfo   g_stModuleInfo;
	WNDPROC  g_msgPro; //��紫��Ϊ�˳�ʼ�����ڵ���
	int               g_iIconRes;
	std::string       g_strWindowName;
	std::string       g_strWindowClass;
	LPDIRECT3D9       g_D3D;				//D3D
	LPDIRECT3DDEVICE9 g_D3DDevice;         //�豸

	lua_State*		  L;
	bool			  g_bFullscreen;
	int               g_iWindowPosX;
	int               g_iWindowPosY;
	int               g_iWindowWidth;
	int               g_iWinddowHeight;

	HANDLE       g_LoadThread;    //���d�YԴ�r �@ʾ���d����ľ���
	DWORD        g_LoadThreadId;

	HANDLE       g_LogThread;
	DWORD        g_LogThreadId;

	//���ϱ�����Ϸ���к�Ͳ��ع�����

	int               g_iFps;
	int				  g_iCounter;
	DInt64			  g_iCurrentTime;
	DInt64		      g_iLastTime;

	CCamera*		  g_camera;
	DexMem            g_mem;  //һ���������л��Ĺ�������
	int               g_iMouseX;
	int               g_iMouseY;
	int               g_iMouseXLastFrame;
	int               g_iMouseYLastFrame;
	bool              m_bRenderCollideMesh;
	bool              m_bRenderNodeCube;
	bool              m_bRenderActionRouteLine;    //object action ܉�E
	bool              m_bRenderPieceEffectRoute;   //�IƬ܉�E 
	bool              m_bLoading;
	bool              m_bLightEffect; //�Ƿ����ƹ�Ч��
	bool              m_bLightEnable; //�����ƹ�Ч���󣬵ƹ��Ƿ���

	std::map<string, DexGameState*> g_States;  //�����������Ϸ״̬

	DexGameState*     g_pCurrState;
	DexGameState*     g_pLoadingState;
	/*g_pGlobalVariable��ֻ���ڱ���ȫ�ֱ��������沢��ʵ�֣�����ֻ�ṩһ������
	������stateͨ��engine���ã� ������g_pGlobalVariable��ʲô�����沢��֪����������������ʵ��
	*/
	DexGlobal*     g_pGlobalVariable;
	D3DMATERIAL9 g_material;  //���ڸ�Mesh�ϲ��� ��Ҫ���ڸı���ɫ
	LPD3DXMESH  g_cube;   //���������� �������������Ⱦ���� ����鿴Nodeλ��
	LPD3DXMESH  g_sphere;
	string         g_nextStateName;
	DVector<IDexModelLoader*> vecModelLoader;
	DVector<IDexModelAniFileLoader*> vecModelAniLoader;
	DexTextureManager* g_TextureManager;
	IDexRender*      m_pRender;
	//light data
	DVector<DexLight> g_vecLight;
	DexColor		 g_ambientColor;
	DexColor		 g_clearColor;
	float			 g_pointLightData[100];
	float			 g_DirLightData[100];
	int				 g_lightDataSize;
};
//��define ���������������
#define DEXENGINE_INITGLOBAL(clas)  \
     (clas*) gl = new clas;       \
	 gl->Init()			;			\
	 DexGameEngine::getEngine()->SetGlobalVariable(gl) 


#endif