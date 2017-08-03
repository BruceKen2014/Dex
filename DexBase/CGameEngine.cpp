

#include "CGameEngine.h"






CGameEngine::CGameEngine()
{
	m_fps = 0;
	m_counter = 0;
	m_currentTime = 0;
	m_lastTime = 0;
}

CGameEngine::~CGameEngine()
{
}
bool CGameEngine::Check()
{
	if( m_device == NULL)  return false;
	return true;
}

D3DMULTISAMPLE_TYPE CGameEngine::GetSampleType(DEMO_SAMPLE_TYPE _type, D3DDEVTYPE type, D3DFORMAT format, bool fullscreen)
{
	D3DMULTISAMPLE_TYPE t = D3DMULTISAMPLE_NONE;

	switch(_type)
	{
	case DEMOEX_SAMPLE_NONE:
		{
			return t;
			break;
		}

	case DEMOEX_SAMPLE_2:
		{
			t = D3DMULTISAMPLE_2_SAMPLES;
			break;
		}

	case DEMOEX_SAMPLE_4:
		{
			t = D3DMULTISAMPLE_4_SAMPLES;
			break;
		}
	case DEMOEX_SAMPLE_8:
		{
			t = D3DMULTISAMPLE_8_SAMPLES;
			break;
		}
	case DEMOEX_SAMPLE_16:
		{
			t = D3DMULTISAMPLE_16_SAMPLES;
			break;
		}
	default:
		return t;
		break;
	}
	if(m_direct3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		type, format, !fullscreen, t,
		NULL) == D3D_OK)
		return t;
	return D3DMULTISAMPLE_NONE;
}
bool CGameEngine::Initialize(DInt32 width, DInt32 height, HWND hWnd, bool fullscreen, DEMO_SAMPLE_TYPE _type)
{
	ShutDown();
	m_window = hWnd;
	if(!m_window)
		return false;
	m_fullscreen = fullscreen;

	D3DDISPLAYMODE mode;
	D3DCAPS9 caps;
	D3DPRESENT_PARAMETERS Params;

	ZeroMemory(&Params, sizeof(Params));

	m_direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_direct3D)
		return false;

	if(FAILED(m_direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
		&mode))) 
		return false;

	if(FAILED(m_direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, &caps))) 
		return false;

	DWORD processing = 0;
	if(caps.VertexProcessingCaps != 0)
		processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if(m_fullscreen)
	{
		Params.FullScreen_RefreshRateInHz = mode.RefreshRate;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		//Params.FullScreen_RefreshRateInHz = mode.RefreshRate;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		//Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	Params.Windowed = !m_fullscreen;
	Params.BackBufferWidth = width;
	Params.BackBufferHeight = height;
	Params.hDeviceWindow = m_window;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.BackBufferFormat = mode.Format;
	Params.BackBufferCount = 1;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.MultiSampleType = GetSampleType(_type,
		D3DDEVTYPE_HAL, mode.Format, m_fullscreen);

	m_width = width;
	m_height = height;

	if(FAILED(m_direct3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, m_window, processing,
		&Params, &m_device))) 
		return false;

	if(m_device == NULL) 
		return false;

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);

	D3DXMatrixPerspectiveFovLH(&m_projection, PI/4,
		(float)m_width/(float)m_height, 0.1f, 1000);

	m_device->SetTransform(D3DTS_PROJECTION, &m_projection);


	//其他成员变量初始化
	m_clearColor = DexColor(0.5f, 0.5f, 0.5f);
	m_sleep      = false;
	m_canRender  = false;
	m_openFog    = false;
	return true;
}

void CGameEngine::SetSamState(DEMOEX_SAM_STATE sam_state, int flag)
{
	switch(sam_state)
	{
	case SAM_POINT:
		{
			m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			break;
		}
	case SAM_LINER:
		{
			m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			break;
		}
	case SAM_ANISOTROPY:
		{
			if(flag < 0)
				flag = 0;
			m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, flag);
			break;
		}
	default:
		{
			m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			break;
		}
	}
}

LPDIRECT3DDEVICE9 CGameEngine::GetDevice()
{
	return m_device;
}

int CGameEngine::GetFps()
{
	return m_fps;
}

void CGameEngine::SetClearColor(DexColor color)
{
	m_clearColor = color;
}
void CGameEngine::ShutDown()
{
}

bool CGameEngine::GetSleep()
{
	return m_sleep;
}

void CGameEngine::SetSleep(bool sleep)
{
	m_sleep = sleep;
}

void CGameEngine::StartRender(bool color, bool zbuffer, bool stencil)
{
	if(!Check()) 
		return;

	DUDInt32 buffers = 0;
	if(color) buffers |= D3DCLEAR_TARGET;
	if(zbuffer) buffers |= D3DCLEAR_ZBUFFER;
	if(stencil) buffers |= D3DCLEAR_STENCIL;

	if(FAILED(m_device->Clear(0, NULL, buffers, getD3DColor(m_clearColor), 1, 0)))
		return;
	if(FAILED(m_device->BeginScene())) 
		return;

	m_canRender = true;	

	m_currentTime = GetTickCount();
	if(m_currentTime - m_lastTime > 1000.0f)
	{
		m_lastTime = m_currentTime;
		m_fps = m_counter;
		m_counter = 0;
	}
	else
		m_counter++;
}

void CGameEngine::EndRender()
{
	if(!Check()) return;

	m_device->EndScene();
	m_device->Present(NULL, NULL, NULL, NULL);

	m_canRender = false;
}

void CGameEngine::SetFog(DEMOEX_FOG_TYPE fog_type, DEMOEX_FOG_MODE fog_mode,float start, float end, float density, const DexColor& color, bool range)
{
	m_fog.m_fogType   = fog_type;
	m_fog.m_fogMode   = fog_mode;
	m_fog.m_fogStart  = start;
	m_fog.m_fogEnd    = end;
	m_fog.m_fogdensity= density;
	m_fog.m_fogColor  = color;
	m_fog.m_rangerFog = range;
}

void CGameEngine::SetFog(stFog _fog)
{
	m_fog = _fog;
}

void CGameEngine::EnableFog()
{
	if(!Check()) return;

	m_openFog = true;
	D3DCAPS9 caps;
	m_device->GetDeviceCaps(&caps);

	m_device->SetRenderState(D3DRS_FOGENABLE, true);
	m_device->SetRenderState(D3DRS_FOGCOLOR, getD3DColor(m_fog.m_fogColor));

	m_device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fog.m_fogStart));
	m_device->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fog.m_fogEnd));

	D3DRENDERSTATETYPE  first = D3DRS_FOGVERTEXMODE;
	DWORD               second = D3DFOG_LINEAR; 
	if(m_fog.m_fogType == DEMOEX_PIXEL_FOG)
		first  = D3DRS_FOGTABLEMODE;
	switch (m_fog.m_fogMode)
	{
	case DEMOEX_FOG_EXP:
		{
			second = D3DFOG_EXP;
			break;
		}
	case DEMOEX_FOG_EXP2:
		{
			second = D3DFOG_EXP2;
			break;
		}
	default:
		{
			second = D3DFOG_LINEAR;
			break;
		}
	}
	m_device->SetRenderState(first, second);
	m_device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fog.m_fogdensity));

	if(m_fog.m_rangerFog)
	{
		if(caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE)
			m_device->SetRenderState(D3DRS_RANGEFOGENABLE, true);
	}
	else
			m_device->SetRenderState(D3DRS_RANGEFOGENABLE, false);
}

void CGameEngine::DisableFog()
{
	if(!Check())
		return;
	m_device->SetRenderState(D3DRS_FOGENABLE, false);
	m_openFog = false;
}

bool CGameEngine::GetOpenFog()
{
	return m_openFog;
}

void CGameEngine::AddLight(CLight * light)
{
	m_Lights.push_back(light);
	m_device->SetLight(m_Lights.size(), &light->GetLight());
}

void CGameEngine::EnableLight()
{
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CGameEngine::DisableLight()
{
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CGameEngine::EnableLightId(DInt32 lightid)
{
	for( size_t i = 0; i < m_Lights.size(); i++)
	{
		if(m_Lights[i]->GetId() == lightid)
			m_device->LightEnable(i, TRUE);
	}
}

void CGameEngine::EnableLightIndex(DInt32 index)
{
	if(index < 0)
		return;
	m_device->LightEnable(index, TRUE);
}

void CGameEngine::DisableLightId(DInt32 lightid)
{
	for( size_t i = 0; i < m_Lights.size(); i++)
	{
		if(m_Lights[i]->GetId() == lightid)
			m_device->LightEnable(i, FALSE);
	}
}

void CGameEngine::DisableLightIndex(DInt32 index)
{
	if(index < 0)
		return;
	m_device->LightEnable(index, FALSE);
}

void CGameEngine::AddXmodel(CModelX * model)
{
	m_XModels.push_back(model);
}

void CGameEngine::RenderXModel()
{
	for(size_t i = 0; i < m_XModels.size(); i++)
	{
		m_XModels[i]->Render();
	}
}