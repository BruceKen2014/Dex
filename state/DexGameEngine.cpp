

#include "DexGameEngine.h"
#include "DexGameState.h"
// #include "../DexBase/DexCollideObject.h"
// #include "../DexBase/CDexPartical.h"
// #include "../DexBase/DexTerrain.h"
// #include "../DexBase/DexSceneObjectFlower.h"
#include "../DexBase/Dex3DScene.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/CInput.h"
#include "../DexBase/CDexTime.h"
#include "../DexBase/DexObjectFactory.h"
#include "../DexMath/Dex_To_Dx.h"

DexGameEngine* DexGameEngine::g_pGameEngine = NULL;
CRITICAL_SECTION  g_cs;
DexGameEngine::DexGameEngine()
{
	g_bFullscreen = false;
	g_iWindowPosX = 100;
	g_iWindowPosY = 100;
	g_iWindowWidth = 800;
	g_iWinddowHeight = 600;
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
}
DexGameEngine::~DexGameEngine()
{
	UnregisterClass(g_strWindowClass.c_str(), g_hInstance);
	m_pBufferFontTextureSurface->Release();
	m_pBufferFontTexture->Release();
	m_pBackSurface->Release();
}
DWORD WINAPI LoadThread(LPVOID lpParam)
{
	while(1)
	{
		EnterCriticalSection(&g_cs);
		if(DexGameEngine::getEngine()->GetLoadingState() != NULL)
		{
			if(DexGameEngine::getEngine()->GetLoading())
			{
				DexGameEngine::getEngine()->GetLoadingState()->Update(16);
				DexGameEngine::getEngine()->GetLoadingState()->Render();
			}
		}
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}
void DexGameEngine::SetLoading(string state_name)
{
	m_bLoading = true;
	g_nextStateName = state_name;
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
	//InitializeCriticalSection(&g_cs);
	//g_LoadThread = CreateThread(NULL, 0, LoadThread, NULL, 0, &g_LoadThreadId);

}

bool DexGameEngine::Initialize()
{	
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, g_msgPro, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Pal", NULL };
	wc.hIcon = LoadIcon(g_hInstance,MAKEINTRESOURCE(g_iIconRes));
	RegisterClassEx(&wc);

	// Create the application's window
	DWORD style;
	if(g_bFullscreen)
		style = WS_POPUP; //popup会创造一个无边框的窗口 在全屏要这样做 否则鼠标坐标会有变差
	else
		style = WS_OVERLAPPEDWINDOW;
	g_hWnd = CreateWindow(g_strWindowClass.c_str(), g_strWindowName.c_str(), style ,
		g_iWindowPosX, g_iWindowPosY, g_iWindowWidth, g_iWinddowHeight, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);

	ShowWindow(g_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hWnd);	


/*	getLog()->EnableFontFrontHighLight();
	getLog()->EnableBackColor(true);*/
	getLog()->SetConsoleScreenSize(256, 1000);	   
	srand(unsigned(time(0)));

	D3DDISPLAYMODE displayMode;

	// Create the D3D object.
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_D3D == NULL) 
		return false;

	// Get the desktop display mode.
	if(FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));


	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;

	if(g_D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, displayMode.Format, !g_bFullscreen,
		D3DMULTISAMPLE_8_SAMPLES,
		NULL) == D3D_OK) 
		multiType = D3DMULTISAMPLE_8_SAMPLES;

	if(g_bFullscreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = g_iWindowWidth;
		d3dpp.BackBufferHeight = g_iWinddowHeight;
		//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
		d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.MultiSampleType = multiType;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Create the D3DDevice
	getLog()->BeginLog();
	if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &g_D3DDevice)))
	{

		getLog()->Log(log_ok, "D3D CreateDevice失败！");
		getLog()->EndLog();
		return false;
	}
	getLog()->Log(log_ok, "D3D CreateDevice成功！");
	getLog()->EndLog();

	D3DXCreateBox(g_D3DDevice, 1, 1, 1, &g_cube, NULL);
	D3DXCreateSphere(g_D3DDevice, 1, 10, 10, &g_sphere, NULL);
	InitVertexBuff();

	L = lua_open();	   //创建接口指针
	luaopen_base(L);   //加载基本库
	luaL_openlibs(L);  //加载扩展库

	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, getD3DColor(DexColor(1.0f, 1.0f, 1.0f))); 
	g_camera = new CCamera;
	SetProjectArgus(45.0f, ((float)g_iWindowWidth)/((float)g_iWinddowHeight), 1.0f, 10000.0f);
	CInputSystem::GetInputSingleton();
	CDexObjectFactroy::getObjectFactory();
	CDexScene::RegisterLuaFunction(L);
	GetDevice()->CreateTexture(512,512,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, & m_pBufferFontTexture, NULL);
	m_pBufferFontTexture->GetSurfaceLevel(0, &m_pBufferFontTextureSurface);
	GetDevice()->GetRenderTarget(0, &DexGameEngine::getEngine()->m_pBackSurface);
	return true;
}


void DexGameEngine::RenderCoorLines()
{
	int z_length = 10;  //z方向上间距 一共10格 200单位
	int x_length = 10;  //x方向上间距 一共10格 200单位
	g_D3DDevice->SetTexture(0, NULL);
	g_D3DDevice->SetFVF(D3DFVF_XYZ| D3DFVF_DIFFUSE);
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &g_transMatrix);

	//X轴 绿色
	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_GREEN);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_GREEN);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(0, 0, 0);  
	g_pVertexList0[1].m_pos = D3DXVECTOR3(500, 0, 0);  
	g_D3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
	//Y轴 蓝色
	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_BLUE);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_BLUE);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(0, 0, 0);   
	g_pVertexList0[1].m_pos = D3DXVECTOR3(0, 500, 0);  
	g_D3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
	//Z轴 红色
	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_RED);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_RED);
	g_pVertexList0[0].m_pos = D3DXVECTOR3(0, 0, 0);   
	g_pVertexList0[1].m_pos = D3DXVECTOR3(0, 0, 500);   
	g_D3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));

	g_pVertexList0[0].m_color = getD3DColor(DEXCOLOR_YELLOW);
	g_pVertexList0[1].m_color = getD3DColor(DEXCOLOR_YELLOW);
	
	//绘制网格
	for(int i = 0 ; i < 11; i++)
	{
		g_pVertexList0[0].m_pos = D3DXVECTOR3(-50, 0, -50 + i* z_length);   //z
		g_pVertexList0[1].m_pos = D3DXVECTOR3( 50, 0, -50 + i* z_length);   //z
		g_D3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
		g_pVertexList0[0].m_pos = D3DXVECTOR3(-50 + i* x_length, 0, -50);   //x
		g_pVertexList0[1].m_pos = D3DXVECTOR3(-50 + i* x_length, 0,  50);   //x
		g_D3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, g_pVertexList0, sizeof(stVertex0));
	}
}

void DexGameEngine::SetMeshColor(const DexColor& color)
{
	g_material.Diffuse = D3DXCOLOR(color.r, color.g, color.b, color.a);
	g_material.Ambient = g_material.Diffuse;
	g_material.Specular = g_material.Diffuse;
	g_material.Emissive = g_material.Diffuse;
}

void DexGameEngine::RenderCube(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale)
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
	
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
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

void DexGameEngine::RenderCube(const DexVector3& pos, const D3DXVECTOR3& scale)
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

	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_D3DDevice->SetTexture(0, NULL);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_cube->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//g_D3DDevice->SetMaterial(NULL);
}

void DexGameEngine::RenderCube(const D3DXMATRIX& matrix)
{
	DEX_ENSURE(g_cube);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	g_D3DDevice->SetTexture(0, NULL);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &matrix);
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_cube->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_D3DDevice->SetTransform( D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void DexGameEngine::RenderCube(const DexMatrix4x4& matrix)
{
	DEX_ENSURE(g_cube);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_D3DDevice->SetTexture(0, NULL);
	//g_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_D3DDevice->SetTransform(D3DTS_WORLD, (const D3DXMATRIX*)(&matrix));
	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_cube->DrawSubset(0);
	D3DXMatrixIdentity(&g_worldMatrix);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &g_worldMatrix);
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}


void DexGameEngine::RenderSphere(const D3DXVECTOR3& pos, float scale /* = 1.0f */)
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

	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
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
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &g_worldMatrix);
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
int32 indices[] = {0,1,4,1,5,4,2,3,6,6,3,7,0,4,3,3,4,7,1,2,5,2,6,5,1,0,3,1,3,2,4,5,7,7,5,6};//逆时针索引
test_primitive_indice = (void*)malloc(sizeof(indices));
memcpy(test_primitive_indice, indices, sizeof(indices));

Render:
DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, test_primitive_vertex, 8, test_primitive_indice, 12, sizeof(stVertex0));
*/
void DexGameEngine::DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize)
{
	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
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
	D3DXMatrixIdentity(&g_worldMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertexs, sizeof(stVertex0));
	//这里的D3DFMT_INDEX32，如果indices是int32整型数组的话，设置D3DFMT_INDEX32,如果是int16类型的整型数组的话，那么设置D3DFMT_INDEX16
	g_D3DDevice->DrawIndexedPrimitiveUP(d3d_primitive_type, 0, vertexCount, primitiveCount, indices, D3DFMT_INDEX32, vertexs, stridesize);

	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
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
	static int64 lastTickTime = 0;
	if(lastTickTime == 0)
		lastTickTime = g_iCurrentTime;
	int delta = g_iCurrentTime - lastTickTime;
	lastTickTime = g_iCurrentTime;

	CInputSystem::GetInputSingleton().Update(g_hWnd);
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
	g_pCurrState->BeginRender();
	g_pCurrState->Render();
	g_pCurrState->EndRender();
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
// 		getLog()->BeginLog();
// 		getLog()->Log(log_allert, "Dex调用了一个不支持的render state命令!");
// 		getLog()->EndLog();
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


CCamera* DexGameEngine::getCamera()
{
	return g_camera;
}

bool DexGameEngine::DoLuaFile(const char* filename)
{
	getLog()->BeginLog();
	if(int error = luaL_dofile(L, filename) != 0)   //打开文件并编译
	{	
		getLog()->Log(log_allert, "DoLuaFile: %s error!\n", filename);
		return false;
	}
	getLog()->Log(log_ok, "DoLuaFile: %s ok!\n", filename);
	getLog()->EndLog();
	return true;
}

bool DexGameEngine::DoLuaString(const char* str)
{
	getLog()->BeginLog();
	if(int error = luaL_dostring(L, str) != 0)   //執行一行
	{	
		getLog()->Log(log_allert, "DoLuaString: %s error!\n", str);
		return false;
	}
	getLog()->Log(log_ok, "DoLuaString: %s ok!\n", str);
	getLog()->EndLog();
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
	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &g_projection);
}

void DexGameEngine::LookAtLH(const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)
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

void DexGameEngine::SetCameraPos(const D3DXVECTOR3& pos)
{
	g_camera->SetPosition(pos);
	updateViewMatrix();
}
void DexGameEngine::SetCameraLookAt(const D3DXVECTOR3& lookAt)
{
	g_camera->SetFocus(lookAt);
	updateViewMatrix();
}
void DexGameEngine::MoveCamera(D3DXVECTOR3 vec, float amount, bool move_focus /* = false */)
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
	ray.m_origin = D3DXVECTOR3(0,0,0);
	ray.m_vector = D3DXVECTOR3(px,py,1);

	D3DXMATRIX oppo_view;
	//矩阵求逆
	D3DXMatrixInverse(&oppo_view, 0, &g_ViewMatrix);

	D3DXVec3TransformCoord(&ray.m_origin, &ray.m_origin, &oppo_view);
	D3DXVec3TransformNormal(&ray.m_vector, &ray.m_vector, &oppo_view);

	D3DXVec3Normalize(&ray.m_vector, &ray.m_vector);
}
DexGUI::DexPoint  DexGameEngine::GetXY(const D3DXVECTOR3& world_point)
{
	DexGUI::DexPoint p;
	stRay ray;
	ray.m_origin = g_camera->GetPosition();
	ray.m_vector = ray.m_origin - world_point;  
	D3DXVec3Normalize(&ray.m_vector, &ray.m_vector);

	D3DXMATRIX oppo_view;
	//矩阵求逆
	D3DXMatrixInverse(&oppo_view, 0, &g_ViewMatrix);

	//D3DXVec3TransformCoord(&ray.m_origin, &ray.m_origin, &oppo_view);
	//D3DXVec3TransformNormal(&ray.m_vector, &ray.m_vector, &oppo_view);

	D3DXVec3TransformCoord(&ray.m_origin, &ray.m_origin, &g_ViewMatrix);
	D3DXVec3TransformNormal(&ray.m_vector, &ray.m_vector, &g_ViewMatrix);
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
	D3DXMatrixLookAtLH(&g_ViewMatrix, &g_camera->GetPosition(), &g_camera->GetFocusPoint(), &g_camera->GetUpVector());
	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);
}