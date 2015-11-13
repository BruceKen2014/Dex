
#include "DemoEx.h"
#include "../state/DexGameEngine.h"

#include "resource2.h"


LPD3DXMESH g_model = NULL;

CMyFont    *g_Font = NULL;
//CMouse     *g_mouse = NULL;
CFade         *g_pFade = NULL;


CBillboard*    g_billboard = NULL;

std::vector<TalkStruct*> g_talk;
size_t		  g_talkcontrol = 0;
CSpeak*       g_speakFrame= NULL;


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KEYUP:
		break;
	case WM_CHAR:
		{
			getDesktop()->OnKeyChar(char(wParam));
			break;
		}
	case WM_IME_SETCONTEXT:
		{
			int i = 0;
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevhInst, LPSTR cmdLine, int show)
{
	// Register the window class
	HandleCommandArgu(cmdLine);
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		WINDOW_CLASS, NULL };
	wc.hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hwnd;  
	bool fullscreen = FULLSCREEN;
	DWORD style;
	if(fullscreen)
		style = WS_POPUP;//popup会创造一个无边框的窗口 在全屏要这样做 否则鼠标坐标会有变差
	else
		style = WS_OVERLAPPEDWINDOW;
	hwnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, style ,
		WINDOW_POSITION_X, WINDOW_POSITION_Y, WINDOW_WIDTH, WINDOW_HEIGHT, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);		

	m_hWndPropDlg = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, 
		(DLGPROC) PropDlgProc); 

	//ShowWindow(m_hWndPropDlg, SW_SHOW);

	g_LoadThread = CreateThread(NULL, 0, LoadThread1, NULL, 0, &g_LoadThreadId);

	g_hwnd = hwnd;
	g_hInstance = hInst;
	if(InitializeOnce(hwnd, fullscreen))
	{
		// Enter the message loop
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
			UpdateInput();
			GameRender();
		}
	}

	// Release any and all resources.
	Shutdown();

	// Unregister our window.
	UnregisterClass(WINDOW_CLASS, wc.hInstance);
	return 0;
}


bool InitializeOnce(HWND hWnd, bool fullscreen)
{
	getLog()->EnableFontFrontHighLight();
	getLog()->EnableBackColor(true);
	getLog()->SetConsoleScreenSize(256, 1000);	   
	
	srand(unsigned(time(0)));
	DEX_ENSURE_B(InitializeD3D(g_hwnd, FULLSCREEN));
	DEX_ENSURE_B(IniEngineData());	 

	D3DXVECTOR3 cameraPos(g_camera.m_pos.x, g_camera.m_pos.y,
		g_camera.m_pos.z);
	D3DXVECTOR3 lookAtPos(200, 0, 0);	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);	
	// Build view matrix.
	D3DXMatrixLookAtLH(&g_ViewMatrix, &cameraPos,
		&lookAtPos, &upDir);
	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);	



	g_camera.SetCamera(0.0f, 100.0f, -200.0f,
		200.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	//g_camera.SetCamera(-300.0f, 640.0f, -1300.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f);
	g_camera.SetProject(PROJECT_FOV, PROJECT_ASPECT, PROJECT_NEAR, PROJECT_FAR);

	//g_camera.SetDestPos(-300, 200, 300);
	//g_camera.EnableMove(true);
	//g_camera.SetMoveVel(2.0f);


	D3DCAPS9 caps;
	g_D3DDevice->GetDeviceCaps(&caps);

	// Start and end distance of the fog.
	float start = 1, _end = 5000;

	// Set fog properties.
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, false);
	g_D3DDevice->SetRenderState(D3DRS_FOGCOLOR,
		D3DCOLOR_XRGB(128, 128, 128));
	//g_D3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	g_D3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP);
	g_D3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	g_D3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&_end));
	//设置雾化浓度
	float density = 0.002;
	g_D3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&density));

	//基于像素雾化
	g_D3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

	// Can only use if hardware supports it.
	if(caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE)
		g_D3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, true);


	//getDesktop()->m_bRenderRect = true;
	g_D3DDevice->SetRenderState( D3DRS_AMBIENT,  0x00FF0000 ); 
	g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//g_D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//g_D3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	g_D3DDevice->SetSamplerState(0,   D3DSAMP_MIPFILTER,   D3DTEXF_LINEAR);


	g_Font = new CMyFont(g_D3DDevice);
	g_Font->SetTextSize(15);

	g_Jintian = new CEnemy(g_D3DDevice);
	if(FAILED(g_Jintian->LoadXFile("model\\jingtian.x")))
	{
		_Message("加载X动画模型失败!");
		return false;
	}
	g_Jintian->SetIsPlayer(true);
	g_Jintian->SetPosition(200,0,0);
	g_Jintian->SetRenderSphere(false);
	g_Jintian->IniDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	g_Jintian->SetDirection(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
	//g_Jintian->SetVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	g_Jintian->SetAniSpeed(25);
	//g_Jintian->SetScale(2.0f);
	g_Jintian->SetAnimation(JINGTIAN_STAND);
	g_Jintian->SetScale(0.5);
	g_Jintian->SetBallScale(0.5f);
	g_Jintian->SetVel(0.0f);
	g_Jintian->SetMoving(true);

	EnmyPara para;
	para.m_time = 50;
	para.m_standPos = D3DXVECTOR3(3169, 0, 2194);

	g_Jintian->SetAnimation(JINGTIAN_STAND);
	g_Jintian->LoadIni("ini\\fight.ini", "jingtian");
	para.m_standIndex = JINGTIAN_STAND; 
	para.m_runIndex = JINTTIAN_RUN;
	para.m_attackIndex.push_back(JINGTIAN_ATTACK);

	g_Jintian->SetEnmyPara(para);


	g_pSky = new CSkyBox();
	g_pSky->Initialize(g_D3DDevice);

	L = lua_open();	   //创建接口指针
	luaopen_base(L);   //加载基本库
	luaL_openlibs(L);  //加载扩展库

	LuaRegisterFun(L); 

	g_isTownNpc = true;  
	if(!DoLuaFile(PAL_CREATE_TOWN))
		return false;	   
	g_isTownNpc = false;
	if(!DoLuaFile(PAL_CREATE_OUTDOOR))
		return false;	


	g_palState = PAL_LOADING;	
	g_iLoadRate = 30;


	g_iLoadRate = 31;
	g_strLoadDes = "正在加载UI资源……";
	CDexUiTexFactory::AddFilePath("res/");
	CDexUiTexFactory::AddFilePath("res/button/");
	getUiSrcMgrSingleton()->createUiTexFactory(ui_src_state_interface, "ini/ui_tex1.ini");
	getUiSrcMgrSingleton()->setCurrState(ui_src_state_interface);

	g_iLoadRate = 50;
	g_strLoadDes = "正在加载光标资源……";
	g_Cursors[0] = LoadCursorFromFile("res/Cursor.cur");
	g_Cursors[1] = LoadCursorFromFile("res/cs_duihua.ani");
	g_Cursors[2] = LoadCursorFromFile("res/cs_gongji.ani");
	SetCursor(g_Cursors[0]);



	g_iLoadRate = 55;
	g_strLoadDes = "初始化拾取信息……";
	g_pPick = new CPick();   
	g_pPick->Initial(g_D3DDevice);



	//if(int error = luaL_dofile(L, "script\\fight.lua") != 0)   //打开文件并编译
	//{
	//	_Message("初始化脚本fight.lua出错！");
	//	return false;
	//}

	g_iLoadRate = 60;
	g_strLoadDes = "初始化自主脚本信息……";
	g_pScriptMgr = new CDexScriptMgr;			
	//注意 浮点数必须加小数点 否则会被解析为整数
	//如果是负数如 -100.0 则必须写成 0.0-100.0的形式
	//g_pScriptMgr->AddCommand("MoveCamera(200.0,100.0,100.0,0.5)");	
	//g_pScriptMgr->AddCommand("Wait(15000)");
	//g_pScriptMgr->AddCommand("MoveCamera(500.0,200.0,200.0,0.5)");
	//g_pScriptMgr->AddCommand("Wait(15000)");
	//g_pScriptMgr->AddCommand("MoveCamera(0.0,0.0,0.0,0.5)");
	//g_pScriptMgr->AddCommand("Wait(15000)");

	InitScriptExpression();
	//if(g_palState == PAL_INTERFACE)
	//{
	//	CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_WENQING);
	//	if(!ApplyInterface())
	//		return false;
	//}
	//if(g_palState == PAL_TOWN)
	//{
	//	if(!ApplyTown()) 
	//		return false;
	//}
	//else if(g_palState == PAL_OUTDOORS)
	//{
	//	if(!ApplyOutdoors()) 
	//		return false;
	//}

	g_State2 = PAL2_FLAG;
	g_clickId = 0;

	g_iLoadRate = 70;
	g_strLoadDes = "初始化特效粒子……";
	CSound::getSoundSingleton();
	ApplyTown();

	g_iLoadRate = 90;
	g_strLoadDes = "初始化界面……";
	g_testPanel = new CTestPanel();
	g_testPanel->Init();

	g_palState = FirstState;	


	return true;
}

void UpdateInput()
{  
	CInputSystem::GetInputSingleton().Update(g_hwnd);
	g_iXMouse = (int)CInputSystem::GetInputSingleton().GetMouseXPos();
	g_iYMouse = (int)CInputSystem::GetInputSingleton().GetMouseYPos(); 
	MouseMove(g_iXMouse, g_iYMouse, MF_MouseMove);
	if(CInputSystem::GetInputSingleton().LeftButtonDown())
	{
		MouseLDown(g_iXMouse, g_iYMouse, MF_MouseLDown);
	}
	if(CInputSystem::GetInputSingleton().LeftButtonUp())
		MouseLUp(g_iXMouse, g_iYMouse, MF_MouseLUp);
	if(CInputSystem::GetInputSingleton().RightButtonUp())
		MouseRUp(g_iXMouse, g_iYMouse, MF_MouseRUp);
	KeyUp();
	KeyDown();
}
void GameRender()
{
	GetFps(); 
	//D3DVIEWPORT9 viewport = {viewX, viewY, 800, 600, 0.0f, 1.0f};
	//g_D3DDevice->SetViewport(&viewport);
	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
	g_D3DDevice->BeginScene();
	D3DXMatrixPerspectiveFovLH(&g_projection, PROJECT_FOV,
		PROJECT_ASPECT, PROJECT_NEAR, PROJECT_FAR);
	g_D3DDevice->SetTransform(D3DTS_PROJECTION, &g_projection);

	
	int64 tempTime = getTime()->GetTotalMillSeconds() ;
	if(g_lastTickTime == 0)
		g_lastTickTime = tempTime;
	g_delta = tempTime - g_lastTickTime;
	g_camera.Update(g_delta);
	g_lastTickTime = tempTime;
	D3DXVECTOR3 cameraPos(g_camera.m_pos.x, g_camera.m_pos.y,
		g_camera.m_pos.z);
	D3DXVECTOR3 lookAtPos(g_camera.m_focusPoint.x, g_camera.m_focusPoint.y,
		g_camera.m_focusPoint.z);	 
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);

	// Build view matrix.
	D3DXMatrixLookAtLH(&g_ViewMatrix, &cameraPos,
		&lookAtPos, &upDir);

	g_camera.CalFrustum();  

	g_D3DDevice->SetTransform(D3DTS_VIEW, &g_ViewMatrix);
	switch(g_palState)
	{
	case PAL_LOADING:
		{
			//RenderLoading();
			g_Font->SetPos(200, 200);
			g_Font->Print("加载进度：%d%%", g_iLoadRate);
			g_Font->SetPos(200, 230);
			g_Font->Print("加载说明：%s", g_strLoadDes.c_str());
			break;
		}
	case PAL_INTERFACE:
		{
			RenderInterFace();
			break;
		}
	case PAL_INTRODUCE:
		{
			RenderIntroduce();
			break;
		}
	case PAL_TOWN:
		{
			RenderTown();
			RenderLoading();
			break;
		}
	case PAL_OUTDOORS:
		{
			RenderOutdoors();
			break;
		}
	case PAL_FIGHT:
		{
			//RenderOutdoors(); //暂时将野外场景作为战斗背景
			RenderFight();
			break;
		}
	default:
		break;
	}	
#ifdef DEX_DEBGU
	g_Font->SetPos(0, 0);
	g_Font->Print("fps:%d, 鼠标X位置%d Y位置%d", g_fps, g_iXMouse, g_iYMouse);

	g_Font->SetPos(0, 50);
	//g_Font->Print("设备中 鼠标X位置%ld Y位置%ld", CInputSystem::GetInputSingleton().GetMouseXDPos(), CInputSystem::GetInputSingleton().GetMouseXDPos());
	g_Font->SetPos(400, 0);
	//g_Font->Print("摄像机X:%d, Y:%d Z:%d", (int)g_camera.m_pos.x, (int)g_camera.m_pos.y, (int)g_camera.m_pos.z);
#endif
	g_D3DDevice->EndScene();
	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
}


void Shutdown()
{
	if(g_D3DDevice != NULL) g_D3DDevice->Release();
	if(g_D3D != NULL) g_D3D->Release();
}
void MouseMove(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	switch(g_palState)
	{
	case PAL_INTERFACE:
		{
			MouseMoveInterface(xPos, yPos, flag);
			break;
		}
	case PAL_INTRODUCE:
		{
			MouseMoveIntroduce(xPos, yPos, flag);
			break;
		}
	case PAL_TOWN:
		{
			MouseMoveTown(xPos, yPos, flag);
			break;
		}
	case PAL_OUTDOORS:
		{
			MouseMoveOutdoors(xPos, yPos, flag);
			break;
		}
	case PAL_FIGHT:
		{
			MouseMoveFight(xPos, yPos, flag);
			break;
		}
	default:
		break;
	}
}

void MouseLDown(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	switch(g_palState)
	{
	case PAL_INTERFACE:
		{
			MouseLDownInterface(xPos, yPos, flag);
			break;
		}
	case PAL_INTRODUCE:
		{
			MouseLDownIntroduce(xPos, yPos, flag);
			break;
		}
	case PAL_TOWN:
		{
			MouseLDownTown(xPos, yPos, flag);
			break;
		}
	case PAL_OUTDOORS:
		{
			MouseLDownOutdoors(xPos, yPos, flag);
			break;
		}
	case PAL_FIGHT:
		{
			MouseLDowhFight(xPos, yPos, flag);
			break;
		}
	default:
		break;
	}
}

void MouseLUp(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{    
	switch(g_palState)
	{
	case PAL_INTERFACE:
		{
			MouseLUpInterface(xPos, yPos, flag);
			break;
		}
	case PAL_INTRODUCE:
		{
			MouseLUpIntroduce(xPos, yPos, flag);
			break;
		}
	case PAL_TOWN:
		{
			MouseLUpTown(xPos, yPos, flag);
			break;
		}
	case PAL_OUTDOORS:
		{
			MouseLUpOutdoors(xPos, yPos, flag);
			break;
		}
	case PAL_FIGHT:
		{
			MouseLUpFight(xPos, yPos, flag);
			break;
		}
	default:
		break;
	}
}

void MouseRUp(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{    
	switch(g_palState)
	{
	case PAL_INTERFACE:
		{
			MouseRUpInterface(xPos, yPos, flag);
			break;
		}
	case PAL_TOWN:
		{
			MouseRUpTown(xPos, yPos, flag);
			break;
		}
	case PAL_OUTDOORS:
		{
			MouseRUpOutdoors(xPos, yPos, flag);
			break;
		}
	case PAL_FIGHT:
		{
			MouseRUpFight(xPos, yPos, flag);
			break;
		}
	default:
		break;
	}
}


void KeyUp()
{
	switch(g_palState)
	{
	case PAL_TOWN:
		{
			KeyUpTown();
			break;
		}
	case PAL_OUTDOORS:
		{
			KeyUpOutdoors();
			break;
		}
	case PAL_FIGHT:
		{
			KeyUpFight();
			break;
		}
	default:
		break;
	}
}
void KeyDown()
{
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_LEFT))
	{	
		viewX -= 2;
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_RIGHT))
	{	
		viewX += 2;
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_UP))
	{	
		viewY -= 2;
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_DOWN))
	{	
		viewY += 2;
	}
	switch(g_palState)
	{
	case PAL_TOWN:
		{
			KeyDownTown();
			break;
		}
	case PAL_OUTDOORS:
		{
			KeyDownOutdoors();
			break;
		}
	case PAL_FIGHT:
		{
			KeyDownFight();
			break;
		}
	default:
		break;
	}
}

bool InitializeD3D(HWND hWnd, bool fullscreen)
{
	D3DDISPLAYMODE displayMode;

	// Create the D3D object.
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_D3D == NULL) return false;

	// Get the desktop display mode.
	if(FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));


	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;

	if(g_D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, displayMode.Format, !fullscreen,
		D3DMULTISAMPLE_8_SAMPLES,
		NULL) == D3D_OK) 
		multiType = D3DMULTISAMPLE_8_SAMPLES;

	if(fullscreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
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
	if(FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &g_D3DDevice)))
	{
		
		getLog()->Log(log_ok, "D3D CreateDevice失败！");
		getLog()->EndLog();
		return false;
	}
	getLog()->Log(log_ok, "D3D CreateDevice成功！");
	getLog()->EndLog();
	DexGameEngine::getEngine()->SetDevice(g_D3DDevice);
	DexGameEngine::getEngine()->SetHwnd(g_hwnd);
	DexGameEngine::getEngine()->SetHInstance(g_hInstance);
	return true;
}
bool ApplyIntroduce()
{
	g_pGuiInterface = new CGuiSystem(g_D3DDevice);
	g_pGuiInterface->LoadFont(g_D3DDevice, PAL_FONT);
	g_pGuiInterface->LoadGui(g_D3DDevice, PAL_INTRODUCE_GUI);
	return true;
}
bool ApplyInterface()
{
	g_pGuiInterface = new CGuiSystem(g_D3DDevice);
	g_pGuiInterface->LoadFont(g_D3DDevice, PAL_FONT);
	g_pGuiInterface->LoadGui(g_D3DDevice, PAL_INTERFACE_GUI);
	g_pGuiInterface->SetUniqueId(GUI_ID_INFOBACKTOWER);

	btn = new CButtonGui(g_D3DDevice, "res\\button\\ok.png");
	btn->SetPosition(200, 200);
	btn->m_MouseLUp += UI_DELEGATE_G(InterfaceUiOkButton);

	return true;
}
bool ApplyTown()
{
	g_sceneNode = new CDexSceneObject;
	particalMgr = new CDexParticalEmit;
	particalMgr->m_eParticalFaceState = EPFS_CAM;
	particalMgr->m_createMaxSize = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	particalMgr->m_createMinSize = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	particalMgr->m_createPosMin = D3DXVECTOR3(0, 0, 0);
	particalMgr->m_createPosMax = D3DXVECTOR3(0, 0, 0);
	particalMgr->m_iParticalLiveTime = 1000;
	particalMgr->m_iCreateLastParticalTime = 0;
	particalMgr->m_createInterval = 10;
	particalMgr->m_velMin = D3DXVECTOR3(-10, -10, -10);
	particalMgr->m_velMax = D3DXVECTOR3(10, 10, 10);
	particalMgr->m_acc = D3DXVECTOR3(0, 0, 0);	
	particalMgr->m_fMiddleTimeRate = 0.5f;
	particalMgr->m_fScaleStart = 1.0f;
	particalMgr->m_fScaleMiddle = 0.5f;
	particalMgr->m_fScaleEnd = 0.1f;
	particalMgr->m_startColor = DexColor(1.0f, 1.0f, 1.0f,1.0f);
	particalMgr->m_middleColor = DexColor(1.0f, 1.0f, 0.0f,0.5f);
	particalMgr->m_endColor = DexColor(1.0f, 0.0f, 0.0f, 0.0f);


	//particalMgr->m_rotateVelMax = D3DXVECTOR3(0, 0, 90*3);
	particalMgr->SetTexFile("e18.png");	  
	particalMgr->PreInitMemRes();

	DexSceneObjectActionCatmull* action2 = new DexSceneObjectActionCatmull;
	D3DXMATRIX localTransMatrix;
	D3DXMATRIX localMatrix;
	D3DXMATRIX localRotateMatrix;
	D3DXMatrixIdentity(&localTransMatrix);
	D3DXMatrixIdentity(&localRotateMatrix);
	D3DXMatrixIdentity(&localMatrix);
	D3DXMatrixRotationX(&localRotateMatrix, _getRadian(45.0f));
	D3DXMatrixTranslation(&localTransMatrix, 0, 0, 0);
	D3DXMatrixMultiply(&localMatrix, &localRotateMatrix, &localTransMatrix);
	float circle2 = 50.0f;
	action2->addCatmullPoint(D3DXVECTOR3(circle2,0,0));
	action2->addCatmullPoint(D3DXVECTOR3(0,0,circle2));
	action2->addCatmullPoint(D3DXVECTOR3(-circle2,0,0));
	action2->addCatmullPoint(D3DXVECTOR3(0,0,-circle2));
	action2->addCatmullPoint(D3DXVECTOR3(circle2,0,0));
	action2->addCatmullPoint(D3DXVECTOR3(0,0,circle2));
	action2->addCatmullPoint(D3DXVECTOR3(-circle2,0,0));
	action2->setCatmullSpeed(3.0f);
	particalMgr->addAction(action2);
	particalMgr->setLocalMatrix(localMatrix);
	DexCollideObjectSphere* ball2 = new DexCollideObjectSphere(D3DXVECTOR3(0,0,0), 3.0f);
	particalMgr->setCollideObject(ball2);

	//particalMgr->bindSceneNode(g_sceneNode);

	particalMgr->m_EmitType = EET_FRAME_NORMAL;
	particalMgr->AddKeyFrameDur(0, 120);

	g_pEmittInstance = new CDexEffectEmitInstance;
	//g_pEmittInstance->AddEmitter(particalMgr);
	g_pEmittInstance->SetTotalFrames(180);


	CDexParticalEmit* particalMgr2 = new CDexParticalEmit;
	particalMgr2->m_eParticalFaceState = EPFS_CAM_Y;
	particalMgr2->m_EmitType = EET_FRAME_NORMAL;
	particalMgr2->AddKeyFrameDur(50, 119);
	particalMgr2->m_createMaxSize = D3DXVECTOR3(20.0f, 50.0f, 0.0f);
	particalMgr2->m_createMinSize = D3DXVECTOR3(60.0f, 200.0f, 0.0f);
	particalMgr2->m_createPosMin = D3DXVECTOR3(-10, 50, 0);
	particalMgr2->m_createPosMax = D3DXVECTOR3(10, 50, 0);
	particalMgr2->m_iParticalLiveTime = 500;
	particalMgr2->m_iCreateLastParticalTime = 0;
	particalMgr2->m_createInterval = 20;
	particalMgr2->m_velMin = D3DXVECTOR3(0, 0, 0);
	particalMgr2->m_velMax = D3DXVECTOR3(0, 0, 0);
	particalMgr2->m_acc = D3DXVECTOR3(0, 0, 0);	
	particalMgr2->m_fMiddleTimeRate = 0.5f;
	particalMgr2->m_fScaleStart = 1.0f;
	particalMgr2->m_fScaleMiddle = 1.0f;
	particalMgr2->m_fScaleEnd = 1.0f;
	particalMgr2->m_startColor = DexColor(1.0f, 0.0f, 0.0f,0.5f);
	particalMgr2->m_middleColor = DexColor(0.0f, 1.0f, 0.0f,1.0f);
	particalMgr2->m_endColor = DexColor(0.0f, 0.0f, 1.0f, 0.0f);
	particalMgr2->SetTexFile("splash.png");
	particalMgr2->m_iTexFrameX = 4;
	particalMgr2->m_iTexFrameTotal = 4;
	particalMgr2->PreInitMemRes();
	particalMgr2->bindSceneNode(g_sceneNode);
	//particalMgr2->m_rotateInit = D3DXVECTOR3(0, 90, 0);


	CDexParticalEmit* particalMgr3 = new CDexParticalEmit;
	particalMgr3->m_eParticalFaceState = EPFS_CAM;	 
	particalMgr3->m_EmitType = EET_FRAME_NORMAL;
	particalMgr3->AddKeyFrameDur(0, 59);
	particalMgr3->m_createMaxSize = D3DXVECTOR3(40.0f, 40.0f, 0.0f);
	particalMgr3->m_createMinSize = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
	particalMgr3->m_createPosMin = D3DXVECTOR3(0, 150, 0);
	particalMgr3->m_createPosMax = D3DXVECTOR3(0, 150, 0);
	particalMgr3->m_iParticalLiveTime = 500;
	particalMgr3->m_iCreateLastParticalTime = 0;
	particalMgr3->m_createInterval = 100;
	particalMgr3->m_velMin = D3DXVECTOR3(0, 0, 0);
	particalMgr3->m_velMax = D3DXVECTOR3(0, 0, 0);
	particalMgr3->m_acc = D3DXVECTOR3(0, 0, 0);	
	particalMgr3->m_fMiddleTimeRate = 0.5f;
	particalMgr3->m_fScaleStart = 1.0f;
	particalMgr3->m_fScaleMiddle = 1.0f;
	particalMgr3->m_fScaleEnd = 1.0f;
	particalMgr3->m_startColor = DexColor(1.0f, 1.0f, 1.0f,1.0f);
	particalMgr3->m_middleColor = DexColor(1.0f, 1.0f, 1.0f,1.0f);
	particalMgr3->m_endColor = DexColor(1.0f, 1.0f, 1.0f, 0);
	particalMgr3->SetTexFile("tt1.png");
	particalMgr3->m_iTexFrameX = 4;
	particalMgr3->m_iTexFrameTotal = 16;
	particalMgr3->PreInitMemRes();
	particalMgr3->bindSceneNode(g_sceneNode);

	//CDexParticalEmit* particalMgr5 = new CDexParticalEmit;

	//particalMgr3->Archive(DexGameEngine::getEngine()->getMem(), DexMem::MEM_WRITE);
	//particalMgr5->Archive(DexGameEngine::getEngine()->getMem(), DexMem::MEM_READ);

	//particalMgr5->bindSceneNode(g_sceneNode);
	g_pEmittInstance->AddEmitter(particalMgr3);
	g_pEmittInstance->AddEmitter(particalMgr2);

	DexGameEngine::getEngine()->getMem().BeginWrite();
	g_pEmittInstance->Archive(DexGameEngine::getEngine()->getMem(),  DexMem::MEM_WRITE);
	
	DexGameEngine::getEngine()->getMem().SaveToFile("effect/effect.eft");

	delete g_pEmittInstance;
	g_pEmittInstance = new CDexEffectEmitInstance;

	DexGameEngine::getEngine()->getMem().Reset();
	DexGameEngine::getEngine()->getMem().IniFromFile("effect/effect.eft");
	DexGameEngine::getEngine()->getMem().BeginRead();
	g_pEmittInstance->SetTotalFrames(180);
	g_pEmittInstance->Archive(DexGameEngine::getEngine()->getMem(),  DexMem::MEM_READ);
	
	g_pEmittInstance->bindSceneNode(g_sceneNode);

	g_Ex = new CTextureEx();
	g_Ex->Load(g_D3DDevice, "res/FightSelectBack.png");
	g_pGuiTown = new CGuiSystem(g_D3DDevice);
	g_pGuiTown->LoadFont(g_D3DDevice, PAL_FONT);
	g_pGuiTown->LoadGui(g_D3DDevice, PAL_GUI_INI_TOWN);
	g_FileTex = new CDexTex();
	g_FileTex->LoadTex("res\\3.png", DexColor(0,0,0));
	g_FireBill = new CBillBoardAni(g_D3DDevice);
	g_FireBill->SetFrame(16, 4);
	_ADDREF(g_FileTex);
	g_FireBill->SetTex(g_FileTex);
	g_FireBill->CreateVertex();
	g_FireBill->SetPos(200,0,0);
	g_FireBill->SetSize(40, 30);
	g_FireBill->SetSpeed(5);
	g_FireBill->SetDir(1,0,0);
	g_FireBill->SetChangeByCamera(true);

	g_water = new CWaterF();
	g_water->Init(g_D3DDevice, "res\\water\\1.bmp", 128*4, 128*4);
	g_water->SetPos(D3DXVECTOR3(200, 10, 200));
	g_water->AddTexture("res\\water\\2.bmp");
	g_water->AddTexture("res\\water\\3.bmp");
	g_water->AddTexture("res\\water\\4.bmp");
	g_water->AddTexture("res\\water\\5.bmp");
	g_water->AddTexture("res\\water\\6.bmp");
	g_water->AddTexture("res\\water\\7.bmp");
	g_water->AddTexture("res\\water\\8.bmp");
	g_water->AddTexture("res\\water\\9.bmp");
	g_water->AddTexture("res\\water\\10.bmp");
	g_water->AddTexture("res\\water\\11.bmp");
	g_water->AddTexture("res\\water\\12.bmp");
	g_water->AddTexture("res\\water\\13.bmp");
	g_water->AddTexture("res\\water\\14.bmp");
	g_water->AddTexture("res\\water\\15.bmp");
	g_water->AddTexture("res\\water\\16.bmp");
	g_water->AddTexture("res\\water\\17.bmp");
	g_water->AddTexture("res\\water\\18.bmp");
	g_water->AddTexture("res\\water\\19.bmp");
	g_water->AddTexture("res\\water\\20.bmp");
	g_water->AddTexture("res\\water\\21.bmp");
	g_water->AddTexture("res\\water\\22.bmp");
	g_water->AddTexture("res\\water\\23.bmp");
	g_water->AddTexture("res\\water\\24.bmp");
	g_water->AddTexture("res\\water\\25.bmp");
	g_water->AddTexture("res\\water\\26.bmp");
	g_water->AddTexture("res\\water\\27.bmp");
	g_water->AddTexture("res\\water\\28.bmp");
	g_water->AddTexture("res\\water\\29.bmp");


	//CMusicMidi::getMidiSingleton().Play("music\\3.mid");

	//CSound::getSoundSingleton().Play("sound\\1.wav", 0, true);
	//CSound::getSoundSingleton().Play("5.wav", 1);
	//g_pModle->SetPosition(-200, 0, 200)

	//g_pFade = new CFade();
	//g_pFade->Initialize(g_D3DDevice);
	////g_pFade->SetFadeOut();
	//g_pFade->SetRange(800, 600);

	//g_pGround = new CGround();
	//g_pGround->Initialize(g_D3DDevice, D3DXVECTOR3(-4000, 0, -4000), D3DXVECTOR2(8000, 8000));
	//g_pGround->SetTexture("res\\map\\grass.jpg");



	g_billboard = new CBillboard(g_D3DDevice);
	g_billboard->CreateVertex();
	g_billboard->LoadTexture("res\\sixiangfangqu.png");
	//g_billboard->SetScale(50.0f);
	g_billboard->SetSize(100.0f);
	g_billboard->SetPos(0,0, 0);

	//for(std::map::iterator it = g_headmap.begin(); it!= g_headmap.end(); it++)
	//{ 
	//	(*it) = NULL;
	//	(*it) = new CTextureEx();
	//}
	g_headmap[SPEAK_JINGTIAN_NORMAL] = new CTextureEx();
	g_headmap[SPEAK_JINGTIAN_NORMAL]->Load(g_D3DDevice, HEAD_JINGTIAN);
	g_headmap[SPEAK_JINGTIAN_ANGRY] = new CTextureEx();
	g_headmap[SPEAK_JINGTIAN_ANGRY]->Load(g_D3DDevice, HEAD_JINGTIAN_ANGRY);
	g_headmap[SPEAK_JINGJING_NORAML] = new CTextureEx();
	g_headmap[SPEAK_JINGJING_NORAML]->Load(g_D3DDevice, HEAD_JINGJING);
	g_headmap[SPEAK_DRAGON] = new CTextureEx();
	g_headmap[SPEAK_DRAGON]->Load(g_D3DDevice, HEAD_DRAGON);
	g_headmap[SPEAK_JINGTIAN_JING] = new CTextureEx();
	g_headmap[SPEAK_JINGTIAN_JING]->Load(g_D3DDevice, HEAD_JINGTIAN_JING);
	g_headmap[SPEAK_SHOUZI] = new CTextureEx();
	g_headmap[SPEAK_SHOUZI]->Load(g_D3DDevice, HEAD_SHOUZI);


	g_speakFrame = new CSpeak(g_D3DDevice);
	g_speakFrame->LoadBackPic(LEFT, "res\\speakFrame\\left.png");
	g_speakFrame->LoadBackPic(RIGHT, "res\\speakFrame\\right.png");
	g_speakFrame->LoadBackPic(BOTTOM, "res\\speakFrame\\bottom.png");
	g_speakFrame->LoadBackPic(CENTER, "res\\speakFrame\\center.png");
	g_speakFrame->LoadBackPic(TOP, "res\\speakFrame\\top.png");
	g_speakFrame->LoadBackPic(TOP_LEFT, "res\\speakFrame\\top_left.png");
	g_speakFrame->LoadBackPic(TOP_RIGHT, "res\\speakFrame\\top_right.png");
	g_speakFrame->LoadBackPic(BOTTOM_LEFT, "res\\speakFrame\\bottom_left.png");
	g_speakFrame->LoadBackPic(BOTTOM_RIGHT, "res\\speakFrame\\bottom_right.png");
	//g_speakFrame->SetHead(g_headJingtian);
	g_speakFrame->SetStartPos(10, 400);
	g_speakFrame->SetSize(760, 150);
	//g_speakFrame->SetWords("“也许并不是她没给我机会，只是我没抓住而已。”墨然醒来的时候，脑中反反复复回响着这句话，可是连他自己都不知自己的脑海中为什么会回响这句话。");
	g_speakFrame->SetWordsOffset(20, 20);

	//g_terrain = new CTerrain(g_D3DDevice);
	//g_terrain->CreateTerrain("res\\map\\ter.raw", 40, 40, 1.5f);
	//g_terrain->LoadTexture("res\\map\\texture.tga");
	//g_terrain->SetPos(D3DXVECTOR3(1300, 0, 0));



	//int hit = CIni::GetInt("ini\\fight.ini", "player", "hit");
	//CIni::SetInt("ini\\fight.ini", "player", "hit", 12);
	//hit = CIni::GetInt("ini\\fight.ini", "player", "hit");

	//float luck = CIni::GetFloat("ini\\fight.ini", "player", "luck");
	//CIni::SetFloat("ini\\fight.ini", "player", "luck", 7.5, 1);
	//luck = CIni::GetFloat("ini\\fight.ini", "player", "luck");

	//char skill[128];
	//CIni::GetString("ini\\fight.ini", "player", "skill", skill);
	//CIni::SetString("ini\\fight.ini", "player", "skill", "\"飞龙探云手：当年李三思传与猩猩，猩猩传与景天，后李逍遥从父亲的包袱中学得\"");
	//CIni::GetString("ini\\fight.ini", "player", "skill", skill);
	//CIni::Trans("script\\01.save");
	
	
	//if(g_task !=2)
	//	CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_TOWN);
	//else
	//	CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_TOWN_WIN);

	float panel_width = 1.0f;
	float panel_height = 1.0f;
	g_D3DDevice->CreateVertexBuffer(4*sizeof(stVertex1), D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1,D3DPOOL_DEFAULT, &g_vertexBuff, NULL);
	g_vertexBuff->Lock(0, 4*sizeof(stVertex1), (void**)&vertexs, 0);
	vertexs[0].m_color = 0xffff0000;
	vertexs[1].m_color = 0xffff0000;
	vertexs[2].m_color = 0xffff0000;
	vertexs[3].m_color = 0xffff0000;

	vertexs[0].m_pos.x = vertexs[3].m_pos.x = -panel_width/2;
	vertexs[1].m_pos.x = vertexs[2].m_pos.x =  panel_width/2;

	vertexs[0].m_pos.y = vertexs[1].m_pos.y = panel_height/2;
	vertexs[2].m_pos.y = vertexs[3].m_pos.y = -panel_height/2;

	vertexs[0].m_pos.z = vertexs[1].m_pos.z = vertexs[2].m_pos.z = vertexs[3].m_pos.z = 1.0f;

	vertexs[0].SetUV(0.0f, 0.0f);
	vertexs[1].SetUV(1.0f, 0.0f);
	vertexs[2].SetUV(1.0f, 1.0f);
	vertexs[3].SetUV(0.0f, 1.0f);
	g_vertexBuff->Unlock();

	g_2DSkill = new CSkillAddBlood(g_D3DDevice);
	g_2DSkill->SetPosition(g_Jintian->GetPosition());
	g_2DSkill->LoadIni("ini\\skill.ini", "AddBlood");
	if(!g_2DSkill->Initial())
	{
		_Message("failed!");
		_SafeDelete(g_2DSkill);
	}
	g_2DSkill->SetPosition(D3DXVECTOR3(0,0,0));
	return true;
}

bool ApplyOutdoors()
{
	g_terrain = new CTerrain(g_D3DDevice);
	g_Jintian->SetPositionY(g_terrain->GetHeight(g_Jintian->GetPosition()));
	if(!g_terrain->LoadIni(PAL_DATA_INI, "TerrainData"))
	{
		_Message("地图初始信息读取失败！");
		return false;
	}
	CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_OUTDOORS);
	g_OpenFog = true;
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, g_OpenFog);
	return true;
}

bool ApplyFight()						   
{
	g_WinTex = new CTextureEx();
	g_WinTex->Load(g_D3DDevice, "res\\win.png");
	g_playPos = g_Jintian->GetPosition();
	g_playDir = g_Jintian->GetDirection();

	g_cameraPos = g_camera.GetPosition();

	g_camera.SetPosition(3082,  30,  2319);
	g_camera.SetFocus(3206,   0,    2304);

	g_camera.EnableMove(true);
	g_camera.SetMoveVel(2.5);
	g_camera.SetDestPos(3051,   100,   2177);

	g_Jintian->SetPosition(3169,   0,   2194);
	g_Jintian->FaceTo(3206,   0,    2304);
	g_Jintian->SetAnimation(JINGTIAN_STAND);
	if(spell != NULL)
	{
		spell->SetPos(3169,   0,   2194);
	}
	//_SafeDelete(g_pGuiFight);   //防止之前没有释放，此处再次确认一下

	g_BloodLabel = new CBloodLabel(g_D3DDevice);
	if(!g_BloodLabel->LoadIni("ini\\skill.ini", "jing"))
		_Message("血条精初始化失败！");
	g_BloodLabel->SetNumber(g_Jintian->GetHp());
	g_BloodLabel->SetFont(g_Font);

	g_LoseTex = new CTextureMove(g_D3DDevice);
	if(!g_LoseTex->Load(g_D3DDevice, "res\\fail.png"))
		return false;
	g_LoseTex->SetStartPos(0, -600);
	g_LoseTex->SetEndPos(0, 0);
	g_LoseTex->SetSpeed(2);
	g_pGuiFight = new CGuiSystem(g_D3DDevice);
	g_pGuiFight->LoadFont(g_D3DDevice, PAL_FONT);
	g_pGuiFight->LoadGui(g_D3DDevice, PAL_FIGHT_GUI);

	g_pGuiFightInoBack = new CGuiSystem(g_D3DDevice);
	g_pGuiFightInoBack->LoadFont(g_D3DDevice, PAL_FONT);
	g_pGuiFightInoBack->LoadGui(g_D3DDevice, PAL_FIGHT_GUI_BACK);



	spell = new CSpell(g_D3DDevice);
	spell->SetPos(0,10,0);
	spell->SetSize(100, 100);
	spell->LoadTexture("res\\123123.png");
	spell->SetSpeed(0.01f);
	spell->CreateVB();

	g_FightLabel = new CFightLabel(g_D3DDevice);
	if(!g_FightLabel->LoadBackTexture("res\\fightBackLabel.png"))
	{
		_SafeDelete(g_FightLabel);
		return false;
	}
	if(!g_FightLabel->LoadPointHeadTex("res\\PointEnmyHead.png"))
	{
		_SafeDelete(g_FightLabel);
		return false;
	}

	g_FightLabel->SetPos(400, 20);
	g_FightLabel->SetLength(335);
	g_FightLabel->SetOffsetM(25, -10);
	g_FightLabel->SetOffset(25, 10);

	CFightHead* temphead = new CFightHead(g_D3DDevice);
	if(!temphead->LoadTexture("res\\headflag_jingtian.png"))
	{
		_SafeDelete(temphead);
		return false;
	}
	temphead->SetSpeed(3.0);
	temphead->SetIsPlayer(true);
	temphead->SetEnemy(g_Jintian);

	CFightHead* temphead2 = new CFightHead(g_D3DDevice);
	if(!temphead2->LoadTexture("res\\headflag_hunter.png"))
	{
		_SafeDelete(temphead2);
		return false;
	}
	temphead2->SetSpeed(1);
	temphead2->SetIsPlayer(false);
	temphead2->SetEnemy(g_enemyVector[0]);

	CFightHead* temphead3 = new CFightHead(g_D3DDevice);
	if(!temphead3->LoadTexture("res\\headflag_panda.png"))
	{
		_SafeDelete(temphead3);
		return false;
	}
	temphead3->SetSpeed(1.3);
	temphead3->SetIsPlayer(false);
	temphead3->SetEnemy(g_enemyVector[1]);


	CFightHead* temphead4 = new CFightHead(g_D3DDevice);
	if(!temphead4->LoadTexture("res\\headflag_bear.png"))
	{
		_SafeDelete(temphead4);
		return false;
	}
	temphead4->SetSpeed(1.9);
	temphead4->SetIsPlayer(false);
	temphead4->SetEnemy(g_enemyVector[2]);
	g_FightLabel->AddHead(temphead);
	g_FightLabel->AddHead(temphead2);
	g_FightLabel->AddHead(temphead3);
	g_FightLabel->AddHead(temphead4);

	g_FightStage = FIGHT_MOVE_CAMERA;

	//CModelXAni* tempEnemy = new CModelXAni(g_D3DDevice);
	//if(FAILED(tempEnemy->LoadXFile("model\\bear.x")))
	//	_Message("战斗模型bear.x初始失败！");
	//else
	//{
	//	//tempEnemy->SetPosition(3206, 0, 2338);
	//	tempEnemy->SetPosition(3206, 0, 2338);
	//	tempEnemy->IniDirection(1.0, 0.0, 0.0);
	//	tempEnemy->FaceTo(g_Jintian->GetPosition());


	//	tempEnemy->SetRenderSphere(false);
	//	//g_Jintian->SetVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//	tempEnemy->SetAniSpeed(10);
	//	//g_Jintian->SetScale(2.0f);
	//	tempEnemy->SetAnimation(BEAR_STAND);
	//	tempEnemy->SetScale(1.0);
	//	tempEnemy->SetBallScale(1.0f);
	//	tempEnemy->SetVel(0.0f);

	//	//g_Jintian->SetVel(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//	//g_Jintian->SetScale(2.0f);
	//	g_Jintian->SetMoving(false);

	//	g_enemyVector.push_back(tempEnemy);
	//}

	CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_FIGHT2);
	return true;
}

void ReleaseIntroduce()
{
	_SafeDelete(g_pGuiInterface);
}
void ReleaseInterface()
{
	_SafeDelete(g_pGuiInterface);
}
void ReleaseTown()
{
	_REDUCEREF(g_FileTex);
	_SafeDelete(g_pGuiTown);
	//CMusicMidi::getMidiSingleton().Play("music\\3.mid");

	//CSound::getSoundSingleton().Play("sound\\1.wav", 0, true);
	//CSound::getSoundSingleton().Play("5.wav", 1);
	//g_pModle->SetPosition(-200, 0, 200)

	//g_pFade = new CFade();
	//g_pFade->Initialize(g_D3DDevice);
	////g_pFade->SetFadeOut();
	//g_pFade->SetRange(800, 600);
	_SafeDelete(g_billboard);

	for(std::map<int, CTextureEx*>::iterator it = g_headmap.begin(); it != g_headmap.end();)
	{
		if((it->second) != NULL)
		{
			delete (it->second);
			g_headmap.erase(it);
			it = g_headmap.begin();
		}
		else
			it++;
	}
	g_headmap.clear();
	_SafeDelete(g_speakFrame);
	//_SafeClearVector(g_modelVector);
	//_SafeClearVector(g_alphaModelVector);
	//_SafeClearVector(g_aniModelVector);
	//_SafeClearVector(g_npcVector);
	//_SafeClearVector(g_collideSphere);
	//_SafeClearVector(g_collideBox);
	//_SafeClearVector(g_sndObject);
	//_SafeClearVector(g_partical);
}
void ReleaseOutdoors()
{
	_SafeDelete(g_terrain);
	g_OpenFog = false; 
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, g_OpenFog);
}
void ReleaseFight()
{
	_SafeDelete(g_pGuiFight);   
	_SafeDelete(g_pGuiFightInoBack); 
	_SafeDelete(spell);
	_SafeDelete(g_FightLabel);   
	_SafeDelete(g_BloodLabel);
	_SafeDelete(g_LoseTex);
	_SafeDelete(g_WinTex);

	//_SafeClearVector(g_enemyVector);

}

void RenderLoading()
{
	if(g_testPanel != NULL)
	{
		//g_testPanel->Update(g_delta);
		//g_testPanel->Render();
	}
}

void RenderIntroduce()
{
	btn->Render();
	_SafeRender(g_pGuiInterface);
	if(g_Font != NULL)
	{
		g_Font->UseColor(DexColor(0.4f, 0.4f, 0.8f));
		g_Font->SetPos(130, 140);
		g_Font->Print("引擎:阚正杰");
		g_Font->SetPos(130, 160);
		g_Font->Print("主程序：阚正杰");
		g_Font->SetPos(130, 200);
		g_Font->UseColor(DexColor(1.0f, 0, 0));
		g_Font->Print("注：");
		g_Font->SetPos(130, 220);
		g_Font->UseColor(DexColor(0.4f, 0.4f, 0.8f));
		g_Font->Print("    图片资源、wav音效提取自各代仙剑奇侠传，midi格式声音、3d模型来源于网络。这是作者的第一款3D游戏，引擎及主程序的优化还很不成熟，作者是个仙剑迷，这个游戏也算是向仙剑致敬，大四了，限于技术、时间、及精力等因素，本来想加的东西大多取消了，游戏非常简陋。。\n    作者另有仙剑主题小游戏《仙剑连连看》，有兴趣的可以去www.gameres.com下载\n    联系QQ:1339267050");
		g_Font->SetPos(450, 380);
		g_Font->Print("--2013.3");
		g_Font->ResetColor();
	}
}

void RenderInterFace()
{
	
	g_pGuiInterface->Render(GUI_ID_INFOBACKTOWER);
	if(g_SlowWeather != NULL)
	{
		g_SlowWeather->UpdatePartical();
		g_SlowWeather->Render();
	}
	_SafeRender(g_pGuiInterface);	 
	btn->Render();
}
void RenderTown()
{ 
	for(size_t i = 0; i < g_npcVector.size(); i++)
	{
		g_npcVector[i]->Update();
		g_npcVector[i]->Render();
	}	
	for(size_t i = 0; i < g_collideSphere.size(); i++)
	{
		g_collideSphere[i]->Render();
	}
	for(size_t i = 0; i < g_collideBox.size(); i++)
	{
		g_collideBox[i]->Render();
	}	  
	//g_pGuiInterface->Render();
	g_billboard->Update(g_camera.GetPosition());
	g_pSky->Render();

	static int frame = 0;
	static int delay = 5;
	if(delay-- < 0)
	{
		frame++;	 
		if(frame >= 20)
			frame = 0;
		delay = 5;
	}


	static int counter = 0;
	static int row = 0;
	static int col = 0;
	counter ++;
	if(counter > 3)
	{
		if(++col > 5)
		{
			if(++row > 3)
				row = 0;
			col = 0;
		}
		counter = 0;
	}
	RECT Rect;
	Rect.left = /*col*48*/ 0;
	Rect.right = Rect.left + 48;
	Rect.top = /*row*64*/0;
	Rect.bottom = Rect.top + 64;

#ifdef DEX_DEBGU

	CLine line(D3DXVECTOR3(1, 0, 1), D3DXVECTOR3(0, 0, 0));
	g_Font->SetPos(500, 200);
	CPlane plane(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,3,0), D3DXVECTOR3(0,0,2));

	g_Font->SetPos(500, 500);
	CBox box(D3DXVECTOR3(-100, 0, -100), D3DXVECTOR3(100, 100, 250));

	char cross[32];
	sprintf(cross, "%6.2f %6.2f %6.2f", g_crossPoint.x, g_crossPoint.y, g_crossPoint.z);
	g_Font->SetPos(20, 20);
	g_Font->Print("X动画速度vx,:%d vy:%d vz:%d ",(int)g_Jintian->GetVel().x, (int)g_Jintian->GetVel().y, (int)g_Jintian->GetVel().z);
	
	g_Font->SetPos(50,150);
	D3DXVECTOR3 tempos = g_Jintian->GetPosition();
	if(g_camera.GetFrustum().CheckPointIn(tempos))
		g_Font->Print("坐标%d %d %d可见!", (int)tempos.x, (int)tempos.y, (int)tempos.z);
	else
		g_Font->Print("坐标%d %d %d不可见!", (int)tempos.x, (int)tempos.y, (int)tempos.z);
#endif
	Mouse_Move_Dir dir;
	float reng = 0.05f;
	//g_camera.RotateSelf(DIR_UP, -0.002f);
	//g_camera.MoveCamera(DIR_RIGHT, 1);
	float vel = 4.0f;	
	D3DXVECTOR3 vec = g_camera.GetView();
	vec.y = 0;	
#ifdef DEX_DEBGU
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_LEFT))
	{
		g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_RIGHT))
	{
		g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_UP))
	{
		g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_DOWN))
	{
		g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_LEFT))
	{
		g_Jintian->SetDirection(-g_camera.GetRight());
		g_Jintian->SetMoving(true);
		g_Jintian->SetAnimation(3);
		static long v = 0;
		g_camera.MoveCamera(g_camera.GetRight(), -vel, true);
		//CMusicMidi::getMidiSingleton().SetVolume(v);
		v -= 5;
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_RIGHT))
	{
		g_Jintian->SetDirection(g_camera.GetRight());
		g_Jintian->SetMoving(true);
		g_Jintian->SetAnimation(3);
		g_task = 2;
		//CMusicMidi::getMidiSingleton().Play("4.mid");
		g_camera.MoveCamera(g_camera.GetRight(), vel, true);
		g_D3DDevice->SetCursorPosition(200,300,D3DCURSOR_IMMEDIATE_UPDATE);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_UP))
	{
		g_Jintian->SetAnimation(3);
		g_Jintian->SetDirection(vec);
		g_Jintian->SetMoving(true);
		//CMusicMidi::getMidiSingleton().Play("3.mid");
		g_camera.MoveCamera(vec, vel, true);
	}	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_P))
	{
		//CMusicMidi::getMidiSingleton().Play("3.mid");
		g_camera.SetCamera(D3DXVECTOR3(0,0, -600), D3DXVECTOR3(-100, 0, 100), g_camera.GetUpVector());
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_DOWN))
	{
		g_Jintian->SetDirection(-vec);
		g_Jintian->SetMoving(true);
		g_Jintian->SetAnimation(3);

		//CMusicMidi::getMidiSingleton().Play("1.mid");
		g_camera.MoveCamera(-vec, vel, true);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_SPACE))
	{
		g_camera.MoveCamera(CCamera::DIR_UP,5);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_T))
	{
		particalMgr->SetSeeTexState(!particalMgr->GetSeeTexState());
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_1))
	{
		particalMgr->SetParticalFace(EPFS_X);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_2))
	{
		particalMgr->SetParticalFace(EPFS_Y);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_3))
	{
		particalMgr->SetParticalFace(EPFS_Z);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_4))
	{	
		particalMgr->SetParticalFace(EPFS_CAM);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_5))
	{	
		particalMgr->SetParticalFace(EPFS_V_V);
	}
	if(CInputSystem::GetInputSingleton().KeyUp(DIK_6))
	{	
		particalMgr->SetParticalFace(EPFS_F_V);
	}
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_LCONTROL))
	{
		g_camera.MoveCamera(CCamera::DIR_UP, -5);
	}
#endif
	if(CInputSystem::GetInputSingleton().WheelFront())
	{
		//y -= 5;
		//g_camera.MoveCamera(DIR_VIEW, 5*vel);    //镜头拉近
		g_camera.RotateFocus(0.2f);
		//g_camera.MoveCamera(DIR_VIEW,5);
		//g_camera.RotateFocus(D3DXVECTOR3(-100, 0, 100), 0.2);
	}
	if(CInputSystem::GetInputSingleton().WheelBack())
	{
		//y += 5;
		//g_camera.MoveCamera(DIR_VIEW, -5*vel);	//镜头拉远
		g_camera.RotateFocus(-0.2f);
		//g_camera.MoveCamera(DIR_VIEW, -5);
		//g_camera.RotateFocus(D3DXVECTOR3(-100, 0, 100), -0.2);
	}
	static long volume = 100;


	D3DXMATRIX g_translation;
	D3DXMatrixTranslation(&g_translation, 0.0f, 0.0f, 0.0f);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &g_translation);

    for(size_t i = 0; i < g_sndObject.size(); i++)
	{
		g_sndObject[i]->Render();
	}   

	for(size_t i = 0; i < g_modelVector.size(); i++)
	{
		//if(g_camera.GetFrustum().CheckBallIn(g_modelVector[i]->GetBall()))
		{
			g_modelVector[i]->Update();
			g_modelVector[i]->Render();	
		}
	}
	for(size_t i = 0; i < g_aniModelVector.size(); i++)
	{
		if(g_camera.GetFrustum().CheckBallIn(g_aniModelVector[i]->GetBall()))
		{
			g_aniModelVector[i]->Update();
			g_aniModelVector[i]->Render();	
		}
	}
	if(g_OpenFog)
		g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, false);
	for(size_t i = 0; i < g_partical.size(); i++)
	{
		if(g_task == 2 && g_partical[i] == g_SlowWeather)
			continue;
		if(CheckPoint(g_partical[i]))
		{
			g_partical[i]->UpdatePartical();
			g_partical[i]->Render();
		}
	}  
	g_D3DDevice->SetRenderState(D3DRS_FOGENABLE, g_OpenFog);
	g_water->Update();
	g_water->Render();
	g_Jintian->Update();
	g_Jintian->Render();
	for(size_t i = 0; i < g_alphaModelVector.size(); i++)
	{
		if(g_camera.GetFrustum().CheckBallIn(g_alphaModelVector[i]->GetBall()))
		{
			g_alphaModelVector[i]->UpdateLength(g_camera.GetPosition());
			g_alphaModelVector[i]->Update();
			g_alphaModelVector[i]->Render();	
		}
	}
	//SortAlphaModel();


	//g_billboard->Render();

	//g_pFade->Update();
	//g_pFade->Render();
	if(g_State2 == PAL2_SPEAK)
		g_speakFrame->Show();
	
	//for(int i = 0 ; i< 1000; i++)
	_SafeRender(g_pGuiTown);
	if(g_task == 1)
	{
			if(g_Jintian->GetPosition().x < 10 && g_Jintian->GetPosition().x > -10
			&& g_Jintian->GetPosition().y < 10 && g_Jintian->GetPosition().y > -10
			&& g_Jintian->GetPosition().z < 10 && g_Jintian->GetPosition().z > -10)
			{
				g_palState = PAL_OUTDOORS;
				ApplyOutdoors();
				ReleaseTown();
			}
	}
	g_FireBill->Update(g_camera.GetPosition());
	g_FireBill->Render();

	//g_Ex->SetScale(1.0f, 1.0f);
	//g_Ex->SetAlpha(200);
	static bool bigger = true; 
	static float scale2 = 1.0;
	if(bigger)
	{
		scale2 += 0.02f;	
		if(scale2 >= 1)
		{
			bigger = false;
		}
	}
	else
	{
		scale2 -= 0.02f;	
		if(scale2 <= 0.5)
		{
			bigger = true;
		}
	}
	//g_Ex->SetAlphaColor(0x9000ff00);
	//g_Ex->SetScale(scale, scale);
	g_sceneNode->setMatrix(g_Jintian->GetMatrix());
	g_sceneNode->setDirection(g_Jintian->GetDirection());
	

	//particalMgr->Update(g_delta);
	//particalMgr->Render();
	g_pEmittInstance->Update(g_delta);
	g_pEmittInstance->Render();
	g_pScriptMgr->Run(g_delta);
	//DexGameEngine::getEngine()->Render3DLine(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,200,0), DexColor(1.0f, 0.0f,0.0f,1.0f), DexColor(0.0f, 1.0f,0.0f,1.0f));
	//DexGameEngine::getEngine()->RenderCube(D3DXVECTOR3(0, 0, 0));
	//DexGameEngine::getEngine()->RenderCube(D3DXVECTOR3(200, 0, 200));
	//DexGameEngine::getEngine()->RenderCube(D3DXVECTOR3(200, 282, 200));
	//DexGameEngine::getEngine()->RenderCube(D3DXVECTOR3(0, 282, 0));
	get2DDrawer()->BeginDraw2D();
	//g_Ex->Draw(g_D3DDevice, 50, 50);
	std::vector<DexGUI::DexPointF>  m_points; 
	DexGUI::DexPointF pnt;
	pnt.x = 100;
	pnt.y = 210;
	m_points.push_back(pnt);
	pnt.x = 210;
	pnt.y = 100;
	m_points.push_back(pnt);
	pnt.x = 310;
	pnt.y = 210;
	m_points.push_back(pnt);
	pnt.x = 210;
	pnt.y = 210;
	m_points.push_back(pnt);
	pnt.x = 310;
	pnt.y = 310;
	m_points.push_back(pnt);
	pnt.x = 310;
	pnt.y = 410;
	m_points.push_back(pnt);
	get2DDrawer()->SetCircleColor(255, 0,0,255);
	//get2DDrawer()->DrawLine(D3DXVECTOR2(0,0), D3DXVECTOR2(100, 100));
	get2DDrawer()->DrawCircle(m_points[0], 2.0f);
	get2DDrawer()->DrawCircle(m_points[m_points.size() - 2], 2.0f);
	get2DDrawer()->DrawCircle(m_points[m_points.size() - 1], 2.0f);
	get2DDrawer()->SetLineWidth(1);
	for(size_t i = 1; i < m_points.size()-2; i++)
	{
		int cut = 10; //两点间画多少线段描述曲线
		float add = 1.0f/10;
		for(float f = 0.0f; f < 1.0f;)
		{
			DexGUI::DexPointF p1 = CDexFun::get_catmull_value(m_points[i-1], m_points[i],m_points[i+1],m_points[i+2],f);
			DexGUI::DexPointF p2 = CDexFun::get_catmull_value(m_points[i-1], m_points[i],m_points[i+1],m_points[i+2],f+add);
			get2DDrawer()->DrawLine(p1, p2, 0xff00ff00, 0xff00ff00);
			f += add;
		}
		get2DDrawer()->DrawCircle(m_points[i], 2.0f);
	}
	g_testPanel->Update(g_delta);
	g_testPanel->Render();
	//get2DDrawer()->DrawRectLine(DexRectF(100,100,300,300), 0xffffff00);
	//get2DDrawer()->DrawRectLine(DexRectF(100,100,200,200), 0xffffff00);
	//get2DDrawer()->DrawRectLine(DexRectF(100,100,200,200), 0xffff0000);
	
	//g_Ex->Draw(g_D3DDevice, 50, 50);

	//g_testPanel->Update(g_delta);
	//g_testPanel->Render();

	//for(int i = 0 ; i < 100; i++)
	//	
	//{
		//g_Ex->SetAlpha(100);
		//g_Ex->DrawPart(g_D3DDevice, 10, 10, 100, 100, 100, 100);



		get2DDrawer()->EndDraw2D();

		/*g_Font->SetPos(50, 350);
		g_Font->Print("ime字符串%s", g_imeString.c_str());
		g_Font->SetPos(50, 400);
		g_Font->Print("与上帧相距%d ms", g_delta);
		g_Font->SetPos(50, 450);
		g_Font->Print("直属控件%d个，所有控件%d个", widget_container.GetThisWidgetCount(), widget_container.GetTotalWidgetCount());*/

		// Display the scene.
}

void RenderOutdoors()
{
	if(CInputSystem::GetInputSingleton().WheelFront())
	{
		g_camera.RotateFocus(0.2f);
	}
	if(CInputSystem::GetInputSingleton().WheelBack())
	{
		g_camera.RotateFocus(-0.2f);
	}
	D3DXVECTOR3 tempos = g_Jintian->GetPosition();

	g_Jintian->Update();
	g_Jintian->Render();

	_SafeRender(g_terrain);

	for(size_t i = 0; i < g_npcVector2.size(); i++)
	{
		if(g_npcVector2[i] == NULL)
			continue;  
		_SafeUpdate(g_npcVector2[i]);
		if(g_terrain != NULL)
		{
			float playerY = g_terrain->GetHeight(g_npcVector2[i]->GetPosition().x, g_npcVector2[i]->GetPosition().z);
			g_npcVector2[i]->SetPositionY(playerY);
		}
		_SafeRender(g_npcVector2[i]);
	}
	if(g_npcVector2.size() != NULL)
	{
		stBall temp_ball = g_npcVector2[0]->GetBall();
		if(g_Jintian->GetBall().GetRelation(temp_ball) == 0)
		{//相交,检测到碰撞 
			g_palState = PAL_FIGHT;
			ApplyFight();
		}	
	}
}
void RenderFight()
{	
	g_Jintian->Update();
	g_Jintian->Render(); 
	g_terrain->Render();

	_SafeRender(g_pGuiFightInoBack); 
	if(g_BloodLabel !=NULL)
	{
		g_BloodLabel->Update();
		g_BloodLabel->Render();
	}
	if(g_Jintian->GetIsJustAttack())
	{
		CNumberAni *temp = new CNumberAni(g_D3DDevice);
		if(!temp->LoadTexture("res\\number_enmy.png"))
		{
			_SafeDelete(temp);
		}
		else
		{
			int att = g_Jintian->GetAttack();  
			temp->SetNumber(att);
			temp->SetOpenSign(true);
			POINT p = g_pPick->GetXY(g_projection, g_ViewMatrix, g_Jintian->GetDestPos());
			temp->SetPos(p.x, p.y);
			temp->SetSpeed(1);
			temp->SetEndPos(p.x, p.y - 70);
			g_FightNumbers.push_back(temp);
			if(g_SelectEnemy != NULL)
				g_SelectEnemy->ReduceHp(att);
		}
	}
	if(g_FightStage == FIGHT_ATTACKING)
	{//正处于攻击阶段，则判断攻击是否完成
		bool allflag = true;  //是否都在待命
		if(g_Jintian->GetAttackState() != ATTACK_FLAG)
			allflag = false;
		for(size_t i = 0; i < g_enemyVector.size(); i++)
		{
			if(g_enemyVector[i]->GetAttackState() != ATTACK_FLAG)
			{//只要有一个不在待命，证明还处在攻击阶段
				allflag = false;
				break;
			}
		}
		if(allflag)
		{//全部待命,进度条继续走
			g_FightStage = FIGHT_LABEL;
		}
	}	 
	else if(g_FightStage == FIGHT_SKILLING)
	{//正处于施法阶段  
		for(size_t i = 0; i < g_Skills.size(); i++)
		{
			if(g_Skills[i] != NULL)
			{
				if(g_Skills[i]->GetIsComplete())
				{
					CNumberAni *temp = new CNumberAni(g_D3DDevice);
					if(!temp->LoadIni("ini\\skill.ini", "PlayerAddBloodNumber"))
					{
						_SafeDelete(temp);
					}
					TSHORT num = g_Skills[i]->GetNum();
					temp->SetNumber(num);
					g_BloodLabel->AddNum(num);
					POINT p = g_pPick->GetXY(g_projection, g_ViewMatrix, g_Jintian->GetBallTop());
					temp->SetPos(p.x, p.y);
					temp->SetSpeed(1);
					temp->SetEndPos(p.x, p.y - 70);
					g_FightNumbers.push_back(temp);
					_SafeDelete(g_Skills[i]);
				}
			}
		}
		for(std::vector<CSkill*>::iterator it = g_Skills.begin(); it !=g_Skills.end();)
		{
			if((*it) == NULL)
			{
				g_Skills.erase(it);
				it = g_Skills.begin();
			}
			else
				it++;
		}
		if(g_Skills.size() == 0)
			g_FightStage = FIGHT_LABEL;
	}
	for(size_t i = 0; i < g_enemyVector.size(); i++)
	{
		if(g_enemyVector[i] != NULL)
		{
			g_enemyVector[i]->Update();
			g_enemyVector[i]->Render();
			if(g_enemyVector[i]->GetIsJustAttack())
			{
				CNumberAni *temp = new CNumberAni(g_D3DDevice);
				if(!temp->LoadTexture("res\\number_player.png"))
				{
					_SafeDelete(temp);
					break;
				}  
				int att = g_enemyVector[i]->GetAttack();
				temp->SetNumber(att);
				int num = g_BloodLabel->GetCurrNum();
				num -= att;
				if(num <= 0)
					g_BloodLabel->SetCurrNum(0);
				else
					g_BloodLabel->SetCurrNum(num);
				POINT p = g_pPick->GetXY(g_projection, g_ViewMatrix, g_Jintian->GetBallTop());
				temp->SetOpenSign(true);
				temp->SetPos(p.x, p.y);
				temp->SetSpeed(1);
				temp->SetEndPos(p.x, p.y - 70);
				g_FightNumbers.push_back(temp);
				g_Jintian->SetCurrHp(g_BloodLabel->GetCurrNum());
				if(g_BloodLabel->GetCurrNum() <= 0)	   //进入失败阶段
				{
					g_FightStage = FIGHT_LOSE;
					CMusicMidi::getMidiSingleton().Stop();
					CSound::getSoundSingleton().Play(PAL_SOUND_LOSE, PAL_SOUND_INDEX2);
					g_FightEndDelay = 0;
				}
			}
		}
	}
	if(!g_camera.GetIsMoving() && g_FightStage == FIGHT_MOVE_CAMERA)
	{//摄像机移动到位，进入进度条移动阶段
		g_FightStage = FIGHT_LABEL;
	}
	for(size_t i = 0; i < g_FightNumbers.size(); i++)
	{
		if(g_FightNumbers[i] != NULL)
		{
			g_FightNumbers[i]->Update();
			g_FightNumbers[i]->Render();
			if(g_FightNumbers[i]->GetComplete())
			{
				_SafeDelete(g_FightNumbers[i]);
				g_FightNumbers[i] = NULL;
			}
		}
	}
	for(std::vector<CNumberAni*>::iterator it = g_FightNumbers.begin(); it !=g_FightNumbers.end();)
	{
		if((*it) == NULL)
		{
			g_FightNumbers.erase(it);
			it = g_FightNumbers.begin();
		}
		else
			 it++;
	}
	switch(g_FightStage)
	{
	case FIGHT_MOVE_CAMERA:
		{
			//摄像机移动阶段
			_SafeUpdate(g_FightLabel);
			_SafeRender(g_FightLabel);
			break;
		}
	case FIGHT_LABEL:
		{
			_SafeUpdate(g_FightLabel);
			_SafeRender(g_FightLabel);
			if(g_FightLabel != NULL)
			{
				if(g_FightLabel->ThereIsComplete())
				{//有头像到达，进入选取指令阶段
					if(g_FightLabel->GetFighterHead() != NULL)
					{
						if(g_FightLabel->GetFighterHead()->GetIsPlayer())
						{//由玩家操纵的角色	
							g_FightStage = FIGHT_SELECT_ORDER; 
						}
						else
						{//是敌人，由电脑操作
							if(g_FightLabel->GetFighterHead()->GetEnemy() != NULL)
							{
								g_FightLabel->GetFighterHead()->GetEnemy()->SetAttacking(true);
								g_FightStage = FIGHT_ATTACKING;
							}
						}
					}
					g_FightLabel->ResetFighteHead();
				}
			}
			//判断游戏是否结束，之所以不在刚攻击完判断，是因为可能造成景天还在对方位置时就已进入胜利阶段了
			//判断敌人是否全部死亡
			bool win = true;
			for(size_t i = 0; i < g_enemyVector.size(); i++)
			{
				if(CheckPoint(g_enemyVector[i]))
				{
					if(g_enemyVector[i]->GetCurrHp() > 0)
					{
						win = false;
						break;
					}
				}
			}
			if(win)
			{
				g_FightStage = FIGHT_WIN;
				CMusicMidi::getMidiSingleton().Stop();
				CSound::getSoundSingleton().Play(PAL_SOUND_WIN, PAL_SOUND_INDEX2);
				g_camera.EnableMove(true);
				g_camera.SetFocus(g_Jintian->GetPosition());
				g_camera.SetDestPos(3206, 50, 2338);
				g_camera.SetMoveVel(1.5);
				g_task = 2;
			}
			break;
		}
	case FIGHT_SELECT_ORDER:
		{
			_SafeRender(g_FightLabel);
			_SafeRender(g_pGuiFight);
			break;
		}
	case FIGHT_SELECT_TARGET:
		{
			_SafeRender(g_FightLabel);
			if(spell != NULL)
			{
				spell->Update();
				spell->Render();
			}
			for(size_t i = 0; i < g_enemyVector.size(); i++)
			{
				if(g_enemyVector[i] != NULL)
				{
					if(g_Font != NULL && g_enemyVector[i]->GetCurrHp() > 0)
					{//显示生命
						g_Font->UseColor(DexColor(1.0f, 1.0f, 1.0f));
						POINT p = g_pPick->GetXY(g_projection, g_ViewMatrix, g_enemyVector[i]->GetBallTop());
						g_Font->SetPos(p.x, p.y);
						g_Font->Print("%d/%d", g_enemyVector[i]->GetCurrHp(), g_enemyVector[i]->GetHp());
						g_Font->ResetColor();
					}
				}
			}
			break;
		}
	case FIGHT_ATTACKING:    //正在攻击
		{
			if(g_FightLabel != NULL)
			{	
				if(!g_FightLabel->ThereIsComplete())
				{//没有头像到达，继续进度条，如果有头像到达，则不进行更新，知道一轮攻击过后再进入选取指令阶段
					g_FightLabel->Update();
				}
				g_FightLabel->Render();

			}
			break;
		}
	case FIGHT_SKILLING:	 //正在施法
		{
			for(size_t i = 0; i < g_Skills.size(); i++)
			{
				if(g_Skills[i] != NULL)
				{
					g_Skills[i]->Update();
					g_Skills[i]->Render();
				}
			}
			if(g_FightLabel != NULL)
			{	
				if(!g_FightLabel->ThereIsComplete())
				{//没有头像到达，继续进度条，如果有头像到达，则不进行更新，知道一轮攻击过后再进入选取指令阶段
					g_FightLabel->Update();
				}
				g_FightLabel->Render();

			}
			break;
		}
	case FIGHT_LOSE:
		{	
			if(g_LoseTex != NULL)
			{
				g_LoseTex->Update();
				g_LoseTex->Render();
			}
			g_FightEndDelay++;
			if(g_FightEndDelay >= CIni::GetInt("ini\\data.ini", "GameFailDelay", "time"))
			{
			    PostQuitMessage(0);
			}
			break;
		}
	case FIGHT_WIN:
		{
			g_FightEndDelay++;
			if(g_FightEndDelay >= CIni::GetInt("ini\\data.ini", "GameFailDelay", "time"))
			{
				g_palState = PAL_TOWN;
				ApplyTown();
				ReleaseFight();
				ReleaseOutdoors();
				g_Jintian->SetPosition(560, 0, -30);
				g_Jintian->FaceTo(D3DXVECTOR3(0,0,0));
				g_camera.SetFocus(g_Jintian->GetPosition());
				g_camera.SetPosition(397, 70, 93);
				g_task = 2; 
			}
			if(CheckPoint(g_WinTex))
			{
				g_WinTex->Draw(g_D3DDevice, 80, 70);
			}
			break;
		}
	default:
		break;
	}

}


void IniMesh()
{
	D3DXCreateSphere(g_D3DDevice, 50, 10, 10,  &g_sphere, NULL);
	D3DXCreateSphere(g_D3DDevice, 5, 10, 10,  &g_lightShere, NULL);
	D3DXCreateBox(g_D3DDevice, 100, 100, 100, &g_cube, NULL);
	D3DXCreateTeapot(g_D3DDevice, &g_teapot, NULL);

	memset(&g_material, 0, sizeof(D3DXMATERIAL));
	g_material.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_material.Diffuse  = D3DXCOLOR(0.75f, 0.0f, 0.0f, 1.0f);
	g_material.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_material.Power = 1000;


	memset(&g_material2, 0, sizeof(D3DXMATERIAL));
	g_material2.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_material2.Diffuse  = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
	g_material2.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_material2.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_material2.Power = 0;

	memset(&g_material3, 0, sizeof(D3DXMATERIAL));
	g_material3.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_material3.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_material3.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_material3.Emissive = D3DXCOLOR(0.1, 0.1, 0.1, 1.0f);
	g_material3.Power = 100;

	LPD3DXBUFFER	pbuff = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(g_D3DDevice, "shader\\first_shader.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL,
		&g_pEffect, &pbuff);
	if(pbuff)
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, (char*)(pbuff->GetBufferPointer()));
		pbuff->Release();
	}
	if(FAILED(hr))
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, "D3DXCreateEffectFromFile 调用失败！");
	}
}
void HandleMesh()
{
	int nType = 0;
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_1))
		nType = 1;
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_2))
		nType = 2;
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_3))
		nType = 3;

	if(CInputSystem::GetInputSingleton().KeyDown(DIK_LEFT))
	{
		g_lightPos -= g_camera.GetRight()*2;
	}
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_RIGHT))
	{
		g_lightPos += g_camera.GetRight()*2;
	}
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_DOWN))
	{
		D3DXVECTOR3 vec;
		vec = g_camera.GetView();
		vec.y = 0;
		g_lightPos -= vec*2;
	}
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_UP))
	{
		D3DXVECTOR3 vec;
		vec = g_camera.GetView();
		vec.y = 0;
		g_lightPos += vec*2;
	}
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_P))
	{
		g_lightPos += g_camera.GetUpVector()*2;
	}
	else if(CInputSystem::GetInputSingleton().KeyDown(DIK_L))
	{
		g_lightPos -= g_camera.GetUpVector()*2;
	}
	g_light.Position      = g_lightPos;
	//g_light.Position      = g_Jintian->GetPosition();
	//g_light.Position.z += 200;
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_6))
		g_material.Power += 1;
	g_light.Direction      = g_Jintian->GetPosition() - g_lightPos;
	switch (nType)
	{
	case 1:     //点光源
		g_light.Type          = D3DLIGHT_POINT;
		g_light.Ambient       = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f); 
		g_light.Diffuse       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_light.Specular      = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_light.Position      = g_lightPos;
		g_light.Attenuation0  = 1.0f;
		g_light.Attenuation1  = 0.0f;
		g_light.Attenuation2  = 0.0f;
		g_light.Range         = 1000.0f;
		break;
	case 2:     //平行光
		g_light.Type          = D3DLIGHT_DIRECTIONAL;
		g_light.Ambient       = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		g_light.Diffuse       = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		g_light.Specular      = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//g_light.Direction     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		break;
	case 3:     //聚光灯
		g_light.Type          = D3DLIGHT_SPOT;
		g_light.Position      = g_lightPos;
		//g_light.Direction     = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		g_light.Ambient       = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		g_light.Diffuse       = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		g_light.Specular      = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		g_light.Attenuation0  = 1.0f; 
		g_light.Attenuation1  = 0.0f; 
		g_light.Attenuation2  = 0.0f; 
		g_light.Range         = 1000.0f;
		g_light.Falloff       = 0.1f;
		g_light.Phi           = D3DX_PI / 2.0f;
		g_light.Theta         = D3DX_PI / 2.0f;
		break;
	}
}
void RenderLightMesh()
{
	g_Jintian->Update();
	g_Jintian->Render();	

	g_D3DDevice->SetMaterial(&g_material);
	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_D3DDevice->SetLight(0, &g_light); //设置光源
	g_D3DDevice->LightEnable(0, true);//启用光照
	g_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_D3DDevice->SetRenderState(D3DRS_AMBIENT, getD3DColor(DexColor(0.5f, 0.5f, 1.0f)));  



	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 0.0f);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	//g_sphere->DrawSubset(0);

	UINT npass;
	g_pEffect->SetTechnique("PS_CARTOON");
	g_pEffect->Begin(&npass, 0);
	for(int i=0; i<npass; ++i)
	{
		g_pEffect->BeginPass(i);
		g_sphere->DrawSubset(0);
		g_pEffect->EndPass();
	}
	g_pEffect->End();

	g_D3DDevice->SetMaterial(&g_material2);
	D3DXMatrixTranslation(&translation, 100.0f, 0.0f, 0.0f);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	g_pEffect->SetTechnique("PS_CARTOON");
	g_pEffect->Begin(&npass, 0);
	for(int i=0; i<npass; ++i)
	{
		g_pEffect->BeginPass(i);
		g_cube->DrawSubset(0);	
		g_pEffect->EndPass();
	}
	g_pEffect->End();

	D3DXMatrixTranslation(&translation, g_lightPos.x, g_lightPos.y, g_lightPos.z);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	g_lightShere->DrawSubset(0);



	D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 0.0f);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 10.0f, 10.0f, 10.0f);
	//D3DXMatrixScaling(&scale, 100.0f, 100.0f, 100.0f);
	D3DXMATRIX matrix;
	//D3DXMatrixMultiply(&matrix, &matrix,  &translation);
	D3DXMatrixMultiply(&matrix, &scale,  &translation);
	D3DXMatrixTranslation(&translation, 0.0f, 0.0f, 300.0f);
	D3DXMatrixMultiply(&matrix, &matrix,  &translation);
	g_D3DDevice->SetTransform(D3DTS_WORLD, &matrix);
	g_teapot->DrawSubset(0);

	g_D3DDevice->SetMaterial(&g_material3);
	g_water->Update();
	g_water->Render();

	g_D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_D3DDevice->SetRenderState(D3DRS_SPECULARENABLE, false);
}

void SortAlphaModel()
{
	for(size_t i = 0; i < g_alphaModelVector.size() ; i++)
	{
		for(size_t j = i ; j < g_alphaModelVector.size() ; j++)
		{
			if(g_alphaModelVector[j]->GetZLength() >  g_alphaModelVector[i]->GetZLength())
			{
				g_tempAlpha = g_alphaModelVector[j];
				g_alphaModelVector[j] = g_alphaModelVector[i];
				g_alphaModelVector[i] = g_tempAlpha;
			}
		}
	}
}


DWORD WINAPI LoadThread1(LPVOID lpParam)
{
	while(1)
	{
		if(g_palState == PAL_LOADING)
		{//在这里进行资源的初始化 这个时候加载界面应该在显示了

			//g_iLoadRate = 31;
			//g_strLoadDes = "正在加载UI资源……";
			//CDexUiTexFactory::AddFilePath("../res/");
			//CDexUiTexFactory::AddFilePath("../res/button/");
			//getUiSrcMgrSingleton()->createUiTexFactory(g_D3DDevice, ui_src_state_interface, "../ini/ui_tex1.ini");
			//getUiSrcMgrSingleton()->setCurrState(ui_src_state_interface);

			//g_iLoadRate = 50;
			//g_strLoadDes = "正在加载光标资源……";
			//g_Cursors[0] = LoadCursorFromFile("res\\Cursor.cur");
			//g_Cursors[1] = LoadCursorFromFile("res\\cs_duihua.ani");
			//g_Cursors[2] = LoadCursorFromFile("res\\cs_gongji.ani");
			//SetCursor(g_Cursors[0]);



			//g_iLoadRate = 55;
			//g_strLoadDes = "初始化拾取信息……";
			//g_pPick = new CPick();   
			//g_pPick->Initial(g_D3DDevice);



			////if(int error = luaL_dofile(L, "script\\fight.lua") != 0)   //打开文件并编译
			////{
			////	_Message("初始化脚本fight.lua出错！");
			////	return false;
			////}

			//g_iLoadRate = 60;
			//g_strLoadDes = "初始化自主脚本信息……";
			//g_pScriptMgr = new CDexScriptMgr;			
			////注意 浮点数必须加小数点 否则会被解析为整数
			////如果是负数如 -100.0 则必须写成 0.0-100.0的形式
			//g_pScriptMgr->AddCommand("MoveCamera(200.0,100.0,100.0,0.5)");	
			//g_pScriptMgr->AddCommand("Wait(15000)");
			//g_pScriptMgr->AddCommand("MoveCamera(500.0,200.0,200.0,0.5)");
			//g_pScriptMgr->AddCommand("Wait(15000)");
			//g_pScriptMgr->AddCommand("MoveCamera(0.0,0.0,0.0,0.5)");
			//g_pScriptMgr->AddCommand("Wait(15000)");

			//InitScriptExpression();
			////if(g_palState == PAL_INTERFACE)
			////{
			////	CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_WENQING);
			////	if(!ApplyInterface())
			////		return false;
			////}
			////if(g_palState == PAL_TOWN)
			////{
			////	if(!ApplyTown()) 
			////		return false;
			////}
			////else if(g_palState == PAL_OUTDOORS)
			////{
			////	if(!ApplyOutdoors()) 
			////		return false;
			////}

			//g_State2 = PAL2_FLAG;
			//g_clickId = 0;
			//
			//g_iLoadRate = 70;
			//g_strLoadDes = "初始化特效粒子……";
			//ApplyTown();

			//g_iLoadRate = 90;
			//g_strLoadDes = "初始化界面……";
			//g_testPanel = new CTestPanel();
			//g_testPanel->Init();

			//g_palState = FirstState;	

		}
	}
	
	return 0; //线程完成时推出返回0
}