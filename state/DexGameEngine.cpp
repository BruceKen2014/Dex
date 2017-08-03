

#include "DexGameEngine.h"
#include "DexGameState.h"
	#ifdef _DEX_PLATFORM_WINDOWS
		#include <Psapi.h>  //for windows init module info
	#endif
// #include "../DexBase/DexCollideObject.h"
// #include "../DexBase/CDexPartical.h"
// #include "../DexBase/DexTerrain.h"
// #include "../DexBase/DexSceneObjectFlower.h"
#include "../DexBase/Dex3DScene.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/CInput.h"
#include "../DexBase/CDexTime.h"
#include "../DexBase/DexObjectFactory.h"
#include "../DexBase/DexVertexDeclDx9.h"
#include "../DexBase/DexRenderDirectx9.h"
#include "../DexBase/DexDeviceDirectx9.h"
#include "../DexMath/Dex_To_Dx.h"

#include "../DexModel/DexModelMs3dLoader.h"
#include "../DexModel/DexModelObjLoader.h"
#include "../DexModel/DexModelDaeLoader.h"
#include "../DexModel/DexModelMwsLoader.h"
#include "../DexModel/DexModelSkinMeshLoader.h"
#include "../DexModel/DexSkinMesh.h"
#include "../Source/CTexture.h"
#include "../DexBase/DexDeviceOpenGLWindows.h"
#include "../DexBase/DexTextureManager.h"
#include "../DexBase/DexStreamFile.h"
#include "../DexBase/DexMd5Generator.h"
#include "../DexModel/DexModelSkinMeshTextLoader.h"
#include "../DexModel/DexModelAniFileFF12ActLoader.h"
#include "../DexModel/DexModelAniFileText.h"

/*目前的DexGameEngine特指windows engine，如果日后要在别的平台上开发，需要将engine抽象为基类
实现windows派生类，然后再实现对应平台的派生类*/
DexGameEngine* DexGameEngine::g_pGameEngine = NULL;
DexGameEngine::DexGameEngine()
{
	g_bFullscreen = false;
	g_iWindowPosX = 100;
	g_iWindowPosY = 100;
	g_iWindowWidth = 800;
	g_iWinddowHeight = 600;
	g_pDevice = DexNull;
	g_D3D = NULL;
	g_D3DDevice = NULL;
	g_cube = NULL;
	L = NULL;
	g_pCurrState = NULL;
	g_pLoadingState = NULL;
	g_pGlobalVariable = NULL;
	m_bRenderCollideMesh = true;
	m_bRenderNodeCube = true;
	m_bRenderActionRouteLine = true;
	m_bRenderPieceEffectRoute = true;
	m_pBufferFontTextureSurface = DexNull; 
	m_pBufferFontTexture = DexNull;
	m_pBackSurface = DexNull;
	m_bLoading = false;
	g_iFps = 0;
	g_iCounter = 0;
	g_iCurrentTime = 0;
	g_iLastTime = 0;
	g_iIconRes = 0;
	g_iMouseX = 0;
	g_iMouseY = 0;
	g_iMouseXLastFrame = 0;
	g_iMouseYLastFrame = 0;
	g_strWindowName = "仙三小传之景天伏魔篇";
	g_strWindowClass = "Pal";
	memset(&g_material, 0, sizeof(D3DXMATERIAL));
	g_material.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_material.Diffuse  = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
	g_material.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_material.Power = 0;
	m_RenderMode = DexRenderMode_TRIANGLE;
	m_pRender = DexNull;
	vecModelLoader.push_back(new DexModelMs3dLoader);
	vecModelLoader.push_back(new DexModelObjLoader);
	vecModelLoader.push_back(new DexModelDaeLoader);
	vecModelLoader.push_back(new DexModelMwsLoader);
	vecModelLoader.push_back(new DexModelSkinMeshLoader);
	vecModelLoader.push_back(new DexModelSkinMeshTextLoader);
	vecModelAniLoader.push_back(new DexModelAniFileFF12ActLoader);
	vecModelAniLoader.push_back(new DexModelAniFileTextLoader);
	g_TextureManager = DexTextureManager::GetManager();
}
DexGameEngine::~DexGameEngine()
{
	_SafeRelease(m_pBufferFontTextureSurface);
	_SafeRelease(m_pBufferFontTexture);
	_SafeRelease(m_pBackSurface);
	_SafeClearVector(vecModelLoader);
	_SafeClearVector(vecModelAniLoader);
	_SafeDelete(g_pDevice);
}
DWORD WINAPI LoadThread(LPVOID lpParam)
{
	while(1)
	{
		if(DexGameEngine::getEngine()->GetLoadingState() != NULL)
		{
			if(DexGameEngine::getEngine()->GetLoading())
			{
				DexGameEngine::getEngine()->GetLoadingState()->Update(16);
				DexGameEngine::getEngine()->GetLoadingState()->Render();
			}
		}
	}
	return 0;
}
DWORD WINAPI LogThread(LPVOID lpParam)
{
	while (1)
	{
		DexLog::getSingleton()->DoLog();
	}
	return 0;
}
void DexGameEngine::SetLoading(string state_name)
{
	m_bLoading = true;
	g_pCurrState = g_pLoadingState;
	g_nextStateName = state_name;
}

IDexDevice* DexGameEngine::CreateDevice(DString deviceType)
{
	if (deviceType == DexDeviceDirectx9::getClassType())
	{
		g_pDevice = new DexDeviceDirectx9();
	}
	else if (deviceType == DexDeviceOpenGLWindows::getClassType())
	{
#ifdef _DEX_PLATFORM_WINDOWS
		g_pDevice = new DexDeviceOpenGLWindows();
#endif
	}
	if (g_pDevice != DexNull)
		g_pDevice->InitDevice(g_bFullscreen, g_iWindowWidth, g_iWinddowHeight, (void*)g_msgPro, NULL);
	return g_pDevice;
}
bool DexGameEngine::InitModuleInfo()
{
#ifdef _DEX_PLATFORM_WINDOWS
	MODULEINFO moduleInfo;
	HANDLE processHandle = GetCurrentProcess();
	HMODULE processModule = GetModuleHandle(NULL);
	GetModuleInformation(processHandle, processModule, &moduleInfo, sizeof(moduleInfo));
	g_stModuleInfo.iModuleAddress = (DEXWORD)moduleInfo.lpBaseOfDll;
	g_stModuleInfo.iStartFunctionAddress = (DEXWORD)moduleInfo.EntryPoint;
	g_stModuleInfo.iModuleSize = (DEXWORD)moduleInfo.SizeOfImage;
	return true;
#endif 
	return false;
}

IDexDevice* DexGameEngine::GetDDevice()
{
	return g_pDevice;
}
HWND DexGameEngine::GetHwnd()
{
	return ((IDexDeviceWindows*)g_pDevice)->GetHwnd();
}
void DexGameEngine::SetHInstance(HINSTANCE instance)
{
	g_hInstance = GetModuleHandle(NULL);
}
HINSTANCE DexGameEngine::GetHInstance()
{
	return ((IDexDeviceWindows*)g_pDevice)->GetHInstance();
}
DexGameEngine::stDexModuleInfo* DexGameEngine::GetModuleInfo()
{
	return &g_stModuleInfo;
}
bool DexGameEngine::GetLoading()
{
	return m_bLoading;
}

DexGameState* DexGameEngine::GetLoadingState()
{
	return g_pLoadingState;
}

DexGameState* DexGameEngine::GetCurrState()
{
	return g_pCurrState;
}

void DexGameEngine::SetLoadingState(DexGameState* pLoadingState)
{
	_SafeDelete(g_pLoadingState);
	//創建加載界面線程
	g_pLoadingState = pLoadingState;
	//g_LoadThread = CreateThread(NULL, 0, LoadThread, NULL, CREATE_SUSPENDED, &g_LoadThreadId);

}

bool DexGameEngine::Initialize(int flag)
{	
/*	DexLog::getSingleton()->EnableFontFrontHighLight();
	DexLog::getSingleton()->EnableBackColor(true);*/
	DexLog::getSingleton()->SetConsoleScreenSize(256, 1000);
	CreateThread(NULL, 0, LogThread, NULL, 0, &g_LogThreadId);
	
	DexStreamFile::sCreateStreamFile();
	DexMd5Generator::sCreateDexMd5Generator();
	DEX_ENSURE_B_LOG(InitModuleInfo(), "init module failed!");
	
	srand(unsigned(time(0)));
	L = lua_open();	   //创建接口指针
	luaopen_base(L);   //加载基本库
	luaL_openlibs(L);  //加载扩展库
	g_camera = new CCamera;
	g_camera->SetCamera(DexVector3(0.0f, 0.0f, -10.0f), DexVector3(0.0f, 0.0f, 0.0f), DexVector3(0.0f, 1.0f, 0.0f));
	if (flag == 0)
	{
		CreateDevice(DexDeviceDirectx9::getClassType());
		g_D3DDevice = ((DexDeviceDirectx9*)g_pDevice)->GetD3D9Device();

		D3DXCreateBox(g_D3DDevice, 1, 1, 1, &g_cube, NULL);
		D3DXCreateSphere(g_D3DDevice, 1, 10, 10, &g_sphere, NULL);
		InitVertexBuff();

		SetWorldAmbient(getD3DColor(DexColor(0.0f, 0.0f, 0.0f)));
		SetProjectArgus(45.0f, ((float)g_iWindowWidth) / ((float)g_iWinddowHeight), 1.0f, 10000.0f);
		CInputSystem::GetInputSingleton();
		CDexObjectFactroy::getObjectFactory();
		CDexScene::RegisterLuaFunction(L);

		m_pRender = new DexRenderDirectX9();
		m_pRender->InitShader();
		GetDevice()->CreateTexture(512, 512, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, &m_pBufferFontTexture, NULL);
		m_pBufferFontTexture->GetSurfaceLevel(0, &m_pBufferFontTextureSurface);
		GetDevice()->GetRenderTarget(0, &DexGameEngine::getEngine()->m_pBackSurface);
	}
	else
	{
		CreateDevice(DexDeviceOpenGLWindows::getClassType());
	}
	g_pDevice->SetRenderState(DEXRENDERSTATE_LIGHTING, FALSE); 
	g_TextureManager = DexTextureManager::GetManager();
	return true;
}


void DexGameEngine::RenderCoorLines()
{
	int z_length = 10;  //z方向上间距 一共10格 200单位
	int x_length = 10;  //x方向上间距 一共10格 200单位
	g_D3DDevice->SetTexture(0, NULL);
	g_D3DDevice->SetFVF(D3DFVF_XYZ| D3DFVF_DIFFUSE);
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, (DexMatrix4x4*)&g_transMatrix);

	//X轴 绿色
	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_GREEN);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_GREEN);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(0, 0, 0);  
	g_pVertexList0[1].m_pos = D3DXVECTOR3(500, 0, 0);  
	g_pDevice->DrawPrimitiveUp(DexPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
	//Y轴 蓝色
	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_BLUE);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_BLUE);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(0, 0, 0);   
	g_pVertexList0[1].m_pos = D3DXVECTOR3(0, 500, 0);  
	g_pDevice->DrawPrimitiveUp(DexPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
	//Z轴 红色
	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_RED);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_RED);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(0, 0, 0);   
	g_pVertexList0[1].m_pos = D3DXVECTOR3(0, 0, 500);   
	g_pDevice->DrawPrimitiveUp(DexPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));

	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_YELLOW);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_YELLOW);
	
	//绘制网格
	for(int i = 0 ; i < 11; i++)
	{
		g_pVertexList0[0].m_pos = D3DXVECTOR3(-50, 0, -50 + i* z_length);   //z
		g_pVertexList0[1].m_pos = D3DXVECTOR3( 50, 0, -50 + i* z_length);   //z
		g_pDevice->DrawPrimitiveUp(DexPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
		g_pVertexList0[0].m_pos = D3DXVECTOR3(-50 + i* x_length, 0, -50);   //x
		g_pVertexList0[1].m_pos = D3DXVECTOR3(-50 + i* x_length, 0,  50);   //x
		g_pDevice->DrawPrimitiveUp(DexPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
	}
}

void DexGameEngine::SetMeshColor(const DexColor& color)
{
	g_material.Diffuse = D3DXCOLOR(color.r, color.g, color.b, color.a);
	g_material.Ambient = g_material.Diffuse;
	g_material.Specular = g_material.Diffuse;
	g_material.Emissive = g_material.Diffuse;
}

void DexGameEngine::RenderCube(const DexVector3& pos, const DexVector3& scale)
{
	DEX_ENSURE(g_cube);
	D3DXMatrixIdentity(&g_transMatrix);
	D3DXMatrixIdentity(&g_worldMatrix);
	D3DXMatrixIdentity(&g_scaleMatrix);
	D3DXMatrixScaling(&g_scaleMatrix, scale.x, scale.y, scale.z);

	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	D3DXMatrixMultiply(&g_worldMatrix, &g_worldMatrix, &g_transMatrix);
	D3DXMatrixMultiply(&g_worldMatrix, &g_worldMatrix, &g_scaleMatrix);
	D3DXMatrixTranslation(&g_transMatrix, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&g_worldMatrix, &g_worldMatrix, &g_transMatrix);
	


	g_D3DDevice->SetTexture(0, NULL);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_cube->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	//g_D3DDevice->SetMaterial(NULL);
}
void DexGameEngine::RenderCube(const D3DXMATRIX& matrix)
{
	DEX_ENSURE(g_cube);
	g_D3DDevice->SetTexture(0, NULL);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, (DexMatrix4x4*)&matrix);
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_cube->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, (DexMatrix4x4*)&g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void DexGameEngine::RenderCube(const DexMatrix4x4& matrix)
{
	DEX_ENSURE(g_cube);
	g_D3DDevice->SetTexture(0, NULL);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, &matrix);
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_cube->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, (DexMatrix4x4*)&g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}


void DexGameEngine::RenderSphere(const DexVector3& pos, float scale /* = 1.0f */)
{
	DEX_ENSURE(g_sphere);
	D3DXMatrixIdentity(&g_transMatrix);
	D3DXMatrixIdentity(&g_worldMatrix);
	D3DXMatrixIdentity(&g_scaleMatrix);
	D3DXMatrixScaling(&g_scaleMatrix, scale, scale, scale);

	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	D3DXMatrixMultiply(&g_worldMatrix, &g_worldMatrix, &g_transMatrix);
	D3DXMatrixMultiply(&g_worldMatrix, &g_worldMatrix, &g_scaleMatrix);
	D3DXMatrixTranslation(&g_transMatrix, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&g_worldMatrix, &g_worldMatrix, &g_transMatrix);

	g_D3DDevice->SetTexture(0, NULL);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_sphere->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}


void DexGameEngine::Render3DLine(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const DexColor& color1 /* = 0xffffffff */, const DexColor& color2 /* = 0xffffffff */)
{
	g_pVertexList1[0].m_pos = p0;
	g_pVertexList1[1].m_pos = p1;
	g_pVertexList1[0].m_color = getD3DColor(color1);
	g_pVertexList1[1].m_color = getD3DColor(color2);
	D3DXMatrixIdentity(&g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList1, sizeof(stVertex1));
}

void DexGameEngine::Render3DLine(const DexVector3& p0, const DexVector3& p1, const DexColor& color1 /* = 0xffffffff */, const DexColor& color2 /* = 0xffffffff */)
{
	memcpy(&g_pVertexList1[0].m_pos, &p0, sizeof(p0));
	memcpy(&g_pVertexList1[1].m_pos, &p1, sizeof(p1));
	g_pVertexList1[0].m_color = getD3DColor(color1);
	g_pVertexList1[1].m_color = getD3DColor(color2);
	D3DXMatrixIdentity(&g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, (DexMatrix4x4*)&g_worldMatrix);
	g_pDevice->DrawPrimitiveUp(DexPT_LINELIST, 1, g_pVertexList1, sizeof(stVertex1));
}
void DexGameEngine::Render3DLine(const DexVector3& p, const DexVector3& vec, const DexColor& color1, float length, const DexColor& color2)
{
	g_pVertexList1[0].SetPos(p);
	g_pVertexList1[0].m_color = getD3DColor(color1);
	DexVector3 vector = vec;
	vector.Normalize();
	g_pVertexList1[1].SetPos(p + length * vector);
	g_pVertexList1[1].m_color = getD3DColor(color2);
	D3DXMatrixIdentity(&g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, (DexMatrix4x4*)&g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 1, g_pVertexList1, sizeof(stVertex1));
}

void DexGameEngine::Render3DLine(const D3DXVECTOR3& p, const D3DXVECTOR3& vec, const DexColor& color1, float length, const DexColor& color2)
{
	g_pVertexList1[0].m_pos = p;
	g_pVertexList1[0].m_color = getD3DColor(color1);
	D3DXVECTOR3 vector;
	D3DXVec3Normalize(&vector, &vec);
	g_pVertexList1[1].m_pos = p + length * vector;
	g_pVertexList1[1].m_color = getD3DColor(color2);
	D3DXMatrixIdentity(&g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList1, sizeof(stVertex1));
}

void DexGameEngine::SetMaterial(const DexMaterial& material)
{
	size_t size = sizeof(DexColor);
	memcpy(&g_material.Diffuse, &material.diffuse, size);
	memcpy(&g_material.Ambient, &material.ambient, size);
	memcpy(&g_material.Emissive, &material.emissive, size);
	memcpy(&g_material.Specular, &material.specular, size);
	g_material.Power = material.power;
	g_D3DDevice->SetMaterial(&g_material);
}
void DexGameEngine::SetTexture(DInt32 stage, CDexTex* texture)
{
	LPDIRECT3DTEXTURE9 dx_tex = NULL;
	if (texture != NULL)
	{
		dx_tex = texture->GetTexPt();
	}
	g_D3DDevice->SetTexture(stage, dx_tex);
}
/*
使用示例：
void* test_primitive_vertex;
void* test_primitive_indice;
stVertex0	vertexs[8];
vertexs[0].m_pos = D3DXVECTOR3(30.0f, 0, -30.0f); vertexs[0].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[1].m_pos = D3DXVECTOR3(30.0f, 0, 30.0f); vertexs[1].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[2].m_pos = D3DXVECTOR3(-30.0f, 0, 30.0f); vertexs[2].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[3].m_pos = D3DXVECTOR3(-30.0f, 0, -30.0f); vertexs[3].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[4].m_pos = D3DXVECTOR3(30.0f, 60, -30.0f); vertexs[4].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[5].m_pos = D3DXVECTOR3(30.0f, 60, 30.0f); vertexs[5].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[6].m_pos = D3DXVECTOR3(-30.0f, 60, 30.0f); vertexs[6].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
vertexs[7].m_pos = D3DXVECTOR3(-30.0f, 60, -30.0f); vertexs[7].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f));
test_primitive_vertex = (void*)malloc(sizeof(stVertex0) * 8);
memcpy(test_primitive_vertex, vertexs, sizeof(stVertex0)*8);
DInt32 indices[] = {0,1,4,1,5,4,2,3,6,6,3,7,0,4,3,3,4,7,1,2,5,2,6,5,1,0,3,1,3,2,4,5,7,7,5,6};//逆时针索引
test_primitive_indice = (void*)malloc(sizeof(indices));
memcpy(test_primitive_indice, indices, sizeof(indices));

Render:
DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, test_primitive_vertex, 8, test_primitive_indice, 12, sizeof(stVertex0));
*/
void DexGameEngine::DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 fvf, DInt32 stridesize)
{
	D3DPRIMITIVETYPE d3d_primitive_type = D3DPT_LINELIST;
	switch (type)
	{
	case DexPT_POINTLIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_POINTLIST;break;}
	case DexPT_LINELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINELIST; break; }
	case DexPT_LINESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINESTRIP; break; }
	case DexPT_TRIANGLELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST; break; }
	case DexPT_TRIANGLESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP; break; }
	case DexPT_TRIANGLEFAN:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLEFAN; break; }
	default:
		break;
	}
	DexGameEngine::getEngine()->GetDevice()->SetFVF((DWORD)fvf);
	//DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertexs, sizeof(stVertex0));
	//这里的D3DFMT_INDEX32，如果indices是DInt32整型数组的话，设置D3DFMT_INDEX32,如果是DInt16类型的整型数组的话，那么设置D3DFMT_INDEX16
	g_D3DDevice->DrawIndexedPrimitiveUP(d3d_primitive_type, 0, vertexCount, primitiveCount, indices, D3DFMT_INDEX32, vertexs, stridesize);
}

void DexGameEngine::DrawPrimitive(DexPrimitivetType type, const void* vertexs, DInt32 vertexCount, const void* indices, DInt32 primitiveCount, DInt32 stridesize)
{
	D3DPRIMITIVETYPE d3d_primitive_type = D3DPT_LINELIST;
	switch (type)
	{
	case DexPT_POINTLIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_POINTLIST; break; }
	case DexPT_LINELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINELIST; break; }
	case DexPT_LINESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINESTRIP; break; }
	case DexPT_TRIANGLELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST; break; }
	case DexPT_TRIANGLESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP; break; }
	case DexPT_TRIANGLEFAN:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLEFAN; break; }
	default:
		break;
	}
	//DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertexs, sizeof(stVertex0));
	//这里的D3DFMT_INDEX32，如果indices是DInt32整型数组的话，设置D3DFMT_INDEX32,如果是DInt16类型的整型数组的话，那么设置D3DFMT_INDEX16
	g_D3DDevice->DrawIndexedPrimitiveUP(d3d_primitive_type, 0, vertexCount, primitiveCount, indices, D3DFMT_INDEX32, vertexs, stridesize);
}

DexModelBase* DexGameEngine::CreateModel(const char* filename, int flag)
{
	DexModelBase* ret = NULL;
	const char* suffix = dexstrchr(filename, '.');
	for (DUDInt32 i = 0; i < vecModelLoader.size(); ++i)
	{
		if (vecModelLoader[i] != NULL && vecModelLoader[i]->SupportType(suffix))
		{
			ret = vecModelLoader[i]->LoadModel(filename, flag);
			break;
		}
	}
	return ret;
}

bool DexGameEngine::SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int flag/* =0 */)
{
	bool ret = false;
	const char* suffix = dexstrchr(filename, '.');
	for (DUDInt32 i = 0; i < vecModelLoader.size(); ++i)
	{
		if (vecModelLoader[i] != NULL && vecModelLoader[i]->SupportType(suffix))
		{
			ret = vecModelLoader[i]->SaveModel(pSkinMesh, filename, flag);
			break;
		}
	}
	return ret;
}


bool DexGameEngine::ReadModelAnimation(DexSkinMesh* pDexSkinMesh, const char* filename)
{
	const char* suffix = dexstrchr(filename, '.');
	for (DUDInt32 i = 0; i < vecModelAniLoader.size(); ++i)
	{
		if (vecModelAniLoader[i] != NULL && vecModelAniLoader[i]->SupportType(suffix))
		{
			return vecModelAniLoader[i]->LoadAnimationFile(pDexSkinMesh, filename);
		}
	}
	return false;
}

bool DexGameEngine::ReadFFSkeletonInfo(DexSkinMesh* pDexSkinMesh, DString filename)
{
	for (DUDInt32 i = 0; i < vecModelLoader.size(); ++i)
	{
		if (vecModelLoader[i] != NULL && vecModelLoader[i]->SupportType(".dae"))
		{
			return ((DexModelDaeLoader*)vecModelLoader[i])->ReadFFSkeletonInfo(pDexSkinMesh, filename);
		}
	}
	return false;
}


bool DexGameEngine::AddState(DexGameState* state)
{
	DEX_ENSURE_B(state);
	string state_name = state->getType();
	if(g_States.find(state_name) == g_States.end())
	{
		g_States[state_name] = state;
		return true;
	}
	return false;
}

DexGameState* DexGameEngine::getGameState(string type)
{
	if(g_States.find(type) != g_States.end())
	{
		return g_States[type];
	}
	return NULL;
}

void DexGameEngine::SetCurrState(string state_name)
{
	//SetLoading(true);
	if(g_States.find(state_name) != g_States.end())
	{
		if(g_pCurrState != NULL)
		{//如果之前有State 先離開
			g_pCurrState->OnLeaveState();
		}
		g_pCurrState = g_States[state_name];  
		if(g_pCurrState->NeedApply())
		{
			g_pCurrState->ApplyRes();
		}
		g_pCurrState->OnEnterState();
	}
	//SetLoading(false);
}

void DexGameEngine::UpdateFps()
{
	g_iCurrentTime = getTime()->GetTotalMillSeconds();
	if(g_iCurrentTime - g_iLastTime > 1000.0f)
	{
		g_iLastTime = g_iCurrentTime;
		g_iFps = g_iCounter;
		g_iCounter = 0;
	}
	else
	{
		g_iCounter++;
	}
}

void DexGameEngine::CalculateLightData()
{
}
void DexGameEngine::Run()
{
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));

	while(msg.message != WM_QUIT)
	{
		
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}	
		//EnterCriticalSection(&g_cs);
		//if(!m_bLoading)
		//{
		getTime()->BeginGameCycle();
		Update();
		Render();
		getTime()->EndGameCycle();
		//}
		//LeaveCriticalSection(&g_cs);
	}
}
void DexGameEngine::Update()
{
	//计算帧数，和state没有关系，故在此计算
	UpdateFps();
	updateViewMatrix();
	static DInt64 lastTickTime = 0;
	if(lastTickTime == 0)
		lastTickTime = g_iCurrentTime;
	int delta = g_iCurrentTime - lastTickTime;
	lastTickTime = g_iCurrentTime;
	
	
	CInputSystem::GetInputSingleton().Update(GetHwnd());
	g_iMouseX = (int)CInputSystem::GetInputSingleton().GetMouseXPos();
	g_iMouseY = (int)CInputSystem::GetInputSingleton().GetMouseYPos(); 

	DEX_ENSURE(g_pCurrState);
	if(g_iMouseX !=g_iMouseXLastFrame || g_iMouseY != g_iMouseYLastFrame)
	{
		g_pCurrState->MouseMove(g_iMouseX, g_iMouseY);
	}
	if(CInputSystem::GetInputSingleton().WheelFront())
	{
		g_pCurrState->MouseWheelFront();
	}
	if(CInputSystem::GetInputSingleton().WheelBack())
	{
		g_pCurrState->MouseWheelBack();
	}
	if(CInputSystem::GetInputSingleton().LeftButtonDown())
	{
		g_pCurrState->MouseLDown(g_iMouseX, g_iMouseY);
	}
	if(CInputSystem::GetInputSingleton().LeftButtonUp())
	{
		g_pCurrState->MouseLUp(g_iMouseX, g_iMouseY);
	}
	if(CInputSystem::GetInputSingleton().RightButtonDown())
	{
		g_pCurrState->MouseRDown(g_iMouseX, g_iMouseY);
	}
	if(CInputSystem::GetInputSingleton().RightButtonUp())
	{
		g_pCurrState->MouseRUp(g_iMouseX, g_iMouseY);
	}
	
	getCamera()->Update(delta);
	
	g_pCurrState->KeyUp();
	g_pCurrState->KeyDown();
	g_pCurrState->Update(delta);
	g_iMouseXLastFrame = g_iMouseX;
	g_iMouseYLastFrame = g_iMouseY;
}
void DexGameEngine::Render()
{
	if(m_bLoading)
	{
		g_pLoadingState->Render();
		SetCurrState(g_nextStateName);
		m_bLoading = false;
	}
	DEX_ENSURE(g_pCurrState);
	g_pDevice->BeginScene();

	g_pDevice->Clear(DEXCLEAR_TARGET | DEXCLEAR_ZBUFFER, DexColor(125,125,125), 1.0, 0);
	g_pCurrState->BeginRender();
	g_pCurrState->Render();
	g_pCurrState->EndRender();

	g_pDevice->EndScene();
}

void DexGameEngine::SetRenderMode(DexRenderMode mode)
{
	m_RenderMode = mode;
}

DexRenderMode DexGameEngine::GetRenderMode()
{
	return m_RenderMode;
}

void DexGameEngine::OnDragFiles(const DVector<DString>& FileNames)
{
	DEX_ENSURE(g_pCurrState != DexNull);
	g_pCurrState->OnDragFiles(FileNames);
}

#define case_value2value(value1, para, value2) \
case value1:\
{\
	para = value2;\
	break;\
}

//void DexGameEngine::SetRenderState(Dex_RenderState state, DWORD value)
//{
// 	D3DRENDERSTATETYPE d3dState;
// 	switch(state)
// 	{
// 		case_value2value(DEXRS_ZENABLE, d3dState, D3DRS_ZENABLE)
// 		//case_value2value(DEXRS_ZVISIBLE, d3dState, D3DRS_ZVISIBLE)
// 		case_value2value(DEXRS_ZWRITEENABLE, d3dState, D3DRS_ZWRITEENABLE)
// 		case_value2value(DEXRS_ALPHATESTENABLE, d3dState, D3DRS_ALPHATESTENABLE)
// 		case_value2value(DEXRS_SRCBLEND, d3dState, D3DRS_SRCBLEND)
// 		case_value2value(DEXRS_DESTBLEND, d3dState, D3DRS_DESTBLEND)
// 		case_value2value(DEXRS_CULLMODE, d3dState, D3DRS_CULLMODE)
// 		case_value2value(DEXRS_ALPHABLENDENABLE, d3dState, D3DRS_ALPHABLENDENABLE)
// 		case_value2value(DEXRS_FOGDENSITY, d3dState, D3DRS_FOGDENSITY)
// 		case_value2value(DEXRS_RANGEFOGENABLE, d3dState, D3DRS_RANGEFOGENABLE)
// 		case_value2value(DEXRS_STENCILENABLE, d3dState, D3DRS_STENCILENABLE)
// 		case_value2value(DEXRS_AMBIENT, d3dState, D3DRS_AMBIENT)
// 		case_value2value(DEXRS_POINTSIZE, d3dState, D3DRS_POINTSIZE)
// 		case_value2value(DEXRS_POINTSIZE_MIN, d3dState, D3DRS_POINTSIZE_MIN)
// 		case_value2value(DEXRS_POINTSPRITEENABLE, d3dState, D3DRS_POINTSPRITEENABLE)
// 		case_value2value(DEXRS_POINTSCALEENABLE, d3dState, D3DRS_POINTSCALEENABLE)
// 		case_value2value(DEXRS_POINTSCALE_A, d3dState, D3DRS_POINTSCALE_A)
// 		case_value2value(DEXRS_POINTSCALE_B, d3dState, D3DRS_POINTSCALE_B)
// 		case_value2value(DEXRS_POINTSCALE_C, d3dState, D3DRS_POINTSCALE_C)
// 		case_value2value(DEXRS_POINTSIZE_MAX, d3dState, D3DRS_POINTSIZE_MAX)
// 		case_value2value(DEXRS_INDEXEDVERTEXBLENDENABLE, d3dState, D3DRS_INDEXEDVERTEXBLENDENABLE)
// 		case_value2value(DEXRS_BLENDOP, d3dState, D3DRS_BLENDOP)
// 	default:
// 		DexLog::getSingleton()->BeginLog();
// 		DexLog::getSingleton()->Log(log_allert, "Dex调用了一个不支持的render state命令!");
// 		DexLog::getSingleton()->EndLog();
// 		return;
// 	}
// 	DWORD d3dValue;
// 	switch(value)
// 	{
// 		//blend 
// 		case_value2value(DEXBLEND_ZERO, d3dValue, D3DBLEND_ZERO)
// 		case_value2value(DEXBLEND_ONE, d3dValue, D3DBLEND_ONE)
// 		case_value2value(DEXBLEND_SRCCOLOR, d3dValue, D3DBLEND_SRCCOLOR)
// 		case_value2value(DEXBLEND_INVSRCCOLOR, d3dValue, D3DBLEND_INVSRCCOLOR)
// 		case_value2value(DEXBLEND_INVSRCALPHA, d3dValue, D3DBLEND_INVSRCALPHA)
// 		case_value2value(DEXBLEND_DESTALPHA, d3dValue, D3DBLEND_DESTALPHA)
// 		case_value2value(DEXBLEND_INVDESTALPHA, d3dValue, D3DBLEND_INVDESTALPHA)
// 		case_value2value(DEXBLEND_DESTCOLOR, d3dValue, D3DBLEND_DESTCOLOR)
// 		case_value2value(DEXBLEND_INVDESTCOLOR, d3dValue, D3DBLEND_INVDESTCOLOR)
// 		case_value2value(DEXBLEND_SRCALPHASAT, d3dValue, D3DBLEND_SRCALPHASAT)
// 		case_value2value(DEXBLEND_BOTHSRCALPHA, d3dValue, D3DBLEND_BOTHSRCALPHA)
// 		case_value2value(DEXBLEND_BOTHINVSRCALPHA, d3dValue, D3DBLEND_BOTHINVSRCALPHA)
// 		//fog
// 		case_value2value(DEMOEX_FOG_LINER, d3dValue, D3DFOG_LINEAR)
// 		case_value2value(DEMOEX_FOG_EXP, d3dValue, D3DFOG_EXP)
// 
// 	}
//}

void DexGameEngine::DisableFog()
{
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void DexGameEngine::EnableFog()
{
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

bool DexGameEngine::OpenFog()
{
	DWORD fog;
	DexGameEngine::getEngine()->GetDevice()->GetRenderState(D3DRS_FOGENABLE, &fog);
	return fog;
}

void DexGameEngine::SetFogColor(const DexColor& color)
{
	DWORD d3dColor = getD3DColor(color);
	g_D3DDevice->SetRenderState(D3DRS_FOGCOLOR, d3dColor);
}

void DexGameEngine::SetFogMode(DEMOEX_FOG_MODE mode)
{
	DWORD value = D3DFOG_NONE;
	switch(mode)
	{
		case_value2value(DEMOEX_FOG_LINER, value, D3DFOG_LINEAR)
		case_value2value(DEMOEX_FOG_EXP, value, D3DFOG_EXP)
		case_value2value(DEMOEX_FOG_EXP2, value, D3DFOG_EXP2)
	}
	g_D3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, value);
}

void DexGameEngine::SetFogStart(float start)
{
	g_D3DDevice->SetRenderState(D3DRS_FOGSTART, FtoDW(start));
}

void DexGameEngine::SetFogEnd(float end)
{
	g_D3DDevice->SetRenderState(D3DRS_FOGEND, FtoDW(end));
}

void DexGameEngine::SetFogDensity(float density)
{
	g_D3DDevice->SetRenderState(D3DRS_FOGDENSITY, FtoDW(density));
}

void DexGameEngine::DisableRangeFog()
{
	g_D3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
}

void DexGameEngine::EnableRangeFog()
{
	D3DCAPS9 caps;
	g_D3DDevice->GetDeviceCaps(&caps);
	if(caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE)
		g_D3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}

void DexGameEngine::SetWorldAmbient(const DexColor& color)
{
	g_ambientColor = color;
	//g_D3DDevice->SetRenderState(D3DRS_AMBIENT, getD3DColor(color));
	CalculateLightData();
}

void DexGameEngine::SetLightEffect(bool enable)
{
	m_bLightEffect = enable;
	CalculateLightData();
}

void DexGameEngine::SetLightEnable(bool enable)
{
	m_bLightEnable = enable;
	CalculateLightData();
	//g_D3DDevice->SetRenderState(D3DRS_LIGHTING, enable);
}

void DexGameEngine::SetLightIdEnable(DInt32 lightId, bool enable)
{
	for (size_t i = 0; i < g_vecLight.size(); ++i)
	{
		if (g_vecLight[i].id == lightId)
		{
			g_vecLight[i].enable = enable;
			return;
		}
	}
	CalculateLightData();
	//g_D3DDevice->LightEnable(lightId, enable);
}

DexLight* DexGameEngine::GetLight(DInt32 lightId)
{
	for (size_t i = 0; i < g_vecLight.size(); ++i)
	{
		if (g_vecLight[i].id == lightId)
		{
			return &g_vecLight[i];
		}
	}
	return NULL;
}

void DexGameEngine::AddLight(const DexLight& light)
{
	g_vecLight.push_back(light);
	CalculateLightData();
}

bool DexGameEngine::GetLightEnable()
{
	return m_bLightEnable;
}

CCamera* DexGameEngine::getCamera()
{
	return g_camera;
}

bool DexGameEngine::DoLuaFile(const char* filename)
{
	DexLog::getSingleton()->BeginLog();
	if(int error = luaL_dofile(L, filename) != 0)   //打开文件并编译
	{	
		DexLog::getSingleton()->Log(log_allert, "DoLuaFile: %s error!", filename);
		return false;
	}
	DexLog::getSingleton()->Log(log_ok, "DoLuaFile: %s ok!", filename);
	DexLog::getSingleton()->EndLog();
	return true;
}

bool DexGameEngine::DoLuaString(const char* str)
{
	DexLog::getSingleton()->BeginLog();
	if(int error = luaL_dostring(L, str) != 0)   //執行一行
	{	
		DexLog::getSingleton()->Log(log_allert, "DoLuaString: %s error!", str);
		return false;
	}
	DexLog::getSingleton()->Log(log_ok, "DoLuaString: %s ok!", str);
	DexLog::getSingleton()->EndLog();
	return true;
}

DexMem& DexGameEngine::getMem()
{
	return g_mem;
}

void DexGameEngine::SetProjectArgus(float fov, float aspect, float near_distance, float far_distance)
{
	g_camera->SetProject(fov, aspect, near_distance, far_distance);
	D3DXMatrixPerspectiveFovLH(&g_projection, fov,
		aspect, near_distance, far_distance);
	g_pDevice->SetTransform(DEXTRANSFORMSTATETYPE_PROJECTION, (DexMatrix4x4*)&g_projection);
}

void DexGameEngine::LookAtLH(const DexVector3 *pEye, const DexVector3 *pAt, const DexVector3 *pUp)
{
	g_camera->SetCamera(*pEye, *pAt, *pUp);
	updateViewMatrix();
}

void DexGameEngine::setRendeCollideMesh(bool b)
{
	m_bRenderCollideMesh = b;
}

bool DexGameEngine::getRendeCollideMesh()
{
	return m_bRenderCollideMesh;
}

void DexGameEngine::setRenderNodeCube(bool b)
{
	m_bRenderNodeCube = b;
}

bool DexGameEngine::getRenderNodeCube()
{
	return m_bRenderNodeCube;
}

void DexGameEngine::setRenderActionRouteLine(bool b)
{
	m_bRenderActionRouteLine = b;
}

bool DexGameEngine::getRenderActionRouteLine()
{
	return m_bRenderActionRouteLine;
}

void DexGameEngine::setRenderPieceEffectRoute(bool b)
{
	m_bRenderPieceEffectRoute = b;
}

bool DexGameEngine::getRenderPieceEffectRoute()
{
	return m_bRenderPieceEffectRoute;
}

void DexGameEngine::SetCameraPos(const DexVector3& pos)
{
	g_camera->SetPosition(pos);
	updateViewMatrix();
}
void DexGameEngine::SetCameraLookAt(const DexVector3& lookAt)
{
	g_camera->SetFocus(lookAt);
	updateViewMatrix();
}
void DexGameEngine::MoveCamera(DexVector3 vec, float amount, bool move_focus /* = false */)
{
	g_camera->MoveCamera(vec, amount, move_focus);
	updateViewMatrix();
}
void DexGameEngine::MoveCamera(CCamera::MOVE_DIRECTION direction, float amount, bool move_focus /* = false */)
{
	g_camera->MoveCamera(direction, amount, move_focus);
	updateViewMatrix();
}
void DexGameEngine::RotateCameraByFocus(float value)
{
	g_camera->RotateFocus(value);
	updateViewMatrix();
}

void DexGameEngine::GetRay(int x, int y, stRay& ray)
{
	float px=0;
	float py=0;
	D3DVIEWPORT9 viewport;
	g_D3DDevice->GetViewport(&viewport);
	//视区、投影变换
	px=(((2.0f*x)/viewport.Width)-1.0f)/g_projection(0,0);
	py=(((-2.0f*y)/viewport.Height)+1.0f)/g_projection(1,1);
	ray.m_origin = DexVector3(0, 0, 0);
	ray.m_vector = DexVector3(px, py, 1);

	D3DXMATRIX oppo_view;
	//矩阵求逆
	D3DXMatrixInverse(&oppo_view, 0, &g_ViewMatrix);

	D3DXVECTOR3 origin, vector;
	memcpy(&origin, &ray.m_origin, sizeof(D3DXVECTOR3));
	memcpy(&vector, &ray.m_vector, sizeof(D3DXVECTOR3));
	D3DXVec3TransformCoord(&origin, &origin, &oppo_view);
	D3DXVec3TransformNormal(&vector, &vector, &oppo_view);
	memcpy(&ray.m_origin, &origin, sizeof(D3DXVECTOR3));
	memcpy(&ray.m_vector, &vector, sizeof(D3DXVECTOR3));
	ray.m_vector.Normalize();
}
DexGUI::DexPoint  DexGameEngine::GetXY(const D3DXVECTOR3& world_point)
{
	DexGUI::DexPoint p;
	stRay ray;
	ray.m_origin = g_camera->GetPosition();
	ray.m_vector = ray.m_origin - DexVector3(world_point.x, world_point.y, world_point.z);  
	ray.m_vector.Normalize();

	D3DXMATRIX oppo_view;
	//矩阵求逆
	D3DXMatrixInverse(&oppo_view, 0, &g_ViewMatrix);

	//D3DXVec3TransformCoord(&ray.m_origin, &ray.m_origin, &oppo_view);
	//D3DXVec3TransformNormal(&ray.m_vector, &ray.m_vector, &oppo_view);
	D3DXVECTOR3 origin, vector;
	memcpy(&origin, &ray.m_origin, sizeof(D3DXVECTOR3));
	memcpy(&vector, &ray.m_vector, sizeof(D3DXVECTOR3));
	D3DXVec3TransformCoord(&origin, &origin, &g_ViewMatrix);
	D3DXVec3TransformNormal(&vector, &vector, &g_ViewMatrix);
	memcpy(&ray.m_origin, &origin, sizeof(D3DXVECTOR3));
	memcpy(&ray.m_vector, &vector, sizeof(D3DXVECTOR3));
	float px = ray.m_vector.x/ray.m_vector.z;
	float py = ray.m_vector.y/ray.m_vector.z;
	D3DVIEWPORT9 viewport;
	g_D3DDevice->GetViewport(&viewport);
	//px=(((2.0f*x)/viewport.Width)-1.0f)/projection(0,0);
	//py=(((-2.0f*y)/viewport.Height)+1.0f)/projection(1,1);
	p.x = (px * g_projection(0,0) + 1.0f) * viewport.Width / 2.0f;
	p.y = (py * g_projection(1,1) - 1.0f) * viewport.Height / (-2.0f); 
	return p;
}
void DexGameEngine::updateViewMatrix()
{
	DexVector3 eye = g_camera->GetPosition();
	DexVector3 lootAt = g_camera->GetFocusPoint();
	DexVector3 upDir = g_camera->GetUpVector();
	g_pDevice->LookAt(eye, lootAt, upDir);
}

void DexGameEngine::setDexVertexDecl(IDexVertexDecl* decl)
{
	DEX_ENSURE(decl);
	//ifdef dx9
	g_D3DDevice->SetVertexDeclaration(((DexVertexDeclDx9*)decl)->m_pDecl);
}

DexTextureManager* DexGameEngine::GetTextureManager()
{
	return g_TextureManager;
}
